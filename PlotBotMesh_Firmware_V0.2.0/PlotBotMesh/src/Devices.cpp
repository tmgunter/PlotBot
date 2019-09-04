#include "application.h"
#include "PlotBotMesh.h"

#include "Devices.h"

PlotBotDevice devices[FLEET_SIZE];

void InitializeDevices()
{
        PlotBotDevice* _device = new PlotBotDevice();
        _device->NodeType                   = PLATFORM_ARGON;
        _device->DeviceId                   = "e00fce68930b9e15c7b8139f";
        _device->DeviceName                 = "Rain-Argon";
        _device->ZipCode                    = 98272;
        _device->Latitude                   = 47.85555;
        _device->Longitude                  = -121.96772;
        _device->Elevation                  = 69;

        _device->ReportToThingSpeak         = 0;
        _device->ThingSpeakChannelNumber    = 453149;
        _device->ThingSpeakWriteApiKey      = "AFVCJ483S6L1SVM4";

        _device->ReportToWunderground       = 0;
        _device->WundergroundPwsId          = "KWAMONRO71";
        _device->WundergroundPwsPassword    = "k3mjjpq6";

        _device->ReportToCloud              = 1;

        _device->SleepInterval              = 0;

        _device->Button                     = BUTTON_A;
        fleet[_device->DeviceId]            = _device;

        _device = new PlotBotDevice();
        _device->NodeType                    = PLATFORM_XENON;
        _device->DeviceId                   = "e00fce683c178a3e19c2a424";
        _device->DeviceName                 = "Rain-Xenon-1";
        _device->ZipCode                    = 98272;
        _device->Latitude                   = 47.85555;
        _device->Longitude                  = -121.96772;
        _device->Elevation                  = 69;

        _device->ReportToThingSpeak         = 0;
        _device->ThingSpeakChannelNumber    = 524784;
        _device->ThingSpeakWriteApiKey      = "K3BZQFY4H8WIG5OS";

        _device->ReportToWunderground       = 0;
        _device->WundergroundPwsId          = "";
        _device->WundergroundPwsPassword    = "";

        _device->ReportToCloud              = 1;

        _device->SleepInterval              = 0;
        
        _device->Button                     = BUTTON_B;
        fleet[_device->DeviceId]            = _device;

        device = fleet[System.deviceID()];

        _device = new PlotBotDevice();
        _device->NodeType                   = PLATFORM_XENON;
        _device->DeviceId                   = "e00fce68c8e53c3766e107c9";
        _device->DeviceName                 = "Rain-Xenon-2";
        _device->ZipCode                    = 98272;
        _device->Latitude                   = 47.85555;
        _device->Longitude                  = -121.96772;
        _device->Elevation                  = 69;

        _device->ReportToThingSpeak         = 0;
        _device->ThingSpeakChannelNumber    = 524783;
        _device->ThingSpeakWriteApiKey      = "0IBB8Y0SGUYV6AET";

        _device->ReportToWunderground       = 0;
        _device->WundergroundPwsId          = "KWAMONRO84";
        _device->WundergroundPwsPassword    = "rzftmbdm";

        _device->ReportToCloud              = 1;

        _device->SleepInterval              = 0;
        
        _device->Button                     = BUTTON_C;
        fleet[_device->DeviceId]            = _device;
}
