#include "application.h"//needed for all Particle libraries

#ifndef Serial_h
#define Serial_h

    #include <HttpClient.h>
    #include "Devices.h"
    
    extern Weather sensor;
    
    // function prototypes
    void printBatteryInfo();
    void printWeatherInfo();
    void sendWeatherInfoToWU(PlotBotDevice *device);
#endif