#include "Instruments.h"
#include "ThingSpeakSend.h"

void sendInfoToThingSpeak(PlotBotDevice *device)
{
    ThingSpeak.setField(1, (float)tempf);
    ThingSpeak.setField(2, (float)humidity);
    ThingSpeak.setField(3, (float)baromin);
    ThingSpeak.setField(4, (float)lux);
    ThingSpeak.setField(5, (float)soilmoisturePercentage);
    ThingSpeak.setField(6, (float)soiltempf);
    ThingSpeak.setField(7, (float)batterySoc);
    ThingSpeak.setField(8, (float)batteryVoltage);
    ThingSpeak.writeFields(device->ThingSpeakChannelNumber, device->ThingSpeakWriteApiKey);
}

void sendConfigToThingSpeak()
{

}