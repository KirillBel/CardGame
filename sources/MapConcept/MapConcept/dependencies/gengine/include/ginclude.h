#pragma once


#include <worktools.h>

#include <stdio.h>
#include <tchar.h>
#include <map>
#include <io.h>


//DeviceInclude
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//DxInclude
#include <d3d9.h>
#include <d3dx9.h>


#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

//#define FREEIMAGE_LIB
#include <FreeImage.h>

#include "GSerializable.h"
#include "GFrustum.h"

#include "GResource.h"
#include "ValueList.h"
#include "GRenderFmt.h"
using namespace Render;

#include "GVertexDecl.h"

#include "GDeviceResources.h"
#include "GDeviceCommon.h"
//#include "GDevice.h"

#include "GTexture.h"
#include "GVertexBuffer.h"
#include "GIndexBuffer.h"
#include "ShaderCommon.h"
#include "GShader.h"




#include "GDynamicGeom.h"

#include "GD3DX9Resources.h"
#include "GD3DX9Common.h"
#include "GD3DX9DeviceContext.h"
#include "GD3DX9Device.h"

#include "GRenderObject.h"
#include "GRenderElem.h"
#include "GGeometryBufferElem.h"
#include "GScreenQuadElem.h"
#include "GRenderAppearance.h"
#include "GRenderPipeline.h"
#include "GRenderSemantic.h"
#include "GRenderer.h"
#include "IMaterial.h"
#include "GSimpleMaterial.h"
#include "GMaterial.h"
#include "GMaterialMgr.h"
#include "IRenderNode.h"

#include "GMeshBuffer.h"
#include "GMesh.h"
#include "GSkybox.h"
#include "GTerrain.h"
#include "GGrass.h"
#include "GForest.h"

#include "GSceneNode.h"
#include "GCamera.h"
#include "GScene.h"

#include "FreeImageFunctions.h"
#include "G3DSMeshLoader.h"
#include "GResourceLoader.h"

#include "SerializeCommon.h"
#include "GEngine.h"

#include "GInput.h"
#include "GFramework.h"

#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW  
#endif
#endif  // _DEBUG

//#if defined(_DEBUG) && defined(_CRTDBG_MAP_ALLOC) && defined(__cplusplus) && !defined(_DEBUG_NEW)
////
//inline void *__cdecl operator new(size_t _size, const char *_file_name, int _line)
//{
//  return ::operator new(_size, 1,  _file_name, _line);
//}
////
//#define _DEBUG_NEW new(__FILE__, __LINE__)
//#define new _DEBUG_NEW
//#endif
