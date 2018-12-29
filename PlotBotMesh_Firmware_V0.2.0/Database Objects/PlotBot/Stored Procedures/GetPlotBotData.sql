/****** Object:  StoredProcedure [dbo].[GetPlotBotData]    Script Date: 8/1/2018 2:21:25 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO


--DECLARE @FahrOrCelcius varchar(1) = 'F'
--DECLARE @OffsetFromUTC int = -7
--DECLARE @Interval int = -48

ALTER PROCEDURE [dbo].[GetPlotBotData]
	@FahrOrCelcius varchar(1) = 'F',
	@OffsetFromUTC int = -7,
	@Interval int = -48
AS
BEGIN

SELECT 
	DATEADD(HH, @OffsetFromUTC, pdb.published_at) AS LocalTime,
	d.DeviceName,
	pdb.Soc,
	pdb.BatVoltage,
	CASE
		WHEN @FahrOrCelcius = 'F' THEN 
			CAST(ROUND(pdb.TempF, 2) AS float)
		ELSE
			ROUND (CAST( (pdb.tempf - 32.0) * (5.0 / 9.0) AS float), 2) 
	END AS Temp,
	CASE
		WHEN @FahrOrCelcius = 'F' THEN 
			pdb.DewPtF 
		ELSE
			ROUND (CAST( (pdb.DewPtF - 32.0) * (5.0 / 9.0) AS float), 2) 
	END as DewPt,
	pdb.Hum AS Hum,
	pdb.SoilMoistPercent AS SoilMoistPercent,
	CASE
		WHEN @FahrOrCelcius = 'F' THEN 
			pdb.SoilTempF 
		ELSE
			ROUND (CAST( (pdb.SoilTempF - 32.0) * (5.0 / 9.0) AS float), 2) 
	END as SoilTemp,
	pdb.Lux AS Lux,
	pdb.BaromIn AS BaromIn,
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
	DATEADD(HH, @OffsetFromUTC, pdb.published_at) AS published_at_PSTTime,
	DATEADD(HH, @OffsetFromUTC, pdb.EventEnqueuedUtcTime) AS EventEnqueuedPSTTime,
	DATEADD(HH, @OffsetFromUTC, pdb.EventProcessedUtcTime) AS EventProcessedPSTTime
FROM 
	rawsignals.PlotBot_AllData AS pdb
INNER JOIN 
	Devices AS d ON pdb.device_id = d.DeviceId
WHERE 
	pdb.published_at >= DATEADD(HH, @Interval, GETUTCDATE())

END
GO


