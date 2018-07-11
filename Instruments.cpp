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
void update18B20Temp(DeviceAddress deviceAddress, double &tempC)
{
  tempC = soilTempSensor.getTempC(deviceAddress);
}
//---------------------------------------------------------------
double getSoilTemp()
{
    double temp;
    //get temp from DS18B20
    soilTempSensor.requestTemperatures();
    update18B20Temp(soilThermometer, temp);
    //Every so often there is an error that throws a -127.00, this compensates
    if(temp < -100)
      return(soiltempc);//push last value so data isn't out of scope
    else
        return(temp);
}
//---------------------------------------------------------------
double getSoilMoisture()
{
    /*We found through testing that leaving the soil moisture sensor powered
    all the time lead to corrosion of the probes. Thus, this port breaks out
    Digital Pin D5 as the power pin for the sensor, allowing the Photon to
    power the sensor, take a reading, and then disable power on the sensor,
    giving the sensor a longer lifespan.*/
    digitalWrite(soilMoisturePowerPin, HIGH);
    delay(200);
    double avgMoisture = 0.0;
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

void getLux()
{
    uint16_t broadband, ir;

    // update exposure settings display vars
    if (tsl._gain)
        gain_setting = 16;
    else
        gain_setting = 1;

    if (autoGainOn)
        strcpy(autoGain_s, "yes");
    else
        strcpy(autoGain_s, "no");

    if (operational)
    {
        // device operational, update status vars
        strcpy(tsl_status, "OK");

        // get raw data from sensor
        if(!tsl.getData(broadband,ir,autoGainOn))
        {
            error_code = tsl.getError();
            sprintf(tsl_status, "saturated? %d", error_code);
            operational = false;
        }
    
        // compute illuminance value in lux
        if(!tsl.getLux(integrationTime, broadband, ir, lux))
        {
            error_code = tsl.getError();
            sprintf(tsl_status, "getLuxError: %d", error_code);
            operational = false;
        }
    
        // try the integer based calculation
        if(!tsl.getLuxInt(broadband, ir, lux_int))
        {
            error_code = tsl.getError();
            sprintf(tsl_status, "getLuxIntError: %d", error_code);
            operational = false;
        }

    }
    else
    // device not set correctly
    {
        strcpy(tsl_status,"OperationError");
        lux = -1.0;
        // trying a fix
        // power down the sensor
        tsl.setPowerDown();
        delay(100);
        // re-init the sensor
        if (tsl.begin())
        {
            // power up
            tsl.setPowerUp();
            // re-configure
            tsl.setTiming(tsl._gain, 1, integrationTime);
            // try to go back normal again
            operational = true;
        } 
    }
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
    //soiltempc = getSoilTemp();
    //soiltempf = (soiltempc * 9)/5 + 32;
    
    // Soil Moisture
    //soilmoisture = (-0.1696 + (0.000212 * soilreading)) * 100;
    
    // Wind Speed
    //float tempWindspeedmph = getWindSpeed();
	//if (tempWindspeedmph >= 0 && tempWindspeedmph <= 75)
	//    windspeedmph = tempWindspeedmph;

    // Wind DIrection
    //captureWindVane();
    //float tempWinddir = getAndResetWindVaneDegrees();
    //int tempWinddir = getWindDirection();
    //if (tempWinddir != -1)
    //    winddir = tempWinddir;
    
	//current winddir, current windspeed, windgustmph, and windgustdir are calculated every 100ms throughout the day

	//Calc windspdmph_avg2m
	//float temp = 0;
	//for(int i = 0 ; i < 120 ; i++)
	//	temp += windspdavg[i];
	//temp /= 120.0;
	//windspdmph_avg2m = temp;

	//Calc winddir_avg2m, Wind Direction
	//You can't just take the average. Google "mean of circular quantities" for more info
	//We will use the Mitsuta method because it doesn't require trig functions
	//And because it sounds cool.
	//Based on: http://abelian.org/vlf/bearings.html
	//Based on: http://stackoverflow.com/questions/1813483/averaging-angles-again
	//long sum = winddiravg[0];
	//int D = winddiravg[0];
	//for(int i = 1 ; i < WIND_DIR_AVG_SIZE ; i++)
	//{
	//	int delta = winddiravg[i] - D;

	//	if(delta < -180)
	//		D += delta + 360;
	//	else if(delta > 180)
	//		D += delta - 360;
	//	else
	//		D += delta;

	//	sum += D;
	//}
	//winddir_avg2m = sum / WIND_DIR_AVG_SIZE;
	//if(winddir_avg2m >= 360) winddir_avg2m -= 360;
	//if(winddir_avg2m < 0) winddir_avg2m += 360;


	//Calc windgustmph_10m
	//Calc windgustdir_10m
	//Find the largest windgust in the last 10 minutes
	//windgustmph_10m = 0;
	//windgustdir_10m = 0;
	////Step through the 10 minutes
	//for(int i = 0; i < 10 ; i++)
	//{
	//	if(windgust_10m[i] > windgustmph_10m)
	//	{
	//		windgustmph_10m = windgust_10m[i];
	//		windgustdir_10m = windgustdirection_10m[i];
	//	}
	//}


	//Total rainfall for the day is calculated within the interrupt
	//Calculate amount of rainfall for the last 60 minutes
	//rainin = 0;
	//for(int i = 0 ; i < 60 ; i++)
	//	rainin += rainHour[i];
	//dailyraininVar = dailyrainin;


	//Calc light level
	getLux();

	//Calc battery level
	//batt_lvl = get_battery_level();
}