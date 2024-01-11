# Sammanfattning - En IoT-lösning för Vattentemperaturmätning

> [För fullständig rapport, klicka här](./Examensarbete-Rapport-MartinMyrberg.pdf)

## Om projektet
Projektet syftar till att utveckla en IoT-lösning för att förbättra tillgängligheten och noggrannheten av information om badvattentemperaturer på en ö i Stockholm skärgård.

## Frågeställningar
För att besvara syftet formulerades två frågeställningar:
1. Hur kan en IoT-lösning utformas för att effektivt samla in och överföra realtidsdata om 
badvattentemperaturen?
2.  På vilket sätt kan informationen presenteras på ett användarvänligt sätt?

## Metod
Genomförandet av projektet bestod av:
- Sensorinstallation
- Dataöverföring via LoRaWAN till The Things Network
- Integration med Amazon Web Services för datalagring och eventhantering
- Visualisering genom Grafana-dashboard och Discord-server

## Resultat
- En heltäckande IoT-lösning för realtidsövervakning av vattentemperaturen
- Tillgängligheten förbättrades genom att göra informationen lättåtkomlig via en webblänk
- Noggrannheten förbättrades genom möjliggörandet av lokala temperaturmätningar

![IoT-arkitektur](https://github.com/mmyrberg/AquaTherm-monitoring-system/blob/main/Images/Arkitektur.png)

## Vidareutveckling
Förslag på möjlig vidareutveckling av projektet:
- Hårdvaruskydd
- Jämförelsetal
- Egenutvecklad webbserver
- Robustare säkerhetslösningar