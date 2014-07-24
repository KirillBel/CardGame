#pragma once

class GGeometryBufferElem : public GRenderElem
{
public:
	GGeometryBufferElem(void);
	~GGeometryBufferElem(void);

	///////////////////////

	void setVB(int nStream, GVertexBuffer* pVB);
	void setIB(GIndexBuffer* pIB);
	GVertexBuffer* createVB(int nStream, GVertexDecl& decl, UINT numVertices, eBufferType type,bool bWriteonly);
	GIndexBuffer* createIB(eIndexFormat format, UINT numIndices, eBufferType type, bool bWriteonly);
	void removeVB(int nStream);
	void removeIB();
	void release();

	GVertexBuffer* getVB(int nStream);
	GIndexBuffer* getIB();

	void setPrimitiveType(ePrimitiveType val) {primType=val;};
	void setNumVertices(UINT val=0) {numDrawVertices=val;};
	void setNumIndices(UINT val=0) {numDrawIndices=val;};
	void setNumPrimitives(UINT val=0) {numDrawPrimitives=val;};

	void setBoundingBox(AABB& bbox) {BoundingBox=bbox;};
	bool isValid();
	//////////MB///////////
	void update() {};

	bool elPreDraw(int shaderPass);
	bool elDraw(GShader* pShader, int shaderPass);
	bool elPostDraw(int shaderPass);
	AABB elGetBoundingBox() {return BoundingBox;};
	void elUpdateBoundingBox() {};

	UINT getSubMaterialId() {return SubMaterialId;};
	void setSubMaterialId(UINT nId) {SubMaterialId=nId;};
	////////////////////////

protected:
	void _getVertIndPrimCnt(int& numV, int& numI, int& numP);
protected:
	GVertexBuffer** VB;
	GIndexBuffer* IB;
	int minVBVertices;

	UINT SubMaterialId;
	AABB BoundingBox;

	ePrimitiveType primType;
	UINT numDrawVertices;
	UINT numDrawIndices;
	UINT numDrawPrimitives;
};

