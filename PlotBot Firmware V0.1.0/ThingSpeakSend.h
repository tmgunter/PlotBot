#include "application.h"//needed for all Particle libraries

#ifndef ThingSpeakSend_h
#define ThingSpeakSend_h

    #include <ThingSpeak.h>
    #include <HttpClient.h>
    #include "Devices.h"

    // ThingSpeak variables
	extern TCPClient thingSpeakclient;

    void sendBatteryInfoToThingSpeak();
    void sendInfoToThingSpeak(PlotBotDevice *device);
    void sendConfigToThingSpeak();
#endif