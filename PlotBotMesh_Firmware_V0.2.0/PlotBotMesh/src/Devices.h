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

            int             Button;
    };
    
    void InitializeDevices();
#endif
