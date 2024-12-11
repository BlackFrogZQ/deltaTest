#pragma once

namespace TIGER_CameraDef
{
	struct CCameraPara
	{
		int exposureTime;
		int heartbeatTime;
		QString ip;
		CCameraPara::CCameraPara()
		{
			exposureTime = 40000;
			heartbeatTime = 5000;
			ip = "169.254.169.99";
		};
	};
	CCameraPara *cameraParas();
};