#include "application.h"//needed for all Particle libraries
#include "Devices.h"

std::array<PlotBotDevice *, DEVICE_COUNT> devices;

PlotBotDevice* InitializeDevice(int index, char *deviceId, char *deviceName, unsigned long zipCode, double latitude, double longitude, unsigned int elevation, 
                        unsigned int reportToThingSpeak, unsigned long thingSpeakChannelNumber, char *thingSpeakWriteApiKey, 
                        unsigned int reportToWunderground, char *wundergroundPwsId, char *wundergroundPwsPassword,
                        unsigned int reportToAzure,
                        unsigned int sleepInterval)
    {
        PlotBotDevice *device = new PlotBotDevice();
        
        device->DeviceId                = deviceId;
        device->DeviceName              = deviceName;
        device->ZipCode                 = zipCode;
        device->Latitude                = latitude;
        device->Longitude               = longitude;
        device->Elevation               = elevation;
        
        device->ReportToThingSpeak      = reportToThingSpeak;
        device->ThingSpeakChannelNumber = thingSpeakChannelNumber;
        device->ThingSpeakWriteApiKey   = thingSpeakWriteApiKey;
        
        device->ReportToWunderground    = reportToWunderground;
        device->WundergroundPwsId       = wundergroundPwsId;
        device->WundergroundPwsPassword = wundergroundPwsPassword;
        
        device->ReportToAzure           = reportToAzure;
        
        device->SleepInterval           = sleepInterval;

        
        return device;
    }

void InitializeDevices()
{
    devices[1] = InitializeDevice(1, "1d0028000247343337373738", "Garden_Zombie",     98272, 47.8562834, -121.966498,  69, 
                                        1, 453149, "AFVCJ483S6L1SVM4", 
                                        1, "KWAMONRO71", "k3mjjpq6",
                                        0,
                                        300);
    devices[2] = InitializeDevice(2, "45002b000a47363339343638", "Plot-Bot-98272-01", 98272, 47.8562834, -121.966498,  69, 
                                        1, 524783, "0IBB8Y0SGUYV6AET", 
                                        0, "KWAMONRO71", "k3mjjpq6",
                                        0,
                                        300);  
    devices[3] = InitializeDevice(3, "41003a000a47363339343638", "Plot-Bot-98272-02", 98272, 47.8562834, -121.966498,  69, 
                                        1, 524784, "K3BZQFY4H8WIG5OS", 
                                        0, "KWAMONRO63", "scxex4ni",
                                        0,
                                        300);
    devices[4] = InitializeDevice(4, "000000000000000000000000", "Plot-Bot-98391-01", 98391, 47.1770457, -122.186505, 572, 
                                        1, 524785, "ZFB4EP9ZPL54QDO0", 
                                        0, "", "",
                                        0,
                                        300);   
    devices[5] = InitializeDevice(5, "000000000000000000000000", "Plot-Bot-98391-02", 98391, 47.1770457, -122.186505, 572, 
                                        1, 524786, "KEHOQLJ79MNV8O3T", 
                                        0, "", "",
                                        0,
                                        300);
}