#include "Instruments.h""
#include "ThingSpeakSend.h"

void sendBatteryInfoToThingSpeak()
{

}

void sendInfoToThingSpeak(PlotBotDevice *device)
{
    //ThingSpeak.setField(1, device->DeviceId.c_str());
    //ThingSpeak.setField(2, (int)device->ZipCode);
    ThingSpeak.setField(1, (float)tempf);
    ThingSpeak.setField(2, (float)humidity);
    ThingSpeak.setField(3, (float)baromin);
    ThingSpeak.setField(4, (float)lux_int);
    ThingSpeak.setField(5, (float)soilmoisture);
    ThingSpeak.setField(6, (float)soiltempf);
    ThingSpeak.setField(7, (float)batterySoc);
    ThingSpeak.setField(8, (float)batteryVoltage);
    ThingSpeak.writeFields(device->ThingSpeakChannelNumber, device->ThingSpeakWriteApiKey.c_str());

    Serial.printf("Id: %s, ", device->DeviceId.c_str());
    Serial.printf("ThingSpeakChannelNumber: %ld, ", device->ThingSpeakChannelNumber);
    Serial.printlnf("ThingSpeakWriteApiKey: %s", device->ThingSpeakWriteApiKey.c_str());
}

void sendConfigToThingSpeak()
{

}