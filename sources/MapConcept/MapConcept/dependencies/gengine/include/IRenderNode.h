#pragma once

struct sRenderParam
{
	UINT LightMask;
	float Distance;
	BYTE LodLevel;
	float Alpha;
	GRenderObject* pRenderObj;
};

enum eRenderNodeType
{
	RNTYPE_UNKNOWN,
	RNTYPE_MESH,
	RNTYPE_SKYBOX,
	RNTYPE_TERRAIN,
	RNTYPE_GRASS,
	RNTYPE_TREE,
	RNTYPE_LIGHT,
	RNTYPE_END
};

class IRenderNode
{
public:
	IRenderNode() 
	{
		
	};
	virtual ~IRenderNode() {};

	virtual eRenderNodeType GetRenderNodeType() {return RNTYPE_UNKNOWN;};

	virtual AABB GetBBox() = 0;
	virtual void SetBBox(AABB& bbox) = 0;

	virtual void SetMaterial(IMaterial* pMat) = 0;
	virtual IMaterial* GetMaterial() = 0;

	void SetMatrix(Matrix44& matr) {};

	virtual void Render(sRenderParam& renderParam) = 0;
};