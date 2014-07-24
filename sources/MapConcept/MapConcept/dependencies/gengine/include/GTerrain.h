#pragma once

struct sTerrainVertex
{
	Vec3 pos;
	Vec2 uv;
};

struct sChunkCoord
{
	sChunkCoord()
	{
		level=0;
		faceNum=0;
	};
	sChunkCoord(int level_,Vec2i globalCoord_)
	{
		level=level_;
		globalCoord=globalCoord_;
		faceNum=0;
	};

	int level;
	int faceNum;
	Vec2i localCoord;
	Vec2i globalCoord;
};

struct sTerrainNode
{
	sTerrainNode()
	{
		parent=0;
		texScale=1;
		chunkScale=1;
		roughtness=0;
		memset(childs,0,4*sizeof(sTerrainNode*));
	};

	sTerrainNode* getChunk(int i)
	{
		return childs[i];
	};

	sTerrainNode* getChunk(Vec2i vec)
	{
		assert(vec.x>=0 && vec.x<2);
		assert(vec.y>=0 && vec.y<2);
		return childs[vec.y*2+vec.x];
	}

	sTerrainNode* createChunk(int i)
	{
		childs[i]=new sTerrainNode();
		createAsChild(childs[i],Vec2i(i%2,i/2));
		return childs[i];
	};

	void createAsChild(sTerrainNode* chunk, Vec2i local)
	{
		chunk->parent=this;
		chunk->coord.level=coord.level+1;
		chunk->coord.localCoord=local;
		chunk->coord.globalCoord=coord.globalCoord*2+local;
		chunk->coord.faceNum=coord.faceNum;
	};

	sTerrainNode* createChunk(Vec2i vec)
	{
		assert(vec.x>=0 && vec.x<2);
		assert(vec.y>=0 && vec.y<2);
		childs[vec.y*2+vec.x]=new sTerrainNode();
		createAsChild(childs[vec.y*2+vec.x],vec);
		return childs[vec.y*2+vec.x];
	};

	Vec2 texOffset;
	float texScale;
	Vec2 chunkOffset;
	float chunkScale;
	AABB bbox;
	sChunkCoord coord;
	float roughtness;

	GRenderAppearance mat;
	
	sTerrainNode* parent;
	sTerrainNode* childs[4];
};

class GTerrain : public IRenderNode
{
public:
	GTerrain();
	~GTerrain(void);

	eRenderNodeType GetRenderNodeType() {return RNTYPE_TERRAIN;};

	AABB GetBBox() {return terrainMesh.elGetBoundingBox();};
	void SetBBox(AABB& bbox) {};

	void SetMaterial(IMaterial* pMat) {};
	IMaterial* GetMaterial() {return 0;};

	void create(GTexture* heightmap,GTexture* color,GTexture* normal, float size, float height, const char* terrainDrawShader,const char* normalGenShader, int nodeWidth=128);
	
	sTerrainNode* getNode(int nLevel, int x, int y);
	sTerrainNode* getRoot() {return levels[0][0];};
	int getLevelDim(int nLevel);
	void createChunkBuffer(int chunkSize_);

	void Render(sRenderParam& renderParam);

	void drawChunk(sTerrainNode* pNode);
	AABB getBoundingBox();

	void release();

	static bool GenerateHeightData(GTexture* tex, float** pHeight, int* rows, int* cols);

	//void createForest(GMeshElem* pMesh, GTexture* treeTex);

protected:
	void createLevels(int nLevels);
	void reqDrawChunk(sTerrainNode* pNode);
	float calcCoeff(sTerrainNode* pNode);
private:
	int chunkSize;
	float terrainSize;
	float terrainHeight;

	GGeometryBufferElem terrainMesh;

	std::vector<std::vector<sTerrainNode*>> levels;
	float* heightmap_data;
	int heightmap_cols;
	int heightmap_rows;

	GTexture* heightmap_tex;
	GTexture* color_tex;
	GTexture* normal_tex;
	GTexture* detail_normals_tex;

	int maxLevel;

	bool bFirstRun;

	GShader* terrainDraw;
	GShader* normalGen;
};

