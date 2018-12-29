#ifndef Serial_h
#define Serial_h

    #include <HttpClient.h>
    
    // function prototypes
    void printWeatherInfo();
    void sendWeatherInfoToWU(PlotBotDevice *device);

    void printBatteryInfo();
#endif