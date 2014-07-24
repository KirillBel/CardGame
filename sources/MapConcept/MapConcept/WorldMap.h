#pragma once

struct HexData
{
	HexData()
	{
		average=0;
		count=0;
		height=0;
	};

	float average;
	int count;
	float height;
};

struct MapVertex
{
	MapVertex(Vec3 pos,Vec2 uv,Vec3 normal)
	{
		this->pos=pos;
		this->uv=uv;
		this->normal=normal;
	};

	Vec3 pos;
	Vec2 uv;
	Vec3 normal;

	static GVertexDecl GetVD()
	{
		GVertexDecl decl;
		decl.addValue(eVertexStreamType::VSTREAM_XYZ,eValueType::VALUE_TYPE_VEC3F);
		decl.addValue(eVertexStreamType::VSTREAM_UV,eValueType::VALUE_TYPE_VEC2F);
		decl.addValue(eVertexStreamType::VSTREAM_NORMAL,eValueType::VALUE_TYPE_VEC3F);
		return decl;
	};
};

class WorldMap
{
public:
	WorldMap(void);
	~WorldMap(void);

	bool load(GTexture* pHM);
	void generate_geometry();
	void draw();
protected:
	HexagonalMap<HexData> hexmap;

	GGeometryBufferElem* pGeometry;
	GShader* pScreenShader;
	GShader* pSSAOShader;
	GShader* pGaussShader;
	GShader* pFinalShader;
	GRenderAppearance gaussApp;

	GTexture* pScreenColor;
	GTexture* pScreenNormal;
	GTexture* pScreenDepth;
	GTexture* pScreenSSAO;
	GTexture* pSSAOrnm;
	GTexture* pTempTex;
};

