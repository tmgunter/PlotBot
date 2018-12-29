#include "application.h"
#include "PlotBotMesh.h"

#include "Azure.h"
#include "Instruments.h"

void sendInfoToAzure()
{
 	#ifdef DHT11
	  Particle.publish("Humidity", String::format("%d", (int)humidity), PRIVATE);
	  Particle.publish("TempF", String::format("%.2f", tempf), PRIVATE);
	  Particle.publish("DewPointF", String::format("%.2f", dewptf), PRIVATE);
 	#endif
	
	Particle.publish("BattVoltage", String::format("%.2f", battvolt), PRIVATE);
}

void sendConfigToAzure()
{
 	// Big, old placeholder
}
