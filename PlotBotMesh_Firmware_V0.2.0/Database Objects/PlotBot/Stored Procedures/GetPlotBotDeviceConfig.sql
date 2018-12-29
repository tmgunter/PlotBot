USE PlotBot
GO

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

--DECLARE @DeviceId varchar(50) = '45002b000a47363339343638'

CREATE PROCEDURE [dbo].[GetPlotBotDeviceConfig]
@DeviceId varchar(50)
AS
BEGIN

	SELECT *
	FROM devices AS d
	WHERE d.DeviceId = @DeviceId

END
GO


