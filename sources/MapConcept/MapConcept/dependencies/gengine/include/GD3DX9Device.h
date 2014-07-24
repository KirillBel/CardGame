#pragma once


class GCamera;

class GDevice
{
public:
	GDevice() {};
	virtual ~GDevice() {};
};

class GD3DX9Device : public GDevice
{
public:
	GD3DX9Device(void);
	virtual ~GD3DX9Device(void);

	bool init(sDeviceDecl& decl);
	void release();
	bool resize(int newWidth, int newHeight, bool bWindowed);
	bool restart();
	bool isDeviceLost() {return bDeviceLost;};
	bool recoverLostDevice();

	sDeviceCaps getDeviceCaps();

//---------------------Render states-----------------------------
	GDeviceContext* createDeviceContext();
	void deleteDeviceContext(GDeviceContext* pContext);
	GDeviceContext* getDeviceContext();
	bool setDeviceContext(GDeviceContext* pContext);

	void setFillMode(eFillMode Mode, bool bForceUpdate=false);
	void setCullMode(eCullMode Mode, bool bForceUpdate=false);
	void setAlphablendMode(eBlendMode Src, eBlendMode Dst, bool bForceUpdate=false);
	void enableZBuffer(eBoolMode bVal, bool bForceUpdate=false);
	void enableZWrite(eBoolMode bVal, bool bForceUpdate=false);
	void enableAlphaBlending(eBoolMode bVal, bool bForceUpdate=false);

	void setClearColor(ColorF& col);

	ColorF getClearColor();
	eFillMode getFillMode();
	eCullMode getCullMode();
	eBoolMode isZBufferEnabled();
	eBoolMode isZWriteEnabled();
	eBoolMode isAlphablendEnabled();
	eBlendMode getBlendSrcMode();
	eBlendMode getBlendDstMode();

	Render::sRenderState& getRenderStates();
	Render::sRenderState& getDefaultRenderStates();
	void setRenderStates(const Render::sRenderState& rs, bool bForceUpdate=false);
	void setDefaultRenderStates(Render::sRenderState& rs, bool bForceUpdate=false);
	void setRenderStatesToDefault(bool bForceUpdate);
	void updateRenderStates();
//---------------------End render states-----------------------------

//-----------------------Transforms----------------------------------
	void setWorldMatrix(Vec3 Pos, Ang3 Rot=Ang3(0,0,0), Vec3 Scale=Vec3(1,1,1));
	void setWorldMatrix(const Matrix44& Matr);
	void setWorldTransform(const Transform& val);
	void setViewMatrix(const Matrix44& Matr);
	void setCameraTransform(GCamera* pCamera);

	Transform getCameraTransform();
	float getCameraNear();
	float getCameraFar();

	void setProjectionMatrix(const Matrix44& Matr);
	Matrix44 getWorldMatrix();
	Matrix44 getViewMatrix();
	Matrix44 getProjectionMatrix();
	Matrix44 getWVPMatrix();

	void getViewMatrixData(Vec3* pos,Vec3* look=NULL,Vec3* up=NULL,Vec3* right=NULL);
//-----------------------End transforms-------------------------------
//------------------------Draw----------------------------------------
	bool beginFrame(DWORD dwClearFlags);
	bool endFrame();
	void clear(DWORD dwClearFlags);

	void setVertexDeclaration(GVertexDecl& decl, UINT nStream=0);
	void updateVertexDeclaration();

	bool setVertexStream(int nID,  GVertexBuffer* pVB, UINT nOffs=0);
	bool setIndexStream(GIndexBuffer* pIB);
	//bool setDeviceVertexStream(int nID, int vertSize, GDeviceVertexBuffer* pVB, UINT nOffs=0);
	//bool setDeviceIndexStream(GDeviceIndexBuffer* pIB);

	void drawIndexedPrimitive(ePrimitiveType pt, UINT NumVertices, UINT NumIndices,int BaseVertexIndex=0, UINT MinVertIndex=0,UINT StartIndex=0, UINT primitiveCount=0);
	void drawPrimitive(ePrimitiveType pt, UINT StartVertex, UINT PrimitiveCount);
	void drawVertexPrimitiveList(UINT vertexCount,UINT primitiveCount, ePrimitiveType pt,void* vertices=0,UINT vertexSize=0,void* indices=0,eIndexFormat indexFmt=INDEX_32I);

	void drawText(const char* Str, Vec2i xy, ColorF Color);
	void drawScreenQuad();
	void updateScreenQuad(Rect4i& Rect);
	void setTexture(UINT nSlot, GTexture* pTexture);
	/*void setDeviceTexture(UINT nSlot, GDeviceTexture* pTexture);*/

