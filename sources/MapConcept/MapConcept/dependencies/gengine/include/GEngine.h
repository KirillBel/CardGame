#pragma once

class GEngine : public Singleton<GEngine>
{
public:
	GEngine(void);
	virtual ~GEngine(void);

	void Create(int screenWidth, int screenHeight,HWND m_hWnd, bool bWindowed, int AntiAliasLevels=0);
	void Release();

	GD3DX9Device* GetDevice() {return pDevice;};
	GRenderer* GetRenderer() {return pRenderer;};

	GResourceLoader* GetLoader() {return &loader;};
	GMaterialMgr* GetMaterialMgr() {return &matMgr;};
	GRenderSemantic* GetRenderSemantic() {return &RenderSemantic;};

	bool IsCreated();
	bool IsValid();

	void OnWindowResize(int width, int height,bool bWindowed);
	bool OnFrame();
private:
	GRenderSemantic RenderSemantic;
	GD3DX9Device* pDevice;
	GRenderer* pRenderer;
	HWND m_hWnd;
	GResourceLoader loader;
	GMaterialMgr matMgr;
};

static GEngine* Get3dEngine()
{
	return &GEngine::Instance();
};

static GD3DX9Device* Get3dDevice()
{
	return GEngine::Instance().GetDevice();
};

static GRenderer* Get3dRenderer()
{
	return GEngine::Instance().GetRenderer();
};


static GMaterialMgr* Get3dMatMgr()
{
	return GEngine::Instance().GetMaterialMgr();
};

static GRenderSemantic* Get3dSemantic()
{
	return GEngine::Instance().GetRenderSemantic();
};