#pragma once

struct sMeshVertex
{
	sMeshVertex()
	{
	};

	sMeshVertex(Vec3 Pos_,Vec3 Normal_,Vec2 UV_)
	{
		Pos=Pos_;
		UV=UV_;
		Normal=Normal_;
	};

	static GVertexDecl GetVertexDecl()
	{
		GVertexDecl d;
		d.addValue(VSTREAM_XYZ,VALUE_TYPE_VEC3F);
		d.addValue(VSTREAM_UV,VALUE_TYPE_VEC2F);
		d.addValue(VSTREAM_NORMAL,VALUE_TYPE_VEC3F);
		return d;
	};

	Vec3 Pos;
	Vec2 UV;
	Vec3 Normal;
};



class GMesh : public GResource, public IRenderNode
{
public:
	GMesh(void);
	~GMesh(void);

	static GMesh* CreateMesh(const char* name);
	static GMesh* LoadMesh(const char* path,bool bAsyncLoad=true);

	static GMesh* GetMesh(const char* name);
	static GMesh* GetMesh(RCID id);

	static void CreateBox(std::vector<sMeshVertex>& vertices,std::vector<SHORT>& indices, float sizeX, float sizeY, float sizeZ);
	static GMeshBuffer* CreateBox(float sizeX, float sizeY, float sizeZ);

	bool Load(bool bAsyncLoad=false);
	bool Load(const char* path,bool bAsyncLoad=false);

	//Mesh specific
	UINT getMeshBufferCount() const;
	GMeshBuffer* getMeshBuffer(UINT Index);
	void addMeshBuffer(GMeshBuffer* buf);
	void removeMeshBuffer(uint32 index);
	void removeAllMeshBuffers();
	void recalculateBoundingBox();
	void updateMeshBuffers();
	void renderMesh(GRenderObject* pObj,eRIPassList pass=PASS_GENERAL, eRIList nList=RILIST_GENERAL, DWORD batchFlags=0);
	//AABB getBoundingBox();
	void updateBoundingBox();

	


	//IRenderNode
	eRenderNodeType GetRenderNodeType() {return RNTYPE_MESH;};
	virtual AABB GetBBox() {return bounding_box;};
	virtual void SetBBox(AABB& bbox) {bounding_box=bbox;};
	void SetMaterial(IMaterial* pMat) {pMaterial=pMat;};
	IMaterial* GetMaterial() {return pMaterial;};
	void Render(sRenderParam& renderParam);

	//Resource specific
	const char* GetName() const {return meshName.c_str();};
	const char* GetClassNameStr() const;
	int Release();
	void Invalidate();
	bool Restore();
	bool IsValid() const;
	GResource* Clone(const char* newName);
	std::string toString() const;

protected:
	static GMesh* CreateMeshObject(const char* name, bool& bFound);
protected:
	bool bLoaded;
	bool bAsyncLoad;
	std::string meshName;
	std::string SrcFile;

	IMaterial* pMaterial;
	std::vector<GMeshBuffer*> MeshBuffers;
	AABB bounding_box;
};

