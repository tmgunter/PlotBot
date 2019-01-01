#include "application.h"
#include "PlotBotMesh.h"

#include "Devices.h"
#include "Instruments.h"
#include "Serial.h"
#include "Azure.h"

PlotBotDevice *device = new PlotBotDevice();

bool 		bTime = true;
int 		ledActivityPin = D7;
unsigned	startMillis;
char		version[10] = "0.2.0";
unsigned	displayDelay = 5000;
char 		localTime[30];
float 		battvolt = 0.0;

std::map<std::string, PlotBotDevice*> fleet;
std::map<std::string, std::string> events; 

#ifdef SSD1306_128x32x
	#include <SSD1306_128x32.h>

	#define OLED_RESET D4
	SSD1306_128x32 display(OLED_RESET);

	int button = 1;
	int newbutton = 1;
#endif

#ifdef DHT11
	#define DHTPIN 	D4     // set pin
	DHT dht(DHTPIN);

	float humidity = 0;
	float tempf = 0;
	float tempc = 0;
	double dewptf = 0;
	double dewptc = 0;
#endif

#ifdef TM1637Displayx
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // 4-digit display
  ////////////////////////////////////////////////////////////////////////////////////////////////////
	#include "TM1637Display.h"

	#define CLK D2
	#define DIO D3
	uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
	TM1637Display displayTime(CLK, DIO);
#endif

#ifdef CHAINABLE_LED
	#include "ChainableLED.h"

	#define NUM_LEDS 1
	ChainableLED leds(4, 5, NUM_LEDS);
	float hue = 0.0;
	boolean up = true;
#endif
    
void setup() 
{
    STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

    #if PLATFORM_ID == PLATFORM_ARGON
        WiFi.selectAntenna(ANT_EXTERNAL);
		Mesh.subscribe("XenonData", XenonDataHandler);
    #endif
    #if PLATFORM_ID == PLATFORM_XENON
        //Mesh.selectAntenna(ANT_EXTERNAL);
		Mesh.subscribe("ArgonData", ArgonDataHandler);
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
	pinMode(BATT, INPUT);

	#ifdef DHT11
		// Create Particle.variables for each piece of data for easy access
		dht.begin();
		Particle.variable("humidity", (double)humidity);
		Particle.variable("tempf", (double)tempf);
		Particle.variable("dewptf", (double)dewptf);
		Particle.variable("battvolt", (double)battvolt);
	#endif
    
	#ifdef TM1637Displayx
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// 4-digit display
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//displayTime.setBrightness(0xff); //set the diplay to maximum brightness
		displayTime.setBrightness(0x0a); //not so bright

		// Set center colon on without array - Start
		uint8_t segto;
		int value = 1244;
		segto = 0x80 | displayTime.encodeDigit((value / 100) % 10); // And off will be without this 0x80 bit set
		displayTime.setSegments(&segto, 1, 1);
	#endif
    
	#ifdef SSD1306_128x32x
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
	#endif

	#ifdef CHAINABLE_LED
		leds.init();
	#endif
}

