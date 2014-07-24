#pragma once

class GSkybox : public IRenderNode
{
public:
	GSkybox(IMaterial* pMat);
	~GSkybox(void);

	eRenderNodeType GetRenderNodeType() {return RNTYPE_SKYBOX;};

	AABB GetBBox() {return AABB();};
	void SetBBox(AABB& bbox) {};

	void SetMaterial(IMaterial* pMat) {pSkyboxMat=pMat;};
	IMaterial* GetMaterial() {return pSkyboxMat;};

	void Render(sRenderParam& renderParam);

private:
	GGeometryBufferElem gb;
	IMaterial* pSkyboxMat;
};

