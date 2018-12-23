#include "application.h"//needed for all Particle libraries

#ifndef Devices_h
#define Devices_h

    class PlotBotDevice {
    
        public:
            char            DeviceId[26];
            char            DeviceName[50];
            unsigned long   ZipCode;
            double          Latitude;
            double          Longitude;
            unsigned int    Elevation;
            
            unsigned int    ReportToThingSpeak;
            unsigned long   ThingSpeakChannelNumber;
            char            ThingSpeakWriteApiKey[18];
            
            unsigned int    ReportToWunderground;
            char            WundergroundPwsId[12];
            char            WundergroundPwsPassword[10];

            unsigned int    ReportToAzure;
            
            unsigned int    SleepInterval;
    };
    extern PlotBotDevice *device;
    
    void InitializeDevices();
#endif
