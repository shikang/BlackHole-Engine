#ifndef BH_EDITOR_CONFIG_H
#define BH_EDITOR_CONFIG_H

#include "Core/CoreConfig.h"
#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

#ifdef BH_DEBUG
#pragma comment (lib, "../Lib/CEGUIBase-0_d.lib")
#pragma comment (lib, "../Lib/CEGUICommonDialogs-0_d.lib")
#pragma comment (lib, "../Lib/CEGUICoreWindowRendererSet_d.lib")
#pragma comment (lib, "../Lib/CEGUIDirect3D11Renderer-0_d.lib")
#pragma comment (lib, "../Lib/CEGUIExpatParser_d.lib")
#pragma comment (lib, "../Lib/CEGUILuaScriptModule-0_d.lib")
#pragma comment (lib, "../Lib/CEGUISILLYImageCodec_d.lib")
#pragma comment (lib, "../Lib/freetype_d.lib")
#pragma comment (lib, "../Lib/pcre_d.lib")
#pragma comment (lib, "../Lib/expat_d.lib")
#pragma comment (lib, "../Lib/jpeg_d.lib")
#pragma comment (lib, "../Lib/libpng_d.lib")
#pragma comment (lib, "../Lib/lua_d.lib")
#pragma comment (lib, "../Lib/SILLY_d.lib")
#pragma comment (lib, "../Lib/zlib_d.lib")
#else
#pragma comment (lib, "../Lib/CEGUIBase-0.lib")
#pragma comment (lib, "../Lib/CEGUICommonDialogs-0.lib")
#pragma comment (lib, "../Lib/CEGUICoreWindowRendererSet.lib")
#pragma comment (lib, "../Lib/CEGUIDirect3D11Renderer-0.lib")
#pragma comment (lib, "../Lib/CEGUIExpatParser.lib")
#pragma comment (lib, "../Lib/CEGUILuaScriptModule-0.lib")
#pragma comment (lib, "../Lib/CEGUISILLYImageCodec.lib")
#pragma comment (lib, "../Lib/freetype.lib")
#pragma comment (lib, "../Lib/pcre.lib")
#pragma comment (lib, "../Lib/expat.lib")
#pragma comment (lib, "../Lib/jpeg.lib")
#pragma comment (lib, "../Lib/libpng.lib")
#pragma comment (lib, "../Lib/lua.lib")
#pragma comment (lib, "../Lib/SILLY.lib")
#pragma comment (lib, "../Lib/zlib.lib")
#endif	// BH_DEBUG

#endif	// GAME_EDITOR_MODE

#endif