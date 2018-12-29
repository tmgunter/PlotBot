#include "application.h"
#include "PlotBotMesh.h"

#include "Devices.h"
#include "Instruments.h"
#include "Azure.h"

void sendInfoToAzure()
{
	Serial.println("\n*** Azure Payload:");

	char payload[500];
	//sprintf(payload,  "{\"Data\": [{\"Name\":\"%s\",\"Time\":\"%s\",", device->DeviceName, localTime);
	sprintf(payload,  "{\"Data\": [{\"Name\":\"%s\",", device->DeviceName);
	#if PLATFORM_ID == PLATFORM_ARGON
		sprintf(payload, "%s\"BattV\": \"%.2f\"}]}", payload, battvolt);
		if (Particle.connected())
			Particle.publish("ArgonData", payload);
		Serial.printlnf("\tCloud: %s", payload);
	#elif PLATFORM_ID == PLATFORM_XENON
		#ifdef DHT11
			if (tempf == NAN || dewptf == NAN || humidity == NAN || battvolt == NAN)
				return;
			
			sprintf(payload, 
					"%s\"TempF\":\"%.2f\",\"DewPtF\": \"%.2f\",\"Hum%%\": \"%d\",",
					payload,
					tempf,
					dewptf,
					(int)humidity);
		#endif
		sprintf(payload, "%s\"BattV\": \"%.2f\"}]}", payload, battvolt);				

		if (Mesh.ready())
			Mesh.publish("XenonData", payload);
		Serial.printlnf("\tMesh: %s", payload);
	#endif
}

void sendConfigToAzure()
{
 	// Big, old placeholder
}
