#pragma once

struct sDeviceDecl
{
	sDeviceDecl()
	{
		hWnd=NULL;
		width=0;
		height=0;
		bWindowed=true;
		antiAliasLevels=0;
	};

	sDeviceDecl(HWND hWnd,UINT width,UINT height,bool bWindowed,int antiAliasLevels)
	{
		this->hWnd=hWnd;
		this->width=width;
		this->height=height;
		this->bWindowed=bWindowed;
		this->antiAliasLevels=antiAliasLevels;
	};

	HWND hWnd;
	UINT width;
	UINT height;
	bool bWindowed;
	int antiAliasLevels;
};

class GVertexBuffer;
class GIndexBuffer;

struct sStreamInfo
{
	sStreamInfo()
	{
		pStream=0;
		nOffset=0;
		nFreq=0;
	};
	GVertexBuffer *pStream;
	int nOffset;
	int nFreq;
};

struct sScreenVertex
{
	Vec3 pos;
	Vec2 uv;
};


class GDeviceContext
{
public:
	GDeviceContext(UINT MaxVertexStreams, UINT MaxColorTargets, UINT MaxTextures) 
	{
		numStreams=MaxVertexStreams;
		numColorTargets=MaxColorTargets;
		numTextures=MaxTextures;

		pIndexStream=NULL;

		pVertexStreams=new sStreamInfo[numStreams];
		pVertexDecls=new GVertexDecl[numStreams];
		ppTextures=new GTexture*[numTextures];
		ColorTargetStack=new std::list<GTexture*>[numColorTargets];

		memset(ppTextures,0,numTextures*sizeof(GTexture*));
	};

	~GDeviceContext()
	{
		SAFE_DELETE_ARRAY(pVertexStreams);
		SAFE_DELETE_ARRAY(pVertexDecls);
		SAFE_DELETE_ARRAY(ppTextures);
		SAFE_DELETE_ARRAY(ColorTargetStack);
	};

	Render::sRenderState currentRS;
	Render::sRenderState defaultRS;
	ColorF clearColor;

	Matrix44 worldMatrix;
	Matrix44 viewMatrix;
	Matrix44 projectionMatrix;

	Transform currCameraTransform;
	float fCameraNear;
	float fCameraFar;

	GVertexDecl* getVertexDecl(UINT nStream);
	bool setVertexDecl(UINT nStream,GVertexDecl& decl);

	sStreamInfo* getVertexStream(UINT nStream);
	bool setVertexStream(UINT nStream, GVertexBuffer *pStream,int nOffset,int nFreq);
	GIndexBuffer* getIndexStream() {return pIndexStream;};
	void setIndexStream(GIndexBuffer *pStream);

	GTexture* getTexture(UINT nSlot);
	bool setTexture(UINT nSlot, GTexture* pTexture);

	GTexture* getColorTarget(UINT nSlot);
	bool setColorTarget(UINT nSlot,GTexture* pTex);

	GTexture* getDepthTarget();
	bool setDepthTarget(GTexture* pTex);

	std::list<GTexture*>& getColorTargetStack(UINT nSlot);
	std::list<GTexture*>& getDepthTargetStack();

	GVertexDecl* getVertexDeclarations() {return pVertexDecls;};

	UINT getNumVertexStreams() {return numStreams;};
	UINT getNumColorTargets() {return numColorTargets;};
	UINT getNumTextureSlots() {return numTextures;};

protected:
	sStreamInfo* pVertexStreams;
	GVertexDecl* pVertexDecls;
	UINT numStreams;
	UINT numColorTargets;
	UINT numTextures;

	GIndexBuffer *pIndexStream;

	GTexture** ppTextures;
	std::list<GTexture*> *ColorTargetStack;
	std::list<GTexture*> DepthTargetStack;
};

#define TEX_FILTERCAPS_MIN_POINT           0x00000100L /* Min Filter */
#define TEX_FILTERCAPS_MIN_LINEAR          0x00000200L
#define TEX_FILTERCAPS_MIN_ANISOTROPIC     0x00000400L
#define TEX_FILTERCAPS_MIN_PYRAMIDALQUAD   0x00000800L
#define TEX_FILTERCAPS_MIN_GAUSSIANQUAD    0x00001000L
#define TEX_FILTERCAPS_MIP_POINT           0x00010000L /* Mip Filter */
#define TEX_FILTERCAPS_MIP_LINEAR          0x00020000L

#define TEX_FILTERCAPS_MAG_POINT           0x01000000L /* Mag Filter */
#define TEX_FILTERCAPS_MAG_LINEAR          0x02000000L
#define TEX_FILTERCAPS_MAG_ANISOTROPIC     0x04000000L
#define TEX_FILTERCAPS_MAG_PYRAMIDALQUAD   0x08000000L
#define TEX_FILTERCAPS_MAG_GAUSSIANQUAD    0x10000000L

#define TEX_ADDRESSCAPS_WRAP           0x00000001L
#define TEX_ADDRESSCAPS_MIRROR         0x00000002L
#define TEX_ADDRESSCAPS_CLAMP          0x00000004L
#define TEX_ADDRESSCAPS_BORDER         0x00000008L
#define TEX_ADDRESSCAPS_INDEPENDENTUV  0x00000010L
#define TEX_ADDRESSCAPS_MIRRORONCE     0x00000020L

struct sDeviceCaps
{
	UINT MaxTextureWidth;
	UINT MaxTextureHeight;
	UINT MaxVolumeTextureExtent;
	UINT MaxTextureAspectRatio;
	UINT MaxSimultaneousTextures;

	UINT MaxPrimitiveCount;
	UINT MaxVertexIndex;
	UINT MaxVertexStreams;
	UINT MaxVertexStreamStride;

	UINT MaxSimultaneousRTs;

	UINT VertexShaderVersion;
	UINT PixelShaderVersion;

	bool bSupportCubeMap;
	bool bSupportVolumeMap;
	bool bSupportMipMap;
	bool bSupportMipMapVolume;
	bool bSupportMipMapCube;

	DWORD TextureFilterCaps;
	DWORD TextureFilterCaps_Volume;
	DWORD TextureFilterCaps_Cube;

	DWORD TextureAddressCaps;
	DWORD TextureAddressCaps_Volume;

	UINT MaxAnisotropy;

	std::string toString();
};

inline int GetPrimitiveCount(int NumVerts, ePrimitiveType Type)
{
	switch(Type)
	{
	case PRIM_POINTLIST: return NumVerts;
	case PRIM_LINELIST: return NumVerts/2;
	case PRIM_LINESTRIP: return NumVerts-1;
	case PRIM_TRIANGLELIST: return NumVerts/3;
	case PRIM_TRIANGLESTRIP: return NumVerts-2;
	case PRIM_TRIANGLEFAN: return NumVerts-2;
	}
	return 0;
};

struct GRenderVariables
{
	UINT Width;
	UINT Height;
	UINT FPSUpdateTime;
	double CurrentFPS;
	sDeviceCaps Caps;
};

extern GRenderVariables sRendVars;