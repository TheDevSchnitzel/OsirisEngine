#ifndef DIRECTXUSINGS_HPP_
#define DIRECTXUSINGS_HPP_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")

#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dcompiler.h> 	

#include <D3D10_1.h>
#include <dxgi.h>
#include <dinput.h>
#include <D2D1.h>
#include <dwrite.h>

#include <directxmath.h>


#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

#endif /*DIRECTXUSING_HPP_*/
