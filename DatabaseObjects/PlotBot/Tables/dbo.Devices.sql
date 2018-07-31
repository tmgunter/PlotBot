/****** Object:  Table [dbo].[Devices]    Script Date: 7/30/2018 10:59:30 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[Devices](
	[Id] [bigint] IDENTITY(1,1) NOT NULL,
	[IsActive] [bit] NULL,
	[DeviceId] [nvarchar](50) NULL,
	[DeviceName] [nvarchar](50) NULL,
	[ZipCode] [bigint] NULL,
	[Latitude] [float] NULL,
	[Longitude] [float] NULL,
	[Elevation] [int] NULL,
	[ReportToThingSpeak] [bit] NULL,
	[ThingSpeakChannelNumber] [bigint] NULL,
	[ThingSpeakWriteApiKey] [nvarchar](50) NULL,
	[ReportToWunderground] [bit] NULL,
	[WundergroundPwsId] [nvarchar](50) NULL,
	[WundergroundPwsPassword] [nvarchar](50) NULL,
	[ReportToAzure] [bit] NULL,
	[SleepInterval] [int] NULL,
PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO


