#ifndef STRICT
#define STRICT
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef OEMRESOURCE
#define OEMRESOURCE
#endif
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#include <stdio.h>
#include <string>

#include <d3dx9.h>
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include<MMSystem.h>
#include <dsound.h>
#pragma comment(lib, "dsound.lib")
#include <DxErr.h>
#pragma comment(lib, "d3d9.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "dxguid.lib")

#ifndef FW_STD_DEF_
#define FW_STD_DEF_
#endif
//#define FW_DX_DEF_
/*
#ifndef FW_STD_PROGRAMABLE_DEF_
#define FW_STD_PROGRAMABLE_DEF_
#endif
//#define FW_DX_PROGRAMABLE_DEF_
//*/
#include <FantomWaves.h>