#include "application.h"//needed for all Particle libraries

#ifndef Devices_h
#define Devices_h

    class PlotBotDevice {
    
        public:
            std::string     DeviceId;
            std::string     DeviceName;
            unsigned long   ZipCode;
            double          Latitude;
            double          Longitude;
            unsigned int    Elevation;
            
            unsigned int    ReportToThingSpeak;
            unsigned long   ThingSpeakChannelNumber;
            std::string     ThingSpeakWriteApiKey;
            
            unsigned int    ReportToWunderground;
            std::string     WundergroundPwsId;
            std::string     WundergroundPwsPassword;

            unsigned int    ReportToAzure;
            
            unsigned int    SleepInterval;
    };
    #define DEVICE_COUNT 5
    extern std::array<PlotBotDevice *, DEVICE_COUNT> devices;
    extern PlotBotDevice *device;
    
    void InitializeDevices();
#endif