	bool setRenderTarget(UINT nTarget, GTexture* pTexture);
	bool setDepthTarget(GTexture* pTexture);
	bool pushRenderTarget(UINT nTarget, GTexture* pTexture);
	bool pushDepthTarget(GTexture* pTexture);
	bool popRenderTarget(UINT nTarget);
	bool popDepthTarget();
//-----------------------------------End draw-------------------------------------
//----------------------------------Begin viewport--------------------------------
	Vec2i getFrameSize();
	Vec2i getViewportSize();
	Rect4i getViewportRect();
//----------------------------------End viewport----------------------------------

//---------------------NULL Only---------------------------------------
	void updateFPS();
	double getFPS() {return FramesPerSec;};
	UINT getDIPCount() {return DIPCount;};
//---------------------DX Only---------------------------------------
	//GDeviceContext* getDeviceContextDx();
	static LPDIRECT3DDEVICE9EX GetBaseD3DXDevice() {return pD3DDevice;};

	void CreateDeviceFont();
	void ReleaseDeviceFont();
	void SetInitialDeviceStates();
	void ReleaseDeviceResources();
	void CreateDeviceResources();
	void UpdateDefaultDeviceRT();

	LPDIRECT3DSURFACE9 GetTextureSurface(GDeviceTexture* pTexture, int nLevel);
	void StretchRT(GTexture* pSrc,GTexture* pDst);
	//void InvalidateResourcesInVideoMemory();
	//void RestoreResourcesInVideoMemory();

protected:
	void __SetDeviceContext(GDeviceContext* pContext);
	void __UpdateDeviceContext();
	void __ResetRT();

protected:

	//null
	UINT DIPCount;
	UINT FPSFrameCnt;
	DWORD PrevFPSUpdateTime;
	double FramesPerSec;
	Vec2i frameSize;
	sScreenVertex ScreenQuad[4];
	GVertexDecl ScreenQuadVD;

	//dx
	HWND m_hWnd;
	static LPDIRECT3DDEVICE9EX pD3DDevice;
	LPDIRECT3D9EX pD3D;
	D3DPRESENT_PARAMETERS d3dpp;
	ID3DXFont* pD3DFont;
	IDirect3DVertexDeclaration9 *deviceVBDecl;
	LPDIRECT3DSURFACE9 DefaultRTSurface;
	LPDIRECT3DSURFACE9 DefaultDepthSurface;
	bool bDeviceLost;

	GDeviceContext* pDefaultDeviceContext;
	GDeviceContext* pCurrentDeviceContext;
};

