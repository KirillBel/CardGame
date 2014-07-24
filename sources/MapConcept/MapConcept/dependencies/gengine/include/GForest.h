#pragma once

struct sTreeLeafVertex
{
	sTreeLeafVertex()
	{
	};

	sTreeLeafVertex(Vec3 pos, Vec2 uv, Vec2 offset)
	{
		this->pos=pos;
		this->uv=uv;
		this->offset=offset;
	};

	static GVertexDecl GetVertexDecl()
	{
		GVertexDecl d;
		d.addValue(VSTREAM_XYZ,VALUE_TYPE_VEC3F);
		d.addValue(VSTREAM_UV,VALUE_TYPE_VEC2F);
		d.addValue(eVertexStreamType::VSTREAM_UV,eValueType::VALUE_TYPE_VEC2F,1);
		return d;
	};

	Vec3 pos;
	Vec2 uv;
	Vec2 offset;
};

struct sForestTreeBranch
{
	sForestTreeBranch()
	{
		nLevel=0;
		length=0;
		bIsLeaf=0;
	};

	int nLevel;
	float length;
	bool bIsLeaf;
	std::vector<Vec3> points;
	std::vector<float> radiuses;
};

struct sForestTreeModel
{
	GGeometryBufferElem* pTreeBuffer;
	ColorF averageColor;

	GTexture* sprites0[4];
	GTexture* sprites1[4];
	GTexture* sprites2;
};

class GForest : public IRenderNode
{
public:
	GForest(void);
	~GForest(void);

	eRenderNodeType GetRenderNodeType() {return RNTYPE_TREE;};

	AABB GetBBox() {return boundingBox;};
	void SetBBox(AABB& bbox) {};

	void SetMaterial(IMaterial* pMat) {};
	IMaterial* GetMaterial() {return NULL;};

	void Render(sRenderParam& renderParam);

	void init(GTexture* pBranchTex, GTexture* pLeafTex,GTexture* pLeafNormTex,GTexture* pLeafSpecTex);
	void generateTree(int seed=1234, float initialRadius=3.5,Vec2 radiusCoeff=Vec2(0.6,0.4), Vec2 radiusDistribution=Vec2(0.3,0.8), Vec3 startPoint=Vec3(0,0,0), Quat direction=Quat(0,1,0,0), int maxLevel=3,float branchStartAngle=FL_PI/4, int pointCountPerBranch=10, float gravity=5.f, float length=80.f,float branchLengthCoeff=0.4, float branchStartCoeff=0.5,  int branchCount=3, int startBranchCount=8, int leavesCount = 3);
	void printStatistics();
	void draw();

protected:
	sForestTreeBranch generateBranch(Vec3 startPoint, Quat startDirection,  float initialRadius,Vec2 radiusCoeff, Vec2 radiusDistribution, int pointCount, float length, float gravity, float twist, float amplitude, float freq, float randomness);
	sForestTreeBranch generateLeaf(Vec3 startPoint, Quat startDirection, float length);
	void removeRedundantPoints(std::list<Vec3>& points,std::list<float>& radiuses, float coeff=0.99); 
	void reqGenerateTree(std::vector<sForestTreeBranch>& lineList,float initialRadius,Vec2 radiusCoeff, Vec2 radiusDistribution, Vec3 startPoint, Quat direction, int nLevel, int maxLevel=2,float branchStartAngle=FL_PI/5, int pointCountPerBranch=10, float gravity=2.f, float length=40.f,float branchLengthCoeff=0.5, float branchStartCoeff=0.4,  int branchCount=3, int startBranchCount=6, int leavesCount = 3);
	void addBranchVertices(std::vector<sMeshVertex>& vertices, sForestTreeBranch& branch, int radialPoints);
	void addLeafVertices(std::vector<sTreeLeafVertex>& vertices, sForestTreeBranch& leaf);
protected:
	std::vector<sForestTreeBranch> lineList;
	GGeometryBufferElem* geom;
	GGeometryBufferElem* geom1;

	AABB boundingBox;
	
	
	GTexture* pTex;
	GTexture* pLeafTex;
	GTexture* pLeafNormTex;
	GTexture* pLeafSpecTex;
	GShader* pLeafShader;

	GRenderAppearance branchApp;
	GRenderAppearance leafApp;

	bool bDrawBranches;
	bool bDrawLeaves;
	bool bDrawSkeleton;
};

