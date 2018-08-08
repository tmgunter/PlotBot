/****** Object:  Table [rawsignals].[PlotBot_AllData]    Script Date: 7/30/2018 11:00:09 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [rawsignals].[PlotBot_AllData](
	[device_id] [nvarchar](100) NOT NULL,
	[event_name] [nvarchar](50) NULL,
	[published_at] [datetime] NULL,
	[EventProcessedUtcTime] [datetime] NOT NULL,
	[EventEnqueuedUtcTime] [datetime] NULL,
	[Soc] [float] NULL,
	[TempF] [float] NULL,
	[Hum] [float] NULL,
	[DewPtF] [float] NULL,
	[SoilMoistPercent] [float] NULL,
	[SoilTempF] [float] NULL,
	[Lux] [float] NULL,
	[BaromIn] [float] NULL,
	[Collectms] [bigint] NULL,
	[full_json] [nvarchar](500) NULL,
	[BatVoltage] [float] NULL
	CONSTRAINT PK_DeviceId_EventProcessedUtcTime PRIMARY KEY ([device_id], [published_at])
) ON [PRIMARY]
GO

EXEC sys.sp_addextendedproperty @name=N'MS_Description', @value=N'Raw events from stream analytics (mildy processed).' , @level0type=N'SCHEMA',@level0name=N'rawsignals', @level1type=N'TABLE',@level1name=N'PlotBot_AllData'
GO


