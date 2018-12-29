#include "application.h"
#include "PlotBotMesh.h"

#include "Devices.h"

void InitializeDevices()
{
    device = new PlotBotDevice();
    if (strcmp(System.deviceID(), "e00fce68930b9e15c7b8139f") == 0)
    {
        strcpy(device->DeviceId,                "e00fce68930b9e15c7b8139f");
        strcpy(device->DeviceName,              "PlotBot-Argon");
        device->ZipCode                         = 98272;
        device->Latitude                        = 47.8562834;
        device->Longitude                       = -121.966498;
        device->Elevation                       = 69;

        device->ReportToThingSpeak              = 0;
        device->ThingSpeakChannelNumber         = 453149;
        strcpy(device->ThingSpeakWriteApiKey,   "AFVCJ483S6L1SVM4");

        device->ReportToWunderground            = 0;
        strcpy(device->WundergroundPwsId,       "KWAMONRO71");
        strcpy(device->WundergroundPwsPassword, "k3mjjpq6");

        device->ReportToAzure                   = 1;

        device->SleepInterval                   = 0;
    }
    else if (strcmp(System.deviceID(), "e00fce68c8e53c3766e107c9") == 0)
    {
        strcpy(device->DeviceId,                "e00fce68c8e53c3766e107c9");
        strcpy(device->DeviceName,              "PlotBot-Xenon-1");
        device->ZipCode                         = 98272;
        device->Latitude                        = 47.8562834;
        device->Longitude                       = -121.966498;
        device->Elevation                       = 69;

        device->ReportToThingSpeak              = 0;
        device->ThingSpeakChannelNumber         = 524783;
        strcpy(device->ThingSpeakWriteApiKey,   "0IBB8Y0SGUYV6AET");

        device->ReportToWunderground            = 0;
        strcpy(device->WundergroundPwsId,       "KWAMONRO84");
        strcpy(device->WundergroundPwsPassword, "rzftmbdm");

        device->ReportToAzure                   = 1;

        device->SleepInterval                   = 0;
    }
    else if (strcmp(System.deviceID(), "e00fce683c178a3e19c2a424") == 0)
    {
        strcpy(device->DeviceId,                "e00fce683c178a3e19c2a424");
        strcpy(device->DeviceName,              "PlotBot-Xenon-2");
        device->ZipCode                         = 98272;
        device->Latitude                        = 47.8562834;
        device->Longitude                       = -121.966498;
        device->Elevation                       = 69;

        device->ReportToThingSpeak              = 0;
        device->ThingSpeakChannelNumber         = 524784;
        strcpy(device->ThingSpeakWriteApiKey,   "K3BZQFY4H8WIG5OS");

        device->ReportToWunderground            = 0;
        strcpy(device->WundergroundPwsId,       "");
        strcpy(device->WundergroundPwsPassword, "");

        device->ReportToAzure                   = 1;

        device->SleepInterval                   = 0;
    }
}
