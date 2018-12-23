#include "application.h"//needed for all Particle libraries

#ifndef Serial_h
#define Serial_h

    #include "Devices.h"
    
    // function prototypes
    void printWeatherInfo();
    void sendWeatherInfoToWU(PlotBotDevice *device);
#endif