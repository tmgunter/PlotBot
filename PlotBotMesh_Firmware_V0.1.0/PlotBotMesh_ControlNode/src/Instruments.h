#include "application.h"//needed for all Particle libraries

#ifndef Instruments_h
#define Instruments_h

    #include <math.h>
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Weather Shield Variables
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    extern double humidity;
    extern double dewptf;
    extern double dewptc;
    extern double tempf;     //Average of the sensors temperature readings, fahrenheit
    extern double tempc;     //Average of the sensors temperature readings, celsius

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Function Prototypes
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int averageAnalogRead(int analogPin);
    double dewPoint(double celsius, double humidity);
    void calcWeatherInfo();
#endif