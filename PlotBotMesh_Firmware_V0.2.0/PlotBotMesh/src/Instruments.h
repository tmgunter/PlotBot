#include "PlotBotMesh.h"
#include "Devices.h"
#include <map>

#ifndef Instruments_h
#define Instruments_h
    #include <math.h>
    
    #ifdef DHT11
        #include <Seeed_DHT11.h>

        extern DHT dht;
        extern float humidity;
        extern float tempf;
        extern float tempc;
        extern double dewptf;
        extern double dewptc;
    #endif

    extern float            battvolt;
    extern char             localTime[];
 
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Function Prototypes
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int averageAnalogRead(int analogPin);
    float dewPoint(float celsius, float humidity);
    void calcWeatherInfo();
    void calcBatteryInfo();
#endif