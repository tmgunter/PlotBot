/****** Object:  View [dbo].[RecentPlotBotDataF]    Script Date: 8/1/2018 2:36:50 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE VIEW [dbo].[RecentPlotBotDataF]
AS
SELECT
	d.DeviceId,
	DATEADD(HH, - 7, pdb.published_at) AS LocalTime, 
	d.DeviceName, 
	pdb.Soc, 
	pdb.BatVoltage, 
	ROUND(pdb.TempF, 2) AS TempF, 
	ROUND(pdb.DewPtF, 2) AS DewPtF, 
	pdb.Hum, 
	pdb.SoilMoistPercent, 
	ROUND(pdb.SoilTempF, 2) AS SoilTempF, 
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


