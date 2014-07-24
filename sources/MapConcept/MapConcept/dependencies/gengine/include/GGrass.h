#pragma once

#define NUM_GRASS_LEVELS 3
#define NUM_GRASS_VARIATIONS 6
#define NUM_GRASS_IN_NEAR_CHUNK_LEVEL 10

struct sGrassVertex
{
	sGrassVertex(Vec3 pos, Vec2 uv, Vec3 offset)
	{
		this->pos=pos;
		this->uv=uv;
		this->offset=offset;
	};

	Vec3 pos;
	Vec2 uv;
	Vec3 offset;
};

struct sGrassChunk
{
	AABB bbox;

	int detailLevel;
	int variation;

	Vec2 uv_offset;
	Vec2 uv_size;

	GRenderAppearance material;
};

class GGrass : public IRenderNode
{
public:
	GGrass(void);
	~GGrass(void);

	eRenderNodeType GetRenderNodeType() {return RNTYPE_GRASS;};

	AABB GetBBox() {return AABB();};
	void SetBBox(AABB& bbox) {};

	void SetMaterial(IMaterial* pMat) {};
	IMaterial* GetMaterial() {return 0;};

	void Render(sRenderParam& renderParam);
	void reqRender(QuadTree<sGrassChunk>::iterator node_it);

	void create(GTexture* heightmap, GTexture* normals, GTexture* grassDistributionMap,GTexture* grassTexture, float terrainSize, float terrainHeight, float grassDensity);
	void release();

	void initGrassChunks(Vec2 terrainSize, int numLevels);
	void fillGrassLevels();
	void generateGrassChunk(int nLevel, GGeometryBufferElem* pBuf);
private:
	GGeometryBufferElem* grass_levels[NUM_GRASS_LEVELS][NUM_GRASS_VARIATIONS];
	GRenderAppearance grassMaterial;
	float nearLevelSize;
	int grassInChunkLevel;

	QuadTree<sGrassChunk> chunks;

	GTexture* HeightmapTex;
	GTexture* NormalsTex;
	GTexture* GrassTex;
	GTexture* ColorTex;
	GShader* grassShader;

	float GrassHeight;
	float WindStrength;
	Vec3 WindDirection;
	float TerrainSize;
	float TerrainHeight;
	
	float maxGrassDistance;
	int numGrassLevels;
};

