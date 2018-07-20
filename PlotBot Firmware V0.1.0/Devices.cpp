#include "application.h"//needed for all Particle libraries
#include "Devices.h"

void InitializeDevices()
{
    device = new PlotBotDevice();
    if (strcmp(System.deviceID(), "1d0028000247343337373738") == 0)
    {
        strcpy(device->DeviceId,                "1d0028000247343337373738");
        strcpy(device->DeviceName,              "Garden_Zombie");
        device->ZipCode                         = 98272;
        device->Latitude                        = 47.8562834;
        device->Longitude                       = -121.966498;
        device->Elevation                       = 69;

        device->ReportToThingSpeak              = 1;
        device->ThingSpeakChannelNumber         = 453149;
        strcpy(device->ThingSpeakWriteApiKey,   "AFVCJ483S6L1SVM4");

        device->ReportToWunderground            = 1;
        strcpy(device->WundergroundPwsId,       "KWAMONRO71");
        strcpy(device->WundergroundPwsPassword, "k3mjjpq6");

        device->ReportToAzure                   = 1;

        device->SleepInterval                   = 600;
    }
    else if (strcmp(System.deviceID(), "45002b000a47363339343638") == 0)
    {
        strcpy(device->DeviceId,                "45002b000a47363339343638");
        strcpy(device->DeviceName,              "PlotBot-98272-01");
        device->ZipCode                         = 98272;
        device->Latitude                        = 47.8562834;
        device->Longitude                       = -121.966498;
        device->Elevation                       = 69;

        device->ReportToThingSpeak              = 1;
        device->ThingSpeakChannelNumber         = 524783;
        strcpy(device->ThingSpeakWriteApiKey,   "0IBB8Y0SGUYV6AET");

        device->ReportToWunderground            = 1;
        strcpy(device->WundergroundPwsId,       "KWAMONRO84");
        strcpy(device->WundergroundPwsPassword, "rzftmbdm");

        device->ReportToAzure                   = 1;

        device->SleepInterval                   = 0;
    }
    else if (strcmp(System.deviceID(), "41003a000a47363339343638") == 0)
    {
        strcpy(device->DeviceId,                "41003a000a47363339343638");
        strcpy(device->DeviceName,              "PlotBot-98101-01");
        device->ZipCode                         = 98272;
        device->Latitude                        = 47.608679;
        device->Longitude                       = -122.340316;
        device->Elevation                       = 538;

        device->ReportToThingSpeak              = 1;
        device->ThingSpeakChannelNumber         = 524784;
        strcpy(device->ThingSpeakWriteApiKey,   "K3BZQFY4H8WIG5OS");

        device->ReportToWunderground            = 0;
        strcpy(device->WundergroundPwsId,       "");
        strcpy(device->WundergroundPwsPassword, "");

        device->ReportToAzure                   = 1;

        device->SleepInterval                   = 600;
    }
    else if (strcmp(System.deviceID(), "380046000c47363433353735") == 0)
    {
        strcpy(device->DeviceId,                "380046000c47363433353735");
        strcpy(device->DeviceName,              "PlotBot-98391-01");
        device->ZipCode                         = 98391;
        device->Latitude                        = 47.17704574;
        device->Longitude                       = -122.186505;
        device->Elevation                       = 572;

        device->ReportToThingSpeak              = 1;
        device->ThingSpeakChannelNumber         = 524785;
        strcpy(device->ThingSpeakWriteApiKey,   "ZFB4EP9ZPL54QDO0");

        device->ReportToWunderground            = 0;
        strcpy(device->WundergroundPwsId,       "");
        strcpy(device->WundergroundPwsPassword, "");

        device->ReportToAzure                   = 1;

        device->SleepInterval                   = 60;
    }
    else if (strcmp(System.deviceID(), "33002b000e47363433353735") == 0)
    {
        strcpy(device->DeviceId,                "33002b000e47363433353735");
        strcpy(device->DeviceName,              "PlotBot-98034-01");
        device->ZipCode                         = 98034;
        device->Latitude                        = 47.704943;
        device->Longitude                       = -122.215524;
        device->Elevation                       = 538;

        device->ReportToThingSpeak              = 1;
        device->ThingSpeakChannelNumber         = 524786;
        strcpy(device->ThingSpeakWriteApiKey,   "KEHOQLJ79MNV8O3T");

        device->ReportToWunderground            = 0;
        strcpy(device->WundergroundPwsId,       "");
        strcpy(device->WundergroundPwsPassword, "");

        device->ReportToAzure                   = 1;

        device->SleepInterval                   = 60;
    }
}
