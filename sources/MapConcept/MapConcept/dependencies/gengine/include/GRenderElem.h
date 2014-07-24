#pragma once

class GRenderElem
{
public:
	GRenderElem() 
	{
		//m_pCurrentRenderItem=NULL;
	};
	virtual ~GRenderElem() {};

	

	//void setRenderData(sRenderItem* renderItem) {m_pCurrentRenderItem=renderItem;};

	virtual bool elPreDraw(int shaderPass) {return true;};
	virtual bool elDraw(GShader* pShader, int shaderPass) {return false;};
	virtual bool elPostDraw(int shaderPass) {return false;};

	//virtual GShader* elGetOwerrideShader() {return NULL;};

	virtual AABB elGetBoundingBox() {return AABB();};
	virtual void elUpdateBoundingBox() {};
protected:
	//sRenderItem* m_pCurrentRenderItem;
};