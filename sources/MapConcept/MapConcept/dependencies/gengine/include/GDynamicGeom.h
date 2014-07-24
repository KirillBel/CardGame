#pragma once

struct sDynamicGeomRenderParam
{
	sRenderState states;
	ePrimitiveType primitive;
	int worldMatrixId;
	bool bIndexed;

	bool operator<(const sDynamicGeomRenderParam& other) const 
	{
		if(this->states.hash!=other.states.hash)
			return this->states.hash<other.states.hash;
		if(this->primitive!=other.primitive)
			return primitive<other.primitive;
		if(this->bIndexed!=other.bIndexed)
			return this->bIndexed<other.bIndexed;

		return this->worldMatrixId < other.worldMatrixId;
	};

	bool operator==(const sDynamicGeomRenderParam& other) const 
	{
		if(this->states.hash!=other.states.hash)
			return false;
		if(this->primitive!=other.primitive)
			return false;
		if(bIndexed!=other.bIndexed)
			return false;
		return this->worldMatrixId == other.worldMatrixId;
	};
};

struct sDynamicGeomBlock
{
	sDynamicGeomRenderParam param;

	UINT vertex_offs;
	UINT index_offs;
	UINT num_vert;
	UINT num_ind;
};

struct sDynamicGeomVertex
{
	Vec3 pos;
	DWORD color;
	Vec2 uv;
};

struct sDynamicGeomData
{
	sDynamicGeomData()
	{
		m_CurMatricesID=-1;
	};

	void SetMatrix(Matrix44& matr)
	{
		UINT hash=ObjectHash(matr);

		int nFound=FindMatrix(matr, hash);
		if(nFound==-1)
		{
			UsedMatrices.push_back(matr);
			UsedMatricesHash.push_back(hash);
			m_CurMatricesID=UsedMatrices.size()-1;
		}
		else
		{
			m_CurMatricesID=nFound;
		};
	};

	int FindMatrix(Matrix44& matr, UINT hash)
	{
		for(UINT i=0;i<UsedMatricesHash.size();i++)
		{
			if(UsedMatricesHash[i]==hash)
			{
				if(UsedMatrices[i]==matr)
					return i;
			};
		};
		return -1;
	};

	void Clear()
	{
		UsedMatrices.clear();
		UsedMatricesHash.clear();
		m_CurMatricesID=-1;
	};

	std::vector<Matrix44> UsedMatrices;
	std::vector<UINT> UsedMatricesHash;

	int m_CurMatricesID;
};

class GDynamicGeom
{
public:
	GDynamicGeom(void);
	~GDynamicGeom(void);

	void SetRenderStates(sRenderState& state) {currRS=state;};
	sRenderState& GetRenderState() {return currRS;};
	void SetWorldMatrix(Matrix44& mat) {data.SetMatrix(mat);};

	void DrawLine(const Vec3& v0, const ColorF& colV0, const Vec3& v1, const ColorF& colV1);
	void DrawLines(const Vec3* v, uint32 numPoints, const ColorF& col, ePrimitiveType primType=PRIM_LINESTRIP);

	void DrawTriangle(const Vec3& v0, const ColorF& colV0, const Vec3& v1, const ColorF& colV1, const Vec3& v2, const ColorF& colV2);
	void DrawTriangle(const Vec3& v0, const Vec2& tex0, const Vec3& v1, const Vec2& tex1, const Vec3& v2, const Vec2& tex2);
	void DrawTriangles(const Vec3* v, uint32 numPoints, const ColorF& col);

	void DrawQuad(const Vec3& tr, const Vec3& tl, const Vec3& br, const Vec3& bl);

	void DrawAABB(const AABB& aabb, const ColorF& col,Matrix44* pWorld=0);

	void Flush();

protected:
	void AddIndexedPrimitive( sDynamicGeomVertex*& pVertices, UINT numVertices, UINT*& pIndices, UINT numIndices, sDynamicGeomRenderParam& param );
	void AddPrimitive( sDynamicGeomVertex*& pVertices, uint32 numVertices, sDynamicGeomRenderParam& param );

	void AddBlockEntry(int numVert, int numInd, sDynamicGeomRenderParam& param);

	sDynamicGeomVertex* AllocVertexBuffer(UINT numVertices);
	UINT* AllocIndexBuffer(UINT numIndices);
	void SortBlocks();
	void CreateRenderVB();
	void CreateRenderIB();
	void FlushBlocks(UINT nStart, UINT nEnd, const sDynamicGeomRenderParam& param);

	sDynamicGeomRenderParam GetRenderParam(ePrimitiveType prim, bool bIndexed);
private:
	std::vector<sDynamicGeomBlock> blocks;
	std::vector<const sDynamicGeomBlock*> sorted_blocks;
	
	std::vector<sDynamicGeomVertex> vertices;
	UINT curVertex;
	std::vector<UINT> indices;
	UINT curIndex;

	GVertexBuffer* pRenderVB;
	GIndexBuffer* pRenderIB;
	UINT VBCounter;
	UINT IBCounter;
	UINT VertexLockFlag;
	UINT IndexLockFlag;

	sRenderState currRS;
	sDynamicGeomData data;
};

