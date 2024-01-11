/*
* LoRaWan and OLED code is based on examples from official Arduino library for Heltec ESP32 (or ESP32+LoRa) based boards
* DS18B20 temperature sensor code based on examples from official Arduino library for Dallas Temperature ICs
* https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
* https://github.com/milesburton/Arduino-Temperature-Control-Library
*/

#include <LoRaWan_APP.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <HT_SSD1306Wire.h>
#include "credentials.h"

/*Pin where the DS18B20 sensor is connected to the ESP32*/
#define ONE_WIRE_BUS 3

/*Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)*/
OneWire oneWire(ONE_WIRE_BUS);

/*Pass our oneWire reference to Dallas Temperature sensor*/
DallasTemperature sensors(&oneWire);

/*Init tempertature*/
float temperature = 0.0; 

/*OLED Display setup: addr , freq , i2c group , resolution , rst*/
SSD1306Wire displayMcu(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); 

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 3000000;

/*OTAA or ABP*/
bool overTheAirActivation = true;

/*ADR enable*/
bool loraWanAdr = true;

/*Indicates if the node is sending confirmed or unconfirmed messages*/
bool isTxConfirmed = true;

/*Application port*/
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;

/* Prepares the payload of the frame */
static void prepareTxFrame(uint8_t port) {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  if(temperature != DEVICE_DISCONNECTED_C) {
    int16_t tempInt = temperature * 100;

    appDataSize = 2;
    appData[0] = tempInt >> 8;
    appData[1] = tempInt & 0xFF;
  } else {
    Serial.println("Error: Sensor disconnected");
    //Send an error code to TTN
    appDataSize = 1;
    appData[0] = 0xFF;
  }
}

void displayTemperature(float temperature) {
  displayMcu.clear();
  displayMcu.setTextAlignment(TEXT_ALIGN_CENTER);
  displayMcu.setFont(ArialMT_Plain_24);

  if(temperature != DEVICE_DISCONNECTED_C) {
    displayMcu.drawString(displayMcu.getWidth() / 2, displayMcu.getHeight() / 2, String(temperature, 2) + " °C");
  } else {
    displayMcu.drawString(displayMcu.getWidth() / 2, displayMcu.getHeight() / 2, "Sensor Error");
  }
  displayMcu.display();
}

void setup() {
  Serial.begin(115200);
  Mcu.begin();
  sensors.begin();
  displayMcu.init();
  displayMcu.flipScreenVertically();

  deviceState = DEVICE_STATE_INIT;
}

void loop() {
  switch(deviceState) {
    case DEVICE_STATE_INIT:
    {
#if(LORAWAN_DEVEUI_AUTO)
      LoRaWAN.generateDeveuiByChipID();
#endif
      LoRaWAN.init(loraWanClass,loraWanRegion);
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      prepareTxFrame(appPort);
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      /*Display temperature reading on OLED for 1 minute*/
      displayTemperature(temperature);
      delay(600000);
      /*Schedule next packet transmission*/
      txDutyCycleTime = appTxDutyCycle + randr(-APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND);
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.sleep(loraWanClass);
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}