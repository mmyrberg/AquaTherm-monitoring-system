#ifndef CREDENTIALS_H
#define CREDENTIALS_H

/*!
* If OTAA is used
* LoRaWAN Device EUI generate method (select in arduino IDE tools, Tools -> LoRaWan_DevEUI):
* CUSTOM – Defined by the user in the DevEui array of the code, 8 bytes;
* Generate By ChipID – Generated according to the Chip ID of the chip. Selecting this option will override the setting in the DevEui array in the code.
*/
uint8_t devEui[] = {  };
uint8_t appEui[] = {  };
uint8_t appKey[] = {  };

/*If ABP i used*/
uint8_t nwkSKey[] = {  };
uint8_t appSKey[] = {  };
uint32_t devAddr =  (uint32_t)0x007e6ae1;

#endif // CREDENTIALS_H