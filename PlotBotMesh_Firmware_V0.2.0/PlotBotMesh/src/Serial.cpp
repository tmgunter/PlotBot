#include "application.h"
#include "PlotBotMesh.h"

#include "Devices.h"
#include "Instruments.h"
#include "Serial.h"
#include <HttpClient.h>

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

//Wunderground Vars
TCPClient client;

char WEATHERSERVER[] = "rtupdate.wunderground.com";        //Rapidfire update server - for multiple sends per minute
//char SERVER [] = "weatherstation.wunderground.com";   //Standard server - for sends once per minute or less
char WEBPAGE [] = "/weatherstation/updateweatherstation.php";
//char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";

//---------------------------------------------------------------
void sendWeatherInfoToWU(PlotBotDevice *device)
{
    return;
    
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
    #ifdef DHT11
        sprintf(URL, "%s&humidity=%.2f", URL, humidity);
        sprintf(URL, "%s&dewptf=%.2f", URL, dewptf);
        sprintf(URL, "%s&tempf=%.2f", URL, tempf);
    #endif

        request.path = URL;
        request.body = "";
        
        Serial.printlnf("\t%s", URL);

        // Get request
        http.get(request, response, headers);
            
        delay(2000);
        Serial.print("\tResponse status: ");
        Serial.println(response.status);
    
        Serial.print("\tHTTP Response Body: ");
        Serial.println(response.body);
    }
    else
    {
        Serial.println(F("\tConnection failed"));
    }
}

void printWeatherInfo()
{
    #ifdef DHT11
        //This function prints the weather data out to the default Serial Port
        Serial.println("\n*** Sensor Data:");
        if ((int)tempf != 32 && (int)humidity != 0)
        {
            Serial.printf("\ttempf: %.2fF, ", tempf);
            Serial.printf("dewptf: %.2fF, ", dewptf);
            Serial.printlnf("humidity: %d%%", (int)humidity);
        }
        else
            Serial.printlnf("\tTemp/Hum Read Failure");
    #endif
}

void printBatteryInfo()
{
    Serial.println("\n*** Battery Data:");
    Serial.printlnf("\tbattvolt: %.2fV", battvolt);
}

#ifdef LightSensor
    void printLightInfo()
    {
        Serial.println("\n*** Light Data:");
        Serial.printf("\tlightValue: %d, ", lightValue);
        Serial.printlnf("lightValueRaw: %d", lightValueRaw);
    }
#endif
