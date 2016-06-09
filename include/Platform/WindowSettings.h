#ifndef BH_WINDOWSETTINGS_H
#define BH_WINDOWSETTINGS_H

#include "Platform/Typedefs.h"

#include <windows.h>

namespace BH
{
	struct WindowSettings
	{

		String title = "GameWindow";

		u32 x = 0;
		u32 y = 0;

		u32 w = 800; 
		u32 h = 600;

		u32 min_w = 800; 
		u32 min_h = 600;

		u32 max_w = 1920; 
		u32 max_h = 1080;

		u32 flags = WS_OVERLAPPEDWINDOW;

	};

}


#endif