#include "Azure.h"
#include "Instruments.h"

void sendInfoToAzure()
{
    char soc_s[10];
    sprintf(soc_s, "%.2f", batterySoc);
    
    char tempf_s[10];
    sprintf(tempf_s, "%.2f", tempf);
    
    char humidity_s[10];
    sprintf(humidity_s, "%.2f", humidity);
    
    char dewptf_s[10];
    sprintf(dewptf_s, "%.2f", dewptf);
    
    char soiltempf_s[10];
    sprintf(soiltempf_s, "%.2f", soiltempf);
    
    char lux_s[10];
    sprintf(lux_s, "%.2f", lux);
    
    char baromin_s[10];
    sprintf(baromin_s, "%.2f", baromin);
    
    String jsonDataString = String( 
        "{ \"Soc\":" + String(soc_s) + 
        ",\"TempF\":" + String(tempf_s) + 
        ",\"Hum\":" + String(humidity_s) + 
        ",\"DewPtF\":" + String(dewptf_s) + 
        ",\"SoilMoist%\":" + String(soilmoisturePercentage) + 
        ",\"SoilTempF\":" + String(soiltempf_s) +
        ",\"Lux\":" + String(lux_s) +
        ",\"BaromIn\":" + String(baromin_s) +
        ",\"ZipCode\":" + String(device->ZipCode) + 
        ",\"Lat\":" + String(device->Latitude) +
        ",\"Long\":" + String(device->Longitude) +
        ",\"Elev\":" + String(device->Elevation) +
        ",\"PWS\":" + String(device->WundergroundPwsId) + 
        ",\"Collectms\":" + String(collectionTime) + 
        "}");
        
    Serial.printlnf("\n\tAzure JSON: %s", jsonDataString.c_str());
        
    Particle.publish("PlotBot Data", jsonDataString, PRIVATE);
}

void sendConfigToAzure()
{
// Big, old placeholder
}