void loop() 
{
	digitalWrite(ledActivityPin, HIGH);
	delay(250);
	digitalWrite(ledActivityPin, LOW);

	int h = Time.hourFormat12();
	int m = Time.minute();
	int s = Time.second();
	char ampm[5];
	if (Time.isAM())
		strcpy(ampm, "am");
	else
		strcpy(ampm, "pm");
	sprintf(localTime, "%02d/%02d/%02d %02d:%02d:%02d%s", Time.month(), Time.day(), Time.year(), h, m, s, ampm);

	#ifdef SSD1306_128x32x
	if(digitalRead(BUTTON_A) == LOW)
		button = 1;
	if(digitalRead(BUTTON_B) == LOW)
		button = 2;
	if(digitalRead(BUTTON_C) == LOW)
		button = 3;
	#endif

	#if PLATFORM_ID == PLATFORM_ARGON
	if (((unsigned)(millis() - startMillis) > displayDelay) || (button != newbutton))
	#elif PLATFORM_ID == PLATFORM_XENON
	if ((unsigned)(millis() - startMillis) > displayDelay)
	#endif
	{
		#ifdef SSD1306_128x32x
			button = newbutton;
		#endif

		Serial.printlnf("");
		#if PLATFORM_ID == PLATFORM_ARGON
			Serial.printlnf("PlotBot ControlNode (Argon) V%s", version);
		#endif
		#if PLATFORM_ID == PLATFORM_XENON
			Serial.printlnf("PlotBot SensorNode (Xenon) V%s", version);
		#endif

		Serial.printf("\n%s\tcloudReady: %d, ", localTime, Particle.connected());
		#if PLATFORM_ID == PLATFORM_ARGON
			Serial.printf("wifiReady: %d, ", WiFi.ready());
			Serial.printlnf("meshReady: %d", Mesh.ready());
		#elif PLATFORM_ID == PLATFORM_XENON
			Serial.printlnf("meshReady: %d", Mesh.ready());
		#endif

		Serial.println("\n*** Config Data:");
		Serial.printf("\tDeviceId: %s, ", device->DeviceId.c_str());
		Serial.printf("Name: %s, ", device->DeviceName.c_str());
		Serial.printlnf("ZipCode: %ld", device->ZipCode);

		Serial.printf("\tLatitude: %lf, ", device->Latitude);
		Serial.printf("Longitude %lf, ", device->Longitude);
		Serial.printlnf("Elevation: %ld", device->Elevation);

		Serial.printf("\tReportToThingSpeak: %ld, ", device->ReportToThingSpeak);
		Serial.printf("ThingSpeakChannelNumber: %lu, ", device->ThingSpeakChannelNumber);
		Serial.printlnf("ThingSpeakWriteApiKey: %s, ", device->ThingSpeakWriteApiKey.c_str());

		Serial.printf("\tReportToWunderGround: %ld, ", device->ReportToWunderground);
		Serial.printf("WunderGroundPwsiD: %s, ", device->WundergroundPwsId.c_str());
		Serial.printlnf("WunderGroundPassword: %s", device->WundergroundPwsPassword.c_str());

		Serial.printlnf("\tReportToAzure: %ld", device->ReportToAzure);

		Serial.printlnf("\tSleepInterval: %ld", device->SleepInterval);

		Serial.printlnf("\tButton: %ld", device->Button);

		calcBatteryInfo();
		printBatteryInfo();

		#ifdef DHT11
			calcWeatherInfo();
			printWeatherInfo();
		#endif

		#ifdef TM1637Displayx
			if (bTime)
			{
				int num = (h * 100) + m;
				displayTime.showNumberDecEx(num, 0b01000000, true, 4, 0);
				bTime = false;
			}
			else
			{
				#ifdef DHT11
					int num = (int)(tempf * 100);
					displayTime.showNumberDecEx(num, 0b01000000, true, 4, 0);
				#endif
				bTime = true;
			}
		#endif

		#ifdef SSD1306_128x32x
			display.clearDisplay();
			display.display();
			display.setTextSize(2);
			display.setTextColor(WHITE);
			display.setCursor(0, 0);
			char displayStr[100];
			sprintf(displayStr, "T:%02d:%02d:%02d", h, m, s);
			switch (button)
			{
				case 1:
					sprintf(displayStr, "%sButton A", displayStr);
					break;
					
				case 2:
					sprintf(displayStr, "%sButton B", displayStr);
					break;
					
				case 3:
					sprintf(displayStr, "%sButton C", displayStr);        
					break;
			}
			display.println(displayStr);
			display.setCursor(0,0);
			display.display(); // actually display all of the above
		#endif  

		if (device->ReportToAzure == 1)
			sendInfoToAzure();

		startMillis = millis();
	}

#ifdef CHAINABLE_LED
	for (byte i = 0; i < NUM_LEDS; i++)
		leds.setColorHSB(i, hue, 1.0, .025);
    
	delay(50);
    
	if (up)
		hue+= 0.025;
	else
		hue-= 0.025;
    
	if (hue>=1.0 && up)
		up = false;
	else if (hue<=0.0 && !up)
		up = true;	
#endif	

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


void DisplayEventData(int button)
{

}