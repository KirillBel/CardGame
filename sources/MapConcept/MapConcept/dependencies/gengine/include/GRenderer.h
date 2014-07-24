#pragma once


enum eRenderMode
{
	RENDER_MODE_IMMEDIATE,
	RENDER_MODE_PIPELINE
};

class IMaterial;
class GRenderer
{
public:
	GRenderer(void);
	~GRenderer(void);

	static void DrawROPs_Base(GShader* pShader, UINT pass, std::list<sRenderOp*>& oneShaderROPs);
	static void DrawROPs_Shader(GShader* pCurShader,int nShaderTechnique, std::list<sRenderOp*>& oneShaderRI);

	virtual void Release();

	virtual void ClearBuffers();
	virtual void SortScene();
	virtual void RenderScene();

	virtual void DrawRenderList(eRIPassList pass, eRIList nList);
	virtual void SortRenderList(eRIPassList pass, eRIList nList);

	eRenderMode SetRenderMode(eRenderMode mode);
	void Render(GRenderElem* pElem, GRenderObject* pObj, GRenderAppearance* pRA, eRIPassList pass, eRIList nList, DWORD batchFlags);
	void Render(GRenderElem* pElem, GRenderObject* pObj, IMaterial* pMat, eRIPassList pass, eRIList nList, DWORD batchFlags);
	void DrawImmediate(GRenderElem* pElem, GRenderObject* pObj, GRenderAppearance* pRA);

	void EnableSorting(bool bVal) {bEnableSorting=bVal;};
	bool isSortingEnabled() {return bEnableSorting;};

	GRenderObject* GetTempRenderObject();
	void DiscardTempRenderObject(GRenderObject* pObj);
	GDynamicGeom& GetDynamicGeomBuffer() {return dynamicGeometry;};

	GCamera* pActiveCamera;
protected:
	void DiscardAllTempRenderObjects();
protected:
	eRenderMode renderMode;
	GRenderPipeline pipeline;
	GDynamicGeom dynamicGeometry;
	GScreenQuadElem screen_quad;
	bool bEnableSorting;

	std::list<GRenderObject*> NotUsedTempObjects;
	std::vector<GRenderObject*> TempObjects;

	/*std::vector<IRenderLight*> lights;
	std::vector<IRenderLight*> affected_lights;*/
};

