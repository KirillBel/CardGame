#pragma once
class GScreenQuadElem : public GRenderElem
{
public:
	GScreenQuadElem(void);
	~GScreenQuadElem(void);

	bool elPreDraw(int shaderPass);
	bool elDraw(GShader* pShader, int shaderPass);
	bool elPostDraw(int shaderPass);

protected:
	void updateScreenQuad(Rect4i& Rect, bool bForceUpdate=false);
	void createScreenQuad();
protected:
	GVertexBuffer* pVB;
	Rect4i oldRect;

	sScreenVertex ScreenQuad[4];
	GVertexDecl ScreenQuadVD;
};

