#include "application.h"
#include <HttpClient.h>

SYSTEM_MODE(MANUAL);

int ledActivityPin = D7;

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

HttpClient http;
TCPClient client;
char SERVER[] = "mynorthwest.com";
char WEBPAGE [] = "";

void setup()
{
  STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

  //set wifi antenna to external
  //WiFi.selectAntenna(ANT_EXTERNAL);

  while(Time.year() <= 1970)
  {
    Particle.syncTime();
    Particle.process();
    delay(100);
  }
  Time.zone(-7);

  Serial.begin(9600);   // open serial over USB at 9600 baud

  // Make sure your Serial Terminal app is closed before powering your device
  // Now open your Serial Terminal, and hit any key to continue!
  Serial.println("Press any key to begin");
  //This line pauses the Serial port until a key is pressed
  while(!Serial.available())
    Particle.process();

  pinMode(ledActivityPin, OUTPUT);

  WiFi.on();
  WiFi.connect();
}

void loop()
{
    Serial.println("At the top of the loop\n");
    digitalWrite(ledActivityPin, HIGH);
    delay(750);
    digitalWrite(ledActivityPin, LOW);

    Serial.print("\nConnecting to ");
    Serial.print(SERVER);
    Serial.print("... ");

    if (client.connect(SERVER, 80))
    {
        Serial.println("connected. ");

        // Request path and body can be set at runtime or at setup.
        request.hostname = SERVER;
        request.port = 80;
        request.path = WEBPAGE;
        request.body = "";

        Serial.printlnf("%s", WEBPAGE);

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

    delay(3000);
}
