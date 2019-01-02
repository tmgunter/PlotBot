#ifndef Devices_h
#define Devices_h

    class PlotBotDevice {
    
        public:
            int             NodeType;
            String          DeviceId;
            String          DeviceName;
            unsigned long   ZipCode;
            double          Latitude;
            double          Longitude;
            unsigned int    Elevation;
            
            unsigned int    ReportToThingSpeak;
            unsigned long   ThingSpeakChannelNumber;
            String          ThingSpeakWriteApiKey;
            
            unsigned int    ReportToWunderground;
            String          WundergroundPwsId;
            String          WundergroundPwsPassword;

            unsigned int    ReportToCloud;
            
            unsigned int    SleepInterval;

            int             Button;
    };
    
    void InitializeDevices();
#endif
