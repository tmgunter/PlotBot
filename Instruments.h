#include "application.h"//needed for all Particle libraries

#ifndef Instruments_h
#define Instruments_h

    #include <math.h>
    #include <OneWire.h>
    #include <SparkFun_Photon_Weather_Shield_Library.h>
    #include <SparkFunMAX17043.h>
    #include "Spark-Dallas-Temperature.h"
    #include <tsl2561.h>
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // GPIO variables
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    extern int soilTemperaturePin;
    extern int soilMoisturePowerPin;
    extern int soilMoisturePin;
    
    extern bool WUNDERGROUND;
    extern bool AZURE;
	extern bool THINGSPEAK;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Soil Variables
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #define ONE_WIRE_BUS D4
    #define TEMPERATURE_PRECISION 11
    
    extern OneWire oneWire;
    extern DallasTemperature soilTempSensor;

    extern DeviceAddress soilThermometer;
    
    extern double soiltempf;
    extern double soiltempc;
    extern double soilmoisture;
    extern double soilreading;
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Weather Shield Variables
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    extern Weather sensor;
    
    extern double humidity;
    extern double dewptf;
    extern double dewptc;
    extern double tempf;     //Average of the sensors temperature readings, fahrenheit
    extern double tempc;     //Average of the sensors temperature readings, celsius
    extern double pascals;
    extern double baromin;
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Luminosity Variables
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    extern TSL2561 tsl;
    
    extern uint16_t integrationTime;
    extern double lux;
    extern uint32_t lux_int;
    extern bool autoGainOn;
    
    extern bool operational;

    //status vars
    extern char tsl_status[21];
    extern char autoGain_s[4];
    extern uint8_t error_code;
    extern uint8_t gain_setting;
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Battery Variables
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    extern double batteryVoltage;
    extern double batterySoc;
    extern int batteryAlert;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Function Prototypes
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void update18B20Temp(DeviceAddress deviceAddress, double &tempC);
    double getSoilTemp();
    double getSoilMoisture();
    int averageAnalogRead(int analogPin);
    void getBatteryInfo();
    double dewPoint(double celsius, double humidity);
    void calcWeatherInfo();
#endif