/*class GD3DX9Device
{
public:
	GD3DX9Device(void);
	virtual ~GD3DX9Device(void);

	////////////DEVICE//////////////////////
	bool init(sDeviceDecl& decl);
	void release();
	bool resize(int newWidth, int newHeight, bool bWindowed);
	bool restart();
	bool isDeviceLost() {return bDeviceLost;};
	bool recoverLostDevice();

	void StoreResources();
	void RestoreResources();

	/////RENDER STATES////
	//GDeviceContext* createDeviceContext();
	//GD3DX9DeviceContext* getDeviceContextDx();
	//GDeviceContext* getDeviceContext();
	//bool pushDeviceContext(GDeviceContext* pContext);
	//bool popDeviceContext();
	//bool setDeviceContext(GDeviceContext* pContext);

	//void setFillMode(eFillMode Mode, bool bForceUpdate=false);
	//void setCullMode(eCullMode Mode, bool bForceUpdate=false);
	//void setAlphablendMode(eBlendMode Src, eBlendMode Dst, bool bForceUpdate=false);
	//void enableZBuffer(eBoolMode bVal, bool bForceUpdate=false);
	//void enableZWrite(eBoolMode bVal, bool bForceUpdate=false);
	//void enableAlphaBlending(eBoolMode bVal, bool bForceUpdate=false);

	//ColorF getClearColor();
	//eFillMode getFillMode();
	//eCullMode getCullMode();
	//eBoolMode isZBufferEnabled();
	//eBoolMode isZWriteEnabled();
	//eBoolMode isAlphablendEnabled();
	//eBlendMode getBlendSrcMode();
	//eBlendMode getBlendDstMode();

	//Render::sRenderState& getRenderStates();
	//Render::sRenderState& getDefaultRenderStates();
	//void setRenderStates(const Render::sRenderState& rs, bool bForceUpdate);
	//void setDefaultRenderStates(Render::sRenderState& rs, bool bForceUpdate);
	//void setRenderStatesToDefault(bool bForceUpdate);
	//void updateRenderStates();

	///////////TRANSFORMS///////////////////////
	void setWorldMatrix(Vec3 Pos, Ang3 Rot=Ang3(0,0,0), Vec3 Scale=Vec3(1,1,1));
	void setWorldMatrix(const Matrix44& Matr);
	void setWorldTransform(const Transform& val);
	void setViewMatrix(const Matrix44& Matr);
	//void setCameraTransform(GCamera* pCamera);
	Transform getCameraTransform() {return getDeviceContextDx()->currCameraTransform;};
	float getCameraNear() {return getDeviceContextDx()->fCameraNear;};
	float getCameraFar() {return getDeviceContextDx()->fCameraFar;};

	void setProjectionMatrix(const Matrix44& Matr);
	Matrix44 getWorldMatrix();
	Matrix44 getViewMatrix();
	Matrix44 getProjectionMatrix();
	Matrix44 getWVPMatrix();

	void getViewMatrixData(Vec3* pos,Vec3* look=NULL,Vec3* up=NULL,Vec3* right=NULL);

	////////////DRAW////////////////////////
	bool beginFrame(DWORD dwClearFlags);
	bool endFrame();
	void clear(DWORD dwClearFlags);

	void setVertexDeclaration(GVertexDecl& decl, UINT nStream=0);
	void updateVertexDeclaration();
	void drawBuffer(GVertexBuffer* pVB, ePrimitiveType pt, UINT vbSizeVert=0, UINT vbOffsetVert=0, GIndexBuffer* pIB=NULL, UINT ibSizeInd=0, UINT ibOffsetInd=0);
	void drawText(const char* Str, Vec2 xy, ColorF Color);
	void drawScreenQuad();
	void updateScreenQuad(Rect4& Rect);
	void setTexture(UINT nSlot, GTexture* pTexture);

	bool setVertexStream(int nID, GVertexBuffer* pVB, UINT nOffs=0);
	bool setIndexStream(GIndexBuffer* pIB);
	void drawIndexedPrimitive(ePrimitiveType pt, UINT NumVertices, UINT NumIndices,int BaseVertexIndex=0, UINT MinVertIndex=0,UINT StartIndex=0, UINT primitiveCount=0);
	void drawVertexPrimitiveList(UINT vertexCount,UINT primitiveCount, ePrimitiveType pt,void* vertices=0,UINT vertexSize=0,void* indices=0,eIndexFormat indexFmt=INDEX_32I);
	void drawPrimitive(ePrimitiveType pt, UINT StartVertex, UINT PrimitiveCount);

	bool setRenderTarget(UINT nTarget, GTexture* pTexture);
	bool setDepthStencilTarget(GTexture* pTexture);
	bool pushRenderTarget(UINT nTarget, GTexture* pTexture);
	bool pushDepthStencilTarget(GTexture* pTexture);
	bool popRenderTarget(UINT nTarget);
	bool popDepthStencilTarget();
	//////////VIEWPORT//////////////////////
	Vec2i getFrameSize();
	Vec2i getViewportSize();
	Rect4 getViewportRect();
	

	////////////DEVICE DEPENDENT//////////////
	static LPDIRECT3DDEVICE9EX GetBaseD3DXDevice() {return pD3DDevice;};
	void CreateDeviceFont();
	void ReleaseDeviceFont();
	void SetInitialDeviceStates();
	void ReleaseDeviceResources();
	void CreateDeviceResources();
	void UpdateDefaultDeviceRT();

	LPDIRECT3DSURFACE9 GetTextureSurface(GTexture* pTexture, int nLevel);


protected:
	void __SetDeviceContext(GD3DX9DeviceContext* pDxContext);
	void __UpdateDeviceContext();
	void __ResetRT();
private:
	//HWND m_hWnd;
	//static LPDIRECT3DDEVICE9EX pD3DDevice;
	//LPDIRECT3D9EX pD3D;
	//D3DPRESENT_PARAMETERS d3dpp;

	//ID3DXFont* pD3DFont;
	//IDirect3DVertexDeclaration9 *deviceVBDecl;
	///*IDirect3DVertexDeclaration9 *deviceVBDecl;*/

	//LPDIRECT3DSURFACE9 DefaultRTSurface;
	//LPDIRECT3DSURFACE9 DefaultDepthSurface;
	//bool bDeviceLost;

	/*sStreamInfo VertexStreams[MAX_STREAMS];
	void *pIndexStream;

	std::list<LPDIRECT3DSURFACE9> RenderTargetStack[4];
	std::list<LPDIRECT3DSURFACE9> DepthTargetStack;

	Transform currCameraTransform;
	float fCameraNear;
	float fCameraFar;*/

	/*GD3DX9DeviceContext defaultDeviceContext;
	GD3DX9DeviceContext currentDeviceContext;
};*/

