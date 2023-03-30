#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <process.h>

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>


#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Function.h"
#include "Typedef.h"

#pragma warning(disable : 4251)

extern HWND		 g_hWnd;
extern HINSTANCE g_hInst;

using namespace std;
using namespace Engine;
