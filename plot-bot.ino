// This #include statement was automatically added by the Particle IDE.
#include "application.h"

#include "Devices.h"
#include "Instruments.h"
#include "Serial.h"
#include "Azure.h"
#include "ThingSpeakSend.h"
#include "Spark-Dallas-Temperature.h"

PlotBotDevice *device = new PlotBotDevice();
                        
////////////////////////////////////////////////////////////////////////////////////////////////////
// GPIO Pin Usage
////////////////////////////////////////////////////////////////////////////////////////////////////

// Digital Pins
int soilTemperaturePin = D4;
int soilMoisturePowerPin = D5;
int ledActivityPin = D7;

// Analog Pins
int soilMoisturePin = A1;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Battery Variables
////////////////////////////////////////////////////////////////////////////////////////////////////
double batteryVoltage = 0; // Variable to keep track of LiPo voltage
double batterySoc = 0; // Variable to keep track of LiPo state-of-charge (SOC)
int batteryAlert; // Variable to keep track of whether alert has been triggered

////////////////////////////////////////////////////////////////////////////////////////////////////
// Soil Variables
////////////////////////////////////////////////////////////////////////////////////////////////////
//Run I2C Scanner to get address of DS18B20(s)
//(found in the Firmware folder in the Photon Weather Shield Repo)
DeviceAddress soilThermometer = { 0x28, 0xBC, 0xC9, 0x7F, 0x08, 0x00, 0x00, 0xA2 }; //Waterproof temp sensor address

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature soilTempSensor(&oneWire);

double soiltempf = 0.0;
double soiltempc = 0.0;
double soilmoisture = 0.0;
double soilreading = 0.0;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Weather Shield Variables
////////////////////////////////////////////////////////////////////////////////////////////////////
//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barometric sensor
Weather sensor;

double humidity = 0;
double tempf = 0;     //Average of the sensors temperature readings, fahrenheit
double tempc = 0;     //Average of the sensors temperature readings, celsius
double dewptf = 0;
double dewptc = 0;
double pascals = 0;
double baromin = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Luminosity Variables
////////////////////////////////////////////////////////////////////////////////////////////////////
// instanciate a TSL2561 object with I2C address 0x39
TSL2561 tsl(TSL2561_ADDR);

// sensor related vars
uint16_t integrationTime;
double lux;
uint32_t lux_int;
bool autoGainOn;

// execution control var
bool operational;

//status vars
char tsl_status[21] = "na";
char autoGain_s[4] = "na";
uint8_t error_code;
uint8_t gain_setting;

////////////////////////////////////////////////////////////////////////////////////////////////////
// ThingSpeak  Variables
////////////////////////////////////////////////////////////////////////////////////////////////////
TCPClient thingSpeakClient;

