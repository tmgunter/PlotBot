#ifndef PlotBotMesh_h
#define PlotBotMesh_h
    #include "application.h"
    #include "Devices.h"

    #include <math.h>
    #include <map>

    #if PLATFORM_ID == PLATFORM_ARGON
        #define SSD1306_128x32x
        #undef DHT11
        #undef TM1637Displayx
        #undef LightSensor
        #undef Chainable_LED
    #elif PLATFORM_ID == PLATFORM_XENON
        #undef SSD1306_128x32x
        #undef Chainable_LED
        #define DHT11
        #define TM1637Displayx
        #define LightSensor
    #endif

    #define FLEET_SIZE 10
    extern std::map<String, PlotBotDevice*> fleet;
    extern PlotBotDevice *device;
    extern std::map<String, String> events;

    #define BUTTON_A  4
    #define BUTTON_B  3
    #define BUTTON_C  2

    #ifdef LightSensor
        #define LightSensorPin A0
        extern int lightValue;
        extern int lightValueRaw;
    #endif

#endif
