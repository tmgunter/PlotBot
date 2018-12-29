    #include "application.h"

    #if PLATFORM_ID == PLATFORM_ARGON
        #define SSD1306_128x32x
        #undef DHT11
        #undef TM1637Displayx
        #undef CHAINABLE_LED
    #elif PLATFORM_ID == PLATFORM_XENON
        #undef SSD1306_128x32x
        #define DHT11
        #define TM1637Displayx
        #undef CHAINABLE_LED
    #endif

