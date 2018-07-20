#include "Instruments.h"

//---------------------------------------------------------------
//---------------------------------------------------------------
// dewPoint function from NOAA
// reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
// reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
//---------------------------------------------------------------
double dewPoint(double celsius, double humidity)
{
	// (1) Saturation Vapor Pressure = ESGG(T)
	double RATIO = 373.15 / (273.15 + celsius);
	double RHS = -7.90298 * (RATIO - 1);
	RHS += 5.02808 * log10(RATIO);
	RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
	RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
	RHS += log10(1013.246);

  // factor -3 is to adjust units - Vapor Pressure SVP * humidity
	double VP = pow(10, RHS - 3) * humidity;

  // (2) DEWPOINT = F(Vapor Pressure)
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558 - T);
}

//---------------------------------------------------------------
void getSoilTemp()
{
    float _temp;
    int   i = 0;

    do
    {
        _temp = ds18b20.getTemperature();
    } while (!ds18b20.crcCheck() && MAXRETRY > i++);

    if (i < MAXRETRY) {
        soiltempc = _temp;
        soiltempf = ds18b20.convertToFahrenheit(_temp);
    }
    else
    {
        soiltempc = soiltempf = NAN;
        Serial.println("Invalid soil temperature reading");
    }
}
//---------------------------------------------------------------

int getSoilMoisture()
{
    /*We found through testing that leaving the soil moisture sensor powered
    all the time lead to corrosion of the probes. Thus, this port breaks out
    Digital Pin D5 as the power pin for the sensor, allowing the Photon to
    power the sensor, take a reading, and then disable power on the sensor,
    giving the sensor a longer lifespan.*/
    digitalWrite(soilMoisturePowerPin, HIGH);
    delay(100);
    int avgMoisture = 0;
    for (int i = 0; i < 8; i++)
        avgMoisture += analogRead(soilMoisturePin);
    delay(100);
    digitalWrite(soilMoisturePowerPin, LOW);
    return(avgMoisture / 8);
}

//Takes ananalogRawerage of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
	byte numberOfReadings = 8;
	unsigned int runningValue = 0;

	for(int x = 0 ; x < numberOfReadings ; x++)
		runningValue += analogRead(pinToRead);
	runningValue /= numberOfReadings;

	return(runningValue);
}

void getBatteryInfo()
{
    // lipo.getVoltage() returns a voltage value (e.g. 3.93)
	batteryVoltage = lipo.getVoltage();
	// lipo.getSOC() returns the estimated state of charge (e.g. 79%)
	batterySoc = lipo.getSOC();
	// lipo.getAlert() returns a 0 or 1 (0=alert not triggered)
	batteryAlert = (int)lipo.getAlert();
}

//Calculates each of the variables that wunderground is expecting
void calcWeatherInfo()
{
    // Measure Relative Humidity from the HTU21D or Si7021
    humidity = sensor.getRH();

    // Measure Temperature from the HTU21D or Si7021
    double humtempc = sensor.getTemp();
    double humtempf = (humtempc * 9)/5 + 32;
    // Temperature is measured every time RH is requested.
    // It is faster, therefore, to read it from previous RH
    // measurement with getTemp() instead with readTemp()

    //Measure the Barometer temperature in F from the MPL3115A2
    double barotempc = sensor.readBaroTemp();
    double barotempf = (barotempc * 9)/5 + 32; //convert the temperature to F

    //Measure Pressure from the MPL3115A2
    pascals = sensor.readPressure();
    baromin = pascals * 0.0002953; // Calc for converting Pa to inHg (Wunderground expects inHg)

    //If in altitude mode, you can get a reading in feet with this line:
    //float altf = sensor.readAltitudeFt();

    //Average the temperature reading from both sensors
    tempc = ((humtempc + barotempc)/2);
    tempf = ((humtempf + barotempf)/2);

    //Calculate Dew Point
    dewptc = dewPoint(tempc, humidity);
    dewptf = (dewptc * 9.0)/ 5.0 + 32.0;

    // Soil Temperature
    getSoilTemp();

    // Soil Moisture
    soilmoisture = getSoilMoisture();
    soilmoisturePercentage = map(soilmoisture, 3100, 3320, 0, 100);

    lux = getLux();
 }

double getLux()
{
    // Luminosity
    apds.clearIntFlag();
    CH0Level = apds.readCH0Level();
    CH1Level = apds.readCH1Level();
    lux = apds.readLuxLevel();
}
