#pragma once

#pragma warning (disable : 4251)
#include <d3d11.h>

#include "SimpleMath.h"
//#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <Effect11\d3dx11effect.h>
#include <DirectXTK\DDSTextureLoader.h>
#include <DirectXTK\WICTextureLoader.h>
using namespace DirectX;
using namespace SimpleMath;

#include <array>
#include <iostream>
#include <vector> /* �迭 */
#include <list> /* ����ũ�帮��Ʈ */
#include <map> /* ����Ʈ�� */
#include <unordered_map> /* �ؽ����̺� */
#include <algorithm>

#include "Engine_Typedef.h"
#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"

// WIN
#include <windows.h>
#include <assert.h>
#include <optional>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 

#endif

#endif // _DEBUG


using namespace Engine;



using namespace std;