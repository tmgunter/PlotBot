#include "application.h"
#include "PlotBotMesh.h"

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

//Takes average of readings on a given pin
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

//Calculates each of the variables that wunderground is expecting
void calcWeatherInfo()
{
	#ifdef DHT11
		humidity = dht.getHumidity();
		tempc = dht.getTempCelcius();
		tempf = dht.getTempFarenheit();

		dewptc = dewPoint((double)tempc, (double)humidity);
		dewptf = (dewptc * 9.0)/ 5.0 + 32.0;
	#endif
}

//Calculates battery info
void calcBatteryInfo()
{
	battvolt = (float)((float)averageAnalogRead(BATT) / 1000.0);
}

#ifdef LightSensor
	void calcLightInfo()
	{
		lightValueRaw = averageAnalogRead(LightSensorPin);
		lightValue = map(lightValueRaw, 0, 1000, 0, 8);
	}
#endif