//---------------------------------------------------------------
void setup()
{
    STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));
    
    // set wifi antenna to external
    WiFi.selectAntenna(ANT_EXTERNAL);
    
    while(Time.year() <= 1970)
    {
        Particle.syncTime();  
        Particle.process();
        delay(100);
    }
    Time.zone(-7);
    
    Serial.begin(57600);   // open serial over USB at 9600 baud
    
    // Find our device
    delay(5000);
    InitializeDevices();
    const char *deviceId = System.deviceID().c_str();
    for (int i = 1; i <= DEVICE_COUNT; i++)
    {
        if(strcmp(deviceId, devices[i]->DeviceId.c_str()) == 0)
        {
            device = new PlotBotDevice();
            device->DeviceId                = devices[i]->DeviceId;
            device->DeviceName              = devices[i]->DeviceName;
            device->ZipCode                 = devices[i]->ZipCode;
            device->Latitude                = devices[i]->Latitude;
            device->Longitude               = devices[i]->Longitude;
            device->Elevation               = devices[i]->Elevation;
            device->ReportToThingSpeak      = devices[i]->ReportToThingSpeak;
            device->ThingSpeakChannelNumber = devices[i]->ThingSpeakChannelNumber;
            device->ThingSpeakWriteApiKey   = devices[i]->ThingSpeakWriteApiKey;
            device->ReportToWunderground    = devices[i]->ReportToWunderground;
            device->WundergroundPwsId       = devices[i]->WundergroundPwsId;
            device->WundergroundPwsPassword = devices[i]->WundergroundPwsPassword;
            device->ReportToAzure           = devices[i]->ReportToAzure;
            device->SleepInterval           = devices[i]->SleepInterval;
            break;
        }
    }

    // Make sure your Serial Terminal app is closed before powering your device
    // Now open your Serial Terminal, and hit any key to continue!
    //Serial.println("Press any key to begin");
    //This line pauses the Serial port until a key is pressed
    //while(!Serial.available()) 
    //    Particle.process();
    
    pinMode(ledActivityPin, OUTPUT);
        
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Battery Setup
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set up Spark variables (voltage, soc, and alert):
	Particle.variable("voltage", &batteryVoltage, DOUBLE);
	Particle.variable("soc", &batterySoc, DOUBLE);
	Particle.variable("alert", &batteryAlert, INT);
	
	// Set up the MAX17043 LiPo fuel gauge:
	lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge

	// Quick start restarts the MAX17043 in hopes of getting a more accurate  guess for the SOC.
	lipo.quickStart();

	// We can set an interrupt to alert when the battery SoC gets too low.
	// We can alert at anywhere between 1% - 32%:
	lipo.setThreshold(20); // Set alert threshold to 20%. 
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Luminosity Setup
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	// Soil Setup
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    Particle.variable("soilmoist", &soilmoisture, DOUBLE);
    Particle.variable("soiltempf", &soiltempf, DOUBLE);
    
    pinMode(soilTemperaturePin, INPUT);
    pinMode(soilMoisturePowerPin, OUTPUT);
    digitalWrite(soilMoisturePowerPin, LOW);
    pinMode(soilMoisturePin, INPUT);
    
    // DS18B20 initialization
    soilTempSensor.begin();
    soilTempSensor.setResolution(soilThermometer, TEMPERATURE_PRECISION);
    soilreading = getSoilMoisture();       // give ourselves an initial value
    soilmoisture = (-0.1696 + (0.000212 * soilreading)) * 100;
    soiltempc = getSoilTemp();
    soiltempf = (soiltempc * 9)/5 + 32;//else grab the newest, good data

    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Weather Shield Setup
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Create Particle.variables for each piece of data for easy access
    Particle.variable("humidity", &humidity, DOUBLE);
    Particle.variable("tempf", &tempf, DOUBLE);
    Particle.variable("baromin", &baromin, DOUBLE);
    Particle.variable("dewptf", &dewptf, DOUBLE);
    
    //Initialize the I2C sensors and ping them
    sensor.begin();

    /*You can only receive accurate barometric readings or accurate altitude
    readings at a given time, not both at the same time. The following two lines
    tell the sensor what mode to use. You could easily write a function that
    takes a reading in one made and then switches to the other mode to grab that
    reading, resulting in data that contains both accurate altitude and barometric
    readings. For this example, we will only be using the barometer mode. Be sure
    to only uncomment one line at a time. */
    sensor.setModeBarometer();//Set to Barometer Mode
    //sensor.setModeAltimeter();//Set to altimeter Mode

    //These are additional MPL3115A2 functions that MUST be called for the sensor to work.
    sensor.setOversampleRate(7); // Set Oversample rate
    //Call with a rate from 0 to 7. See page 33 for table of ratios.
    //Sets the over sample rate. Datasheet calls for 128 but you can set it
    //from 1 to 128 samples. The higher the oversample rate the greater
    //the time between data samples.

    sensor.enableEventFlags(); //Necessary register calls to enble temp, baro and alt
	
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Luminosity Setup
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    Particle.variable("tsl_status", tsl_status);
    Particle.variable("intTime", integrationTime);
    Particle.variable("gain", gain_setting);
    Particle.variable("autoGainOn", autoGain_s);
    Particle.variable("lux", lux);
    Particle.variable("lux_int", lux_int);
    
    //connecting to light sensor device
    if (tsl.begin()) 
        strcpy(tsl_status,"tsl2561 found");
    else
        strcpy(tsl_status,"tsl 2561 not found ");
    Serial.println(tsl_status);
    
    // setting the sensor: gain x1 and 101ms integration time
    if(!tsl.setTiming(false, 1, integrationTime))
    {
        error_code = tsl.getError();
        sprintf(tsl_status, "setTimingError: %d", error_code);
        Serial.println(tsl_status);
    }
    
    if (!tsl.setPowerUp())
    {
        error_code = tsl.getError();
        sprintf(tsl_status, "PowerUPError: %d", error_code);
        Serial.println(tsl_status);
    }
    
    // device initialized
    operational = true;
    strcpy(tsl_status,"initOK");
    Serial.println(tsl_status);
	
	// ThingSpeak setup
	ThingSpeak.begin(thingSpeakClient);
}

