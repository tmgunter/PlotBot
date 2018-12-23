#include <HttpClient.h>
#include "Instruments.h"
#include "Serial.h"
#include "Devices.h"

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
        
        sprintf(URL, "%s?ID=%s&PASSWORD=%s&action=updateraw&dateutc=now&realtime=1&rtfreq=6", WEBPAGE, device->WundergroundPwsId, device->WundergroundPwsPassword);
        sprintf(URL, "%s&humidity=%.2f", URL, humidity);
        sprintf(URL, "%s&dewptf=%.2f", URL, dewptf);
        sprintf(URL, "%s&tempf=%.2f", URL, tempf);

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
    //This function prints the weather data out to the default Serial Port
    Serial.println("\n*** Sensor Data:");
    Serial.printf("\ttempf: %.2fF, ", tempf);
    Serial.printf("dewptf: %.2fF, ", dewptf);
    Serial.printlnf("humidity: %.2f%%, ", humidity);
}