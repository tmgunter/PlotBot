/****** Object:  View [dbo].[RecentPlotBotDataC]    Script Date: 8/1/2018 2:33:24 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE VIEW [dbo].[RecentPlotBotDataC]
AS
SELECT
	DATEADD(HH, - 7, pdb.published_at) AS LocalTime, 
	d.DeviceName, 
	pdb.Soc, 
	pdb.BatVoltage, 
	ROUND(CAST((pdb.TempF - 32.0) * (5.0 / 9.0) AS float), 2) AS TempC, 
	ROUND(CAST((pdb.DewPtF - 32.0) * (5.0 / 9.0) AS float), 2) AS DewPtC, 
	pdb.Hum, 
	pdb.SoilMoistPercent, 
	ROUND(CAST((pdb.SoilTempF - 32.0) * (5.0 / 9.0) AS float), 2) AS SoilTempC, 
	pdb.Lux, 
	pdb.BaromIn, 
	pdb.Collectms, 
	pdb.device_id, 
	d.ZipCode, 
	d.Latitude, 
	d.Longitude, 
	d.Elevation, 
    d.SleepInterval, 
	d.ReportToAzure, 
	d.ReportToThingSpeak, 
	d.ReportToWunderground, 
	pdb.event_name, 
	DATEADD(HH, - 7, pdb.published_at) AS published_at_PSTTime, 
	DATEADD(HH, - 7, pdb.EventEnqueuedUtcTime) AS EventEnqueuedPSTTime, 
	DATEADD(HH, - 7, pdb.EventProcessedUtcTime) AS EventProcessedPSTTime
FROM
	rawsignals.PlotBot_AllData AS pdb 
	INNER JOIN dbo.Devices AS d ON pdb.device_id = d.DeviceId
WHERE 
	(pdb.published_at >= DATEADD(HH, - 48, GETUTCDATE()))
GO