//---------------------------------------------------------------
void loop()
{ 
    digitalWrite(ledActivityPin, HIGH);
	delay(750);
	digitalWrite(ledActivityPin, LOW);
	
	int startMillis = millis();
    calcWeatherInfo();
    getBatteryInfo();
    Serial.printlnf("%s: Elapsed collection time: %ldMS", Time.timeStr().c_str(), millis() - startMillis);
    
    bool wifiReady = WiFi.ready();
    bool cloudReady = Particle.connected();

    Serial.printf("%s: ", Time.timeStr().c_str());
    Serial.printf(" *** cloudReady: %d", cloudReady);
    Serial.printlnf(" *** wifiReady: %d", wifiReady);

    Serial.printf("Id: %s, ", device->DeviceId.c_str());
    Serial.printf("Name: %s, ", device->DeviceName.c_str());
    Serial.printf("ZipCode: %ld, ", device->ZipCode);
    Serial.printf("Latitude: %lf, ", device->Latitude);
    Serial.printf("Longitude %lf, ", device->Longitude);
    Serial.printlnf("Elevation: %ld", device->Elevation);
    
    Serial.printf("ReportToThingSpeak: %ld, ", device->ReportToThingSpeak);
    Serial.printf("ThingSpeakChannelNumber: %lu, ", device->ThingSpeakChannelNumber);
    Serial.printlnf("ThingSpeakWriteApiKey: %s, ", device->ThingSpeakWriteApiKey.c_str());
    
    Serial.printf("ReportToWunderGround: %ld, ", device->ReportToWunderground);
    Serial.printf("WunderGroundPwsiD: %s, ", device->WundergroundPwsId.c_str());
    Serial.printlnf("WunderGroundPassword: %s", device->WundergroundPwsPassword.c_str());

    Serial.printlnf("ReportToAzure: %ld", device->ReportToAzure);
    
    Serial.printlnf("SleepInterval: %ld", device->SleepInterval);
    
    if (device->ReportToWunderground == 1)
    {
        if (wifiReady && cloudReady && strcmp("", device->WundergroundPwsId.c_str()) != 0)
        {
            sendWeatherInfoToWU(device);
            Serial.println("Reported to Wunderground");
        }
    }
    
    if (device->ReportToAzure == 1)
    {
        if (wifiReady && cloudReady)
        {
            //sendConfigToAzure();
            //sendWeatherInfoToAzure();
            //sendBatteryInfoToAzure();
        }
    }
        			
    if (device->ReportToThingSpeak == 1)
    {
        if (wifiReady && cloudReady && device->ThingSpeakChannelNumber > 0)
        {
            sendInfoToThingSpeak(device);
            Serial.println("Reported to ThingSpeak");
        }
    }
    
    printBatteryInfo();
    printWeatherInfo();
    
    Serial.printlnf("%s: Elapsed processingg time: %ldMS", Time.timeStr().c_str(), millis() - startMillis);

    if (device->SleepInterval > 0)
    {
        Serial.printlnf("%s: calling System.sleep(SLEEP_MODE_DEEP, %ld);", Time.timeStr().c_str(), device->SleepInterval);
        delay(2);
        System.sleep(SLEEP_MODE_DEEP, device->SleepInterval);
        Serial.printlnf("%s: sleeping...", Time.timeStr().c_str());
    }
    
    Serial.begin(57600);
}
//---------------------------------------------------------------


