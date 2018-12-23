#include "Azure.h"
#include "Instruments.h"

void sendInfoToAzure()
{
    Particle.publish("TempF", String(tempf), PRIVATE);
    Particle.publish("Humidity", String(humidity), PRIVATE);
    Particle.publish("DewPointF", String(dewptf), PRIVATE);
}

void sendConfigToAzure()
{
// Big, old placeholder
}