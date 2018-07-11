#include "Instruments.h"
#include "Serial.h"
#include "Devices.h"

/******************************************************************************
  SparkFun Photon Weather Shield basic example
  Joel Bartlett @ SparkFun Electronics
  Original Creation Date: May 18, 2015
  Updated August 21, 2015
  This sketch prints the temperature, humidity, and barometric pressure OR
  altitude to the Serial port.

  The library used in this example can be found here:
  https://github.com/sparkfun/SparkFun_Photon_Weather_Shield_Particle_Library

  Development environment specifics:
  	IDE: Particle Dev
  	Hardware Platform: Particle Photon
                       Particle Core

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
*******************************************************************************/
HttpClient http;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //{ "Content-Type", "application/json" },
    { "User-agent", "Particle HttpClient"},
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_request_t request;
http_response_t response;

char Disp[] = "Captain_Penguin";
char Locn[] = "Garden";

//Wunderground Vars
TCPClient client;

char WEATHERSERVER[] = "rtupdate.wunderground.com";        //Rapidfire update server - for multiple sends per minute
//char SERVER [] = "weatherstation.wunderground.com";   //Standard server - for sends once per minute or less
char WEBPAGE [] = "/weatherstation/updateweatherstation.php";
//char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";

//---------------------------------------------------------------
void sendWeatherInfoToWU(PlotBotDevice *device)
{
    
    Serial.print("Connecting to ");
    Serial.print(WEATHERSERVER);
    Serial.print("... ");

    if (client.connect(WEATHERSERVER, 80)) 
    {
        Serial.println("connected. ");
        
        char URL[512];
        
        // Request path and body can be set at runtime or at setup.
        request.hostname = WEATHERSERVER;
        request.port = 80;
        request.path = WEBPAGE;
        
        sprintf(URL, "%s?ID=%s&PASSWORD=%s&action=updateraw&dateutc=now&realtime=1&rtfreq=6", WEBPAGE, device->WundergroundPwsId.c_str(), device->WundergroundPwsPassword.c_str());
        sprintf(URL, "%s&humidity=%.2f", URL, humidity);
        sprintf(URL, "%s&dewptf=%.2f", URL, dewptf);
        sprintf(URL, "%s&tempf=%.2f", URL, tempf);
        sprintf(URL, "%s&baromin=%.2f", URL, baromin);
        sprintf(URL, "%s&soiltempf=%.2f", URL, soiltempf);
        sprintf(URL, "%s&soilmoisture=%.2f", URL, soilmoisture);

        request.path = URL;
        request.body = "";
        
        Serial.println(URL);

        // Get request
        http.get(request, response, headers);
            
        delay(2000);
        Serial.print("Response status: ");
        Serial.println(response.status);
    
        Serial.print("HTTP Response Body: ");
        Serial.println(response.body);
    }
    else
    {
        Serial.println(F("Connection failed"));
    }
}

void printBatteryInfo()
{
 	// Those variables will update to the Spark Cloud, but we'll also print them
	// locally over serial for debugging:
	Serial.printf("voltage: %.2fV, ", batteryVoltage);  // Print the battery voltage
	Serial.printf("alert: %d, ", batteryAlert);
	Serial.printlnf("soc: %.2f%%", batterySoc); // Print the battery state of charge
}

void printWeatherInfo()
{
    //This function prints the weather data out to the default Serial Port

    Serial.printf("tempf: %.2fF, ", tempf);
    Serial.printf("dewptf: %.2fF, ", dewptf);
    Serial.printf("humidity: %.2f%%, ", humidity);
 
    //The MPL3115A2 outputs the pressure in Pascals. However, most weather stations
    //report pressure in hectopascals or millibars. Divide by 100 to get a reading
    //more closely resembling what online weather reports may say in hPa or mb.
    //Another common unit for pressure is Inches of Mercury (in.Hg). To convert
    //from mb to in.Hg, use the following formula. P(inHg) = 0.0295300 * P(mb)
    //More info on conversion can be found here:
    //www.srh.noaa.gov/images/epz/wxcalc/pressureConversion.pdf
    Serial.printf("pressure: %.2fhPa, ", pascals/100);
    Serial.printlnf("%.2fin.Hg", (pascals/100) * 0.0295300);

    //If in altitude mode, print with these lines
    //Serial.print("Altitude:");
    //Serial.print(altf);
    //Serial.println("ft.");
    
    Serial.printf("soiltempf: %.2fF, ", soiltempf);
    Serial.printf("soilreading: %.2f?, ", soilreading);
    Serial.printlnf("soilmoisture: %.2f%%", soilmoisture);
    
    Serial.printf("tsl_status: %s, ", tsl_status);
    Serial.printf("gain: %d, ", gain_setting);
    Serial.printf("autoGainOn: %s, ", autoGain_s);
    Serial.printf("integrationtime: %dms, ", integrationTime);
    Serial.printlnf("lux: %d", lux_int);
}