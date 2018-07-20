#include "application.h"

#include "Devices.h"
#include "Instruments.h"
#include "Serial.h"
#include "Azure.h"
#include "Sparkfun_APDS9301.h"
#include "ThingSpeakSend.h"

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
const int      MAXRETRY          = 4;

DS18B20 ds18b20(soilTemperaturePin, true);

double soiltempf = 0.0;
double soiltempc = 0.0;
int soilmoisture = 0;
int soilmoisturePercentage = 0;
//double soilreading = 0.0;

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
double lux;

APDS9301 apds;
int CH0Level;
int CH1Level;

////////////////////////////////////////////////////////////////////////////////////////////////////
// ThingSpeak  Variables
////////////////////////////////////////////////////////////////////////////////////////////////////
TCPClient thingSpeakClient;

int collectionTime;

//---------------------------------------------------------------
void setup()
{
    STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

    // set wifi antenna to external
    //WiFi.selectAntenna(ANT_EXTERNAL);

    while(Time.year() <= 1970)
    {
        Particle.syncTime();
        Particle.process();
        delay(100);
    }
    Time.zone(-7);

    Serial.begin(9600);   // open serial over USB at 9600 baud

    // Find our device
    delay(5000);
    InitializeDevices();

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
    Particle.variable("soilmoist", &soilmoisturePercentage, INT);
    Particle.variable("soiltempf", &soiltempf, DOUBLE);

    pinMode(soilTemperaturePin, INPUT);
    pinMode(soilMoisturePowerPin, OUTPUT);
    digitalWrite(soilMoisturePowerPin, LOW);
    pinMode(soilMoisturePin, INPUT);

    // DS18B20 initialization
    //soilTempSensor.begin();
    //4bsoilTempSensor.setResolution(soilThermometer, TEMPERATURE_PRECISION);
    soilmoisture = getSoilMoisture();       // give ourselves an initial value
    getSoilTemp();

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
    Wire.begin();

    Serial.println("APDS-3901 found");

    // APDS9301 sensor setup.
    apds.begin(0x39);   // We're assuming you haven't changed the I2C address from the default by soldering the jumper on the back of the board.
    apds.setGain(APDS9301::HIGH_GAIN); // Set the gain to high. 16X more sensitive than LOW_GAIN
    apds.setIntegrationTime(APDS9301::INT_TIME_402_MS); // Set the integration time to the highest (of three) interval.
    apds.setLowThreshold(0); // Sets the low threshold to 0, effectively disabling the low side interrupt.
    apds.setHighThreshold(65535 ); // Sets the high threshold to 65535.  A value of 65535  will disable the interrupt
    apds.setCyclesForInterrupt(1); // A single reading in the threshold range will cause an interrupt to trigger.
    apds.enableInterrupt(APDS9301::INT_OFF); // Disable the interrupt.
    apds.clearIntFlag();
    CH0Level = apds.readCH0Level();
    CH1Level = apds.readCH1Level();
    lux = apds.readLuxLevel();

    Particle.variable("lux", &lux, DOUBLE);

	// ThingSpeak setup
	ThingSpeak.begin(thingSpeakClient);
}

//---------------------------------------------------------------
void loop()
{
    digitalWrite(ledActivityPin, HIGH);
	delay(750);
	digitalWrite(ledActivityPin, LOW);

    Serial.printlnf("");

	int startMillis = millis();
    calcWeatherInfo();
    getBatteryInfo();
    collectionTime =  millis() - startMillis;
    Serial.printlnf("%s: Elapsed collection time: %ldMS", Time.timeStr().c_str(), collectionTime);
    printBatteryInfo();
    printWeatherInfo();

    bool wifiReady = WiFi.ready();
    bool cloudReady = Particle.connected();

    Serial.printf("\tcloudReady: %d, ", cloudReady);
    Serial.printlnf("wifiReady: %d", wifiReady);

    Serial.println("");
    Serial.println("\n*** Config Data:");
    Serial.printf("\tDeviceId: %s, ", device->DeviceId);
    Serial.printf("Name: %s, ", device->DeviceName);
    Serial.printf("ZipCode: %ld, ", device->ZipCode);
    Serial.printf("Latitude: %lf, ", device->Latitude);
    Serial.printf("Longitude %lf, ", device->Longitude);
    Serial.printlnf("Elevation: %ld", device->Elevation);

    Serial.printf("\tReportToThingSpeak: %ld, ", device->ReportToThingSpeak);
    Serial.printf("ThingSpeakChannelNumber: %lu, ", device->ThingSpeakChannelNumber);
    Serial.printlnf("ThingSpeakWriteApiKey: %s, ", device->ThingSpeakWriteApiKey);

    Serial.printf("\tReportToWunderGround: %ld, ", device->ReportToWunderground);
    Serial.printf("WunderGroundPwsiD: %s, ", device->WundergroundPwsId);
    Serial.printlnf("WunderGroundPassword: %s", device->WundergroundPwsPassword);

    Serial.printlnf("\tReportToAzure: %ld", device->ReportToAzure);

    Serial.printlnf("\tSleepInterval: %ld", device->SleepInterval);

    if (device->ReportToWunderground)
    {
        if (wifiReady && cloudReady && strcmp("", device->WundergroundPwsId) != 0)
        {
            sendWeatherInfoToWU(device);
            Serial.println("\n\tReported to Wunderground");
        }
    }

    if (device->ReportToAzure)
    {
        if (wifiReady && cloudReady)
        {
            sendConfigToAzure();
            sendInfoToAzure();
            Serial.println("\n\tReported to Azure");
        }
    }

    if (device->ReportToThingSpeak)
    {
        if (wifiReady && cloudReady && device->ThingSpeakChannelNumber > 0)
        {
            sendInfoToThingSpeak(device);
            Serial.println("\n\tReported to ThingSpeak");
        }
    }

    Serial.printlnf("%s: Elapsed processing time: %ldMS", Time.timeStr().c_str(), millis() - startMillis);

    if (device->SleepInterval > 0)
    {
        Serial.printlnf("%s: calling System.sleep(SLEEP_MODE_DEEP, %ld);", Time.timeStr().c_str(), device->SleepInterval);
        delay(2);
        System.sleep(SLEEP_MODE_DEEP, device->SleepInterval);
        Serial.printlnf("%s: sleeping...", Time.timeStr().c_str());
    }
    else
        delay(5000);

    Serial.begin(9600);
}
//---------------------------------------------------------------
