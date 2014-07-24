#pragma once

#define MESHBUF_ATTR_POSITION	0x1
#define MESHBUF_ATTR_TEXCOORD	0x2
#define MESHBUF_ATTR_NORMAL		0x4
#define MESHBUF_ATTR_TANGENT	0x8
#define MESHBUF_ATTR_BINORMAL	0x10
#define MESHBUF_ATTR_COLOR		0x20
#define MESHBUF_ATTR_INDEX32	0x40

class GMeshBuffer
{
public:
	GMeshBuffer(DWORD attributes) 
	{
		SetVertexDecl(attributes);
	};
	GMeshBuffer(GVertexDecl& decl, eIndexFormat fmt) 
	{
		SetVertexDecl(decl);
		SetIndexFmt(fmt);
	};
	~GMeshBuffer(void) {};

	void SetVertexDecl(DWORD attributes)
	{
		vertexDecl.clear();
		if(attributes&MESHBUF_ATTR_POSITION)
			vertexDecl.addValue(VSTREAM_XYZ,VALUE_TYPE_VEC3F);
		if(attributes&MESHBUF_ATTR_TEXCOORD)
			vertexDecl.addValue(VSTREAM_UV,VALUE_TYPE_VEC2F);
		if(attributes&MESHBUF_ATTR_NORMAL)
			vertexDecl.addValue(VSTREAM_NORMAL,VALUE_TYPE_VEC3F);
		if(attributes&MESHBUF_ATTR_TANGENT)
			vertexDecl.addValue(VSTREAM_TANGENT,VALUE_TYPE_VEC3F);
		if(attributes&MESHBUF_ATTR_BINORMAL)
			vertexDecl.addValue(VSTREAM_BINORMAL,VALUE_TYPE_VEC3F);
		if(attributes&MESHBUF_ATTR_COLOR)
			vertexDecl.addValue(VSTREAM_COLOR,VALUE_TYPE_COLORB);

		if(attributes&MESHBUF_ATTR_INDEX32)
			SetIndexFmt(INDEX_32I);
		else
			SetIndexFmt(INDEX_16I);
	};

	void SetVertexDecl(GVertexDecl& decl)
	{
		vertexDecl=decl;
	};

	void SetIndexFmt(eIndexFormat fmt)
	{
		indexFmt=fmt;
	};

	template<typename T>
	void InsertVertex(T& vertex)
	{
		if(sizeof(T)!=vertexDecl.getVertexSize())
			return;

		UINT pos=VertexData.size();
		VertexData.resize(VertexData.size()+sizeof(T));

		memcpy(&VertexData[pos],&vertex,sizeof(T));
	};

	template<typename T>
	T* GetVertex(UINT nVert)
	{
		if(sizeof(T)!=vertexDecl.getVertexSize())
			return NULL;

		return (T*)&VertexData[nVert*vertexDecl.getVertexSize()];
	};

	void Invalidate()
	{
		gb.release();
	};

	void Restore()
	{
		Update();
	};

	bool IsValid()
	{
		return gb.isValid();
	};

	UINT GetNumVertices();
	UINT GetNumIndices();

	bool HasVertexPosition();
	bool HasVertexTexCoord();
	bool HasVertexNormal();
	bool HasVertexTangent();
	bool HasVertexBinormal();
	bool HasVertexColor();

	Vec3* GetVertexPosition(UINT nVert);
	Vec2* GetVertexTexCoord(UINT nVert);
	Vec3* GetVertexNormal(UINT nVert);
	Vec3* GetVertexTangent(UINT nVert);
	Vec3* GetVertexBinormal(UINT nVert);
	ColorB* GetVertexColor(UINT nVert);

	UINT* GetIndex32(UINT nInd);
	USHORT* GetIndex16(UINT nInd);

	void InsertIndex32(UINT index);
	void InsertIndex16(USHORT index);

	/////update/////////////////

	void UpdateVertices(bool bRecalcBBox=true);
	void UpdateIndices(bool bRecalcBBox=true);
	void UpdateBBox();
	void Update();
	void CheckBuffers();
	void Release();

	////////////////////////////

	//IRenderNode/////////////
	AABB GetBBox() {return BoundingBox;};
	void SetBBox(AABB& bbox) {BoundingBox=bbox;};

	void Render(GRenderObject* pObj, IMaterial* pMat, eRIPassList pass=PASS_GENERAL, eRIList nList=RILIST_GENERAL, DWORD batchFlags=0);
	//////////////////////////

	std::vector<BYTE> IndexData;
	std::vector<BYTE> VertexData;
protected:
	GVertexDecl vertexDecl;
	eIndexFormat indexFmt;
	GGeometryBufferElem gb;
	AABB BoundingBox;
};