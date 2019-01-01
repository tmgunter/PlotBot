#include "application.h"
#include "PlotBotMesh.h"

#include "Devices.h"
#include "Instruments.h"
#include "Azure.h"
#include <stdio.h>

#include <ArduinoJson.h>

void sendInfoToAzure()
{
	Serial.println("\n*** Payload:");

	char payload[512];
	sprintf(payload,  "{\"nm\":\"%s\",\"dt\":%ld,", device->DeviceName.c_str(), Time.now());

	#if PLATFORM_ID == PLATFORM_ARGON
		sprintf(payload, "%s\"v\":%.2f}", payload, battvolt);
		if (Particle.connected())
			Particle.publish("ArgonData", payload);
		if (Mesh.ready())
			Mesh.publish("ArgonData", payload);
		Serial.printlnf("\tArgonData: %s", payload);
	#elif PLATFORM_ID == PLATFORM_XENON
		#ifdef DHT11
			if ((int)tempf != 32 && (int)humidity != 0)
			{
				sprintf(payload, 
						"%s\"tf\":%.2f,\"dpf\":%.2f,\"h\":%d,",
						payload,
						tempf,
						dewptf,
						(int)humidity);
			}
		#endif
		sprintf(payload, "%s\"v\":%.2f}", payload, battvolt);	

		char eventName[40];
		if (Mesh.ready())
		{
			sprintf(eventName, "XenonData-%s", device->DeviceId);
			Mesh.publish(eventName, payload);
		}
		Serial.printlnf("\t%s: %s", eventName, payload);
	#endif
}

void sendConfigToAzure()
{
 	// Big, old placeholder
}

#if PLATFORM_ID == PLATFORM_ARGON
void XenonDataHandler(const char *event, const char *dataIn)
	{
	 	Serial.printlnf("\n*** %s, %s", event, dataIn ? dataIn : "NULL");

		char *pos = std::strstr(event, "-");
		if (pos)
		{
			std::string _deviceId(++pos);
			std::string _data(dataIn);
			events[_deviceId] = _data;
		}
		/*
		char *data = new char[strlen(dataIn) + 1];
		strcpy (data, dataIn);
		
		StaticJsonBuffer<1000> jsonBuffer;
    	JsonObject& root = jsonBuffer.parseObject(data);

		if (!root.success())
		{
			Serial.println("parseObject() failed");
			return;
		}

    	const char* _deviceName = root["nm"];
    	unsigned long _time = root["dt"];
		float _battvolt = root["v"];
    
		Serial.printlnf("\tName: %s, Time: %s, BattVolt: %.2f", 
				_deviceName, 
				Time.format(_time, TIME_FORMAT_DEFAULT).c_str(), 
				_battvolt);	

		float _tempf = root["tf"];
		float _dewptf = root["dpf"];
		int _humidity = root["h"];

		Serial.printlnf("\tTempF: %.2f, DewPtF: %.2f, Humidity: %d", 
				_tempf, 
				_dewptf, 
				_humidity);	
		*/
	}
#endif

#if PLATFORM_ID == PLATFORM_XENON
void ArgonDataHandler(const char *event, const char *dataIn)
	{
		Serial.printlnf("\n*** %s, %s", event, dataIn ? dataIn : "NULL");

		char *data = new char[strlen(dataIn) + 1];
		strcpy (data, dataIn);

		StaticJsonBuffer<1000> jsonBuffer;
    	JsonObject& root = jsonBuffer.parseObject((char*)data);

		if (!root.success())
		{
			Serial.println("parseObject() failed");
			return;
		}

    	const char* _deviceName = root["nm"];
    	time_t _time = root["dt"];
		float _battvolt = root["v"];
    
	    Serial.printlnf("\tName: %s, Time: %s, BattVolt: %.2f", 
						_deviceName, 
						Time.format(_time, TIME_FORMAT_DEFAULT).c_str(), 
						_battvolt);
	}
#endif
