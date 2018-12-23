/*
 * Project PlotBotMesh_ControlNode
 * Description:
 * Author:
 * Date:
 */
 
#include "Azure.h"
#include "Serial.h"
#include "Instruments.h"
#include "Devices.h"

#include <ThingSpeak.h>
#include <TM1637Display.h>
#include <HttpClient.h>
#include <SSD1306_128x32.h>

PlotBotDevice *device = new PlotBotDevice();

    #define OLED_RESET D4
    SSD1306_128x32 display(OLED_RESET);
    int button = 2;

  #define BUTTON_A  4
  #define BUTTON_B  3
  #define BUTTON_C  2

////////////////////////////////////////////////////////////////////////////////////////////////////
// GPIO Pin Usage
////////////////////////////////////////////////////////////////////////////////////////////////////

// Digital Pins
    int ledActivityPin = D7;

// Analog Pins

////////////////////////////////////////////////////////////////////////////////////////////////////
// Weather Shield Variables
////////////////////////////////////////////////////////////////////////////////////////////////////

double humidity = 0;
double tempf = 0;     //Average of the sensors temperature readings, fahrenheit
double tempc = 0;     //Average of the sensors temperature readings, celsius
double dewptf = 0;
double dewptc = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
// 4-digit display
////////////////////////////////////////////////////////////////////////////////////////////////////
    #define CLK D2
    #define DIO D3
    uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
    TM1637Display displayTime(CLK, DIO);
    
void setup() {
    STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

    #if PLATFORM_ID == 12
        WiFi.selectAntenna(ANT_EXTERNAL);
    #endif
    #if PLATFORM_ID == 14
        //Mesh.selectAntenna(ANT_EXTERNAL);
    #endif
    
    while(Time.year() <= 1970)
    {
        Particle.syncTime();  
        Particle.process();
        delay(100);
    }
    Time.zone(-8);
    
    Serial.begin(115200);   // open serial over USB at 9600 baud
    
    // Find our device
    delay(5000);
    InitializeDevices();
    
// Make sure your Serial Terminal app is closed before powering your device
// Now open your Serial Terminal, and hit any key to continue!
//  Serial.println("Press any key to begin");
// This line pauses the Serial port until a key is pressed
//  while(!Serial.available()) 
//      Particle.process();

    pinMode(ledActivityPin, OUTPUT);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Weather Shield Setup
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Create Particle.variables for each piece of data for easy access
    Particle.variable("humidity", &humidity, DOUBLE);
    Particle.variable("tempf", &tempf, DOUBLE);
    Particle.variable("dewptf", &dewptf, DOUBLE);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // 4-digit display
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    displayTime.setBrightness(0xff); //set the diplay to maximum brightness
    //displayTime.setBrightness(0x0a); //not so bright

    // Set center colon on without array - Start
    uint8_t segto;
    int value = 1244;
    segto = 0x80 | displayTime.encodeDigit((value / 100) % 10); // And off will be without this 0x80 bit set
    displayTime.setSegments(&segto, 1, 1);
    //delay(1000) ; // constant colon
    // End
    
    Serial.println("OLED FeatherWing");
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
    Serial.println("OLED begun");
    
    // Clear the buffer.
    display.clearDisplay();
    display.display();
    
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);
}

void loop() {
    Serial.printlnf("");
    #if PLATFORM_ID == 12
        Serial.printlnf("PlotBot ControlNode (Argon) v0.1.0 (Post-Hackathon 2018)");
    #endif
    #if PLATFORM_ID == 14
        Serial.printlnf("PlotBot SensorNode (Xenon) v0.1.0 (Post-Hackathon 2018)");
    #endif
    
    // Print Data array - Show Time, 24-hour format
    int h = Time.hour();
    int m = Time.minute();
    int s = Time.second();
    data[0] = displayTime.encodeDigit(h / 10);
    data[1] = displayTime.encodeDigit(h % 10) | (Time.second() % 2 ? 0x80 : 0x00);
    data[2] = displayTime.encodeDigit(m / 10);
    data[3] = displayTime.encodeDigit(m % 10);
    displayTime.setSegments(data);
    delay(1000);
    
    digitalWrite(ledActivityPin, HIGH);
	  delay(500);
	  digitalWrite(ledActivityPin, LOW);

    Serial.printlnf("");
	
	int startMillis = millis();
    calcWeatherInfo();
    Serial.printlnf("");
    Serial.printlnf("%s: Elapsed collection time: %ldMS", Time.timeStr().c_str(), millis() - startMillis);
    
    bool cloudReady = Particle.connected();
    Serial.printf("\tcloudReady: %d, ", cloudReady);

    #if PLATFORM_ID == 12
      bool wifiReady = WiFi.ready();
      Serial.printlnf("wifiReady: %d", wifiReady);
    #elif PLATFORM_ID == 14
      bool meshReady = Mesh.ready();
      Serial.printlnf("meshReady: %d", meshReady);
    #endif

    Serial.println("");
    Serial.println("\n*** Config Data:");
    Serial.printf("\tDeviceId: %s, ", device->DeviceId);
    Serial.printf("Name: %s, ", device->DeviceName);
    Serial.printf("ZipCode: %ld, ", device->ZipCode);
    Serial.printf("Latitude: %lf, ", device->Latitude);
    Serial.printf("Longitude %lf, ", device->Longitude);
    Serial.printlnf("Elevation: %ld", device->Elevation);
    
    Serial.printf("\tReportToWunderGround: %ld, ", device->ReportToWunderground);
    Serial.printf("WunderGroundPwsiD: %s, ", device->WundergroundPwsId);
    Serial.printlnf("WunderGroundPassword: %s", device->WundergroundPwsPassword);

    Serial.printlnf("\tReportToAzure: %ld", device->ReportToAzure);
    
    Serial.printlnf("\tSleepInterval: %ld", device->SleepInterval);
    
    // text display tests
    display.clearDisplay();
    display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    char localTime[12];
    sprintf(localTime, "T:%02d:%02d:%02d", h, m, s);
    display.println(localTime);
    Serial.printlnf("button: %d", button);
    switch (button)
    {
        case 1:
            display.println("Button A");
            break;
            
        case 2:
            display.println("Button B");
            break;
            
        case 3:
            display.println("Button C");        
            break;
    }
    display.setCursor(0,0);
    display.display(); // actually display all of the above
      
    printWeatherInfo();
    

    if(digitalRead(BUTTON_A) == LOW) {
        button = 1;
    }
    if(digitalRead(BUTTON_B) == LOW) {
        button = 2;
    }
        if(digitalRead(BUTTON_C) == LOW) {
        button = 3;
    }

    Serial.printlnf("");
    Serial.printlnf("%s: Elapsed processing time: %ldMS", Time.timeStr().c_str(), millis() - startMillis);

    if (device->SleepInterval > 0)
    {
        Serial.printlnf("%s: calling System.sleep(SLEEP_MODE_DEEP, %ld);", Time.timeStr().c_str(), device->SleepInterval);
        delay(2);
        System.sleep(SLEEP_MODE_DEEP, device->SleepInterval);
        Serial.printlnf("%s: sleeping...", Time.timeStr().c_str());
    }
    else
        delay(0000);
        
    Serial.begin(115200);
}