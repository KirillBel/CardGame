#pragma once

enum eMaterialFrontendType
{
	MF_TYPE_NONE=0,
	MF_TYPE_MATERIAL,
	MF_TYPE_PASS,
};

enum eMaterialValueDest
{
	MV_DEST_NONE=0,
	MV_DEST_SHADER_VALUE,
	MV_DEST_TEXTURE_SLOT,
	MV_DEST_COLOR_TARGET_SLOT,
	MV_DEST_DEPTH_TARGET,
};

struct sMaterialValue
{
	sMaterialValue(Value& val,UINT nSlot, eMaterialValueDest value_dest,eRenderSemantic semantic_id,eMaterialFrontendType frontend_type,const char* frontend_name)
	{
		this->val=val;
		this->value_dest=value_dest;
		this->frontend_type=frontend_type;
		this->frontend_name=frontend_name;
		this->semantic_id=semantic_id;
		this->nSlot=nSlot;
	};

	sMaterialValue()
	{
		value_dest=MV_DEST_NONE;
		frontend_type=MF_TYPE_NONE;
		nSlot=0;
		semantic_id=SEMANTIC_NONE;
	};

	Value val;
	eMaterialValueDest value_dest;
	eMaterialFrontendType frontend_type;
	std::string frontend_name;
	eRenderSemantic semantic_id;
	UINT nSlot;
};

struct sMaterialPassDecl
{
	sMaterialPassDecl()
	{
		pShader=NULL;
		ShaderPassMask=0xFFFFFFFF;
		ShaderTechnique=0;
		bDrawScreenQuad=false;
	}
	std::string Name;

	GShader* pShader;
	UINT ShaderPassMask;
	int ShaderTechnique;

	bool bDrawScreenQuad;

	sRenderState RenderStates;

	void InsertValue(sMaterialValue& val);
	void InsertShaderValue(Value& val,eRenderSemantic semantic=SEMANTIC_NONE,eMaterialFrontendType frontend=MF_TYPE_NONE,const char* frontend_name=NULL);
	void InsertTexture(GTexture* pTex,UINT nSlot, eRenderSemantic semantic=SEMANTIC_NONE,eMaterialFrontendType frontend=MF_TYPE_NONE,const char* frontend_name=NULL);
	void InsertColorTarget(GTexture* pTex,UINT nSlot, eRenderSemantic semantic=SEMANTIC_NONE,eMaterialFrontendType frontend=MF_TYPE_NONE,const char* frontend_name=NULL);
	void InsertDepthTarget(GTexture* pTex, eRenderSemantic semantic=SEMANTIC_NONE,eMaterialFrontendType frontend=MF_TYPE_NONE,const char* frontend_name=NULL);

	std::vector<sMaterialValue> material_values;
};

class GMaterial;
class GMaterialContext
{
public:
	GMaterialContext() 
	{
		pMaterial=NULL;
	};
	~GMaterialContext(void) {};

	bool SetSemantic(const char* name, ValueData& val);
	bool SetSemantic(eRenderSemantic semanticId, ValueData& val);
	bool SetValue(const char* name, ValueData& val);
	void Apply();
protected:
	ValueList FrontendValues;
	ValueData SemanticValues[SEMANTIC_END];
	GMaterial* pMaterial;
};


class GMaterialPass
{
public:
	GMaterialPass(const char* name)
	{
		this->name=name;
		bDrawScreenQuad=false;
	};
	GMaterialPass(sMaterialPassDecl& decl)
	{
		CreateFromDecl(decl);
	};
	GMaterialPass(pugi::xml_node& node)
	{
		Serialize(node,false);
	};
	virtual ~GMaterialPass()
	{};

	const char* GetName() {return name.c_str();};

	void CreateFromDecl(sMaterialPassDecl& decl);
	sMaterialPassDecl GetDecl();

	bool isScreenQuadPass() {return bDrawScreenQuad;};

	void drawRE(GRenderElem* pElem, GRenderObject* pObj, bool bImmediate=false, eRIPassList pass=PASS_GENERAL, eRIList nList=RILIST_GENERAL, DWORD batchFlags=0);
	GRenderAppearance* getAppearance() {return &RenderApp;};

	bool SetSemanticValue(eRenderSemantic semanticId, ValueData& val);
	bool SetFrontendValue(const char* name, ValueData& val);
	bool SetValue(UINT valId, ValueData& val);
	void UpdateValues();

	bool Serialize(pugi::xml_node& node, bool bStore);
protected:
	bool SerializeValue(pugi::xml_node& node, bool bStore, sMaterialValue& val);
protected:
	friend class GMaterial;
	ValueList FrontendValues;
	std::list<UINT> SemanticValues[SEMANTIC_END];

	std::vector<sMaterialValue> material_values;
	GRenderAppearance RenderApp;
	std::string name;
	bool bDrawScreenQuad;
};

class GMaterial : public GBaseMaterial
{
public:
	GMaterial(const char* name) : GBaseMaterial(name)
	{
		memset(SemanticValuesMask,0,SEMANTIC_END*sizeof(UINT));
	};

	GMaterial(pugi::xml_node& node) : GBaseMaterial("")
	{
		Serialize(node,false);
	};
	virtual ~GMaterial()
	{};

	//base
	void drawRE(GRenderElem* pElem, GRenderObject* pObj, bool bImmediate=false, eRIPassList pass=PASS_GENERAL, eRIList nList=RILIST_GENERAL, DWORD batchFlags=0);

	int CreatePass(sMaterialPassDecl& decl);
	int CreatePass(pugi::xml_node& node);
	bool SetSemanticValue(eRenderSemantic semanticId, ValueData& val);
	bool SetFrontendValue(const char* name, ValueData& val);

	UINT GetPassCount();
	GMaterialPass* GetPass(UINT nPass);
	GRenderAppearance* GetPassAppearance(UINT nPass);
	bool isScreenQuadPass(UINT nPass);

	bool Serialize(pugi::xml_node& node, bool bStore);
	bool SaveXML(const char* path);
	bool LoadXML(const char* path);
protected:
	void updatePassMask(UINT nPass);
protected:
	ValueList FrontendValuesMask;
	UINT SemanticValuesMask[SEMANTIC_END];

	std::vector<GMaterialPass*> passes;
};







//class GMaterial : public GBaseMaterial
//{
//public:
//	GMaterial(sMaterialDecl& decl) : GBaseMaterial(decl.Name.c_str())
//	{
//		SemanticValues=new std::list<UINT>[SEMANTIC_END];
//		memset(SemanticValues,SEMANTIC_END*sizeof(UINT),0);
//		CreateFromDecl(decl);
//	};
//
//	GMaterial(pugi::xml_node& node) : GBaseMaterial("")
//	{
//		SemanticValues=new std::list<UINT>[SEMANTIC_END];
//		memset(SemanticValues,SEMANTIC_END*sizeof(UINT),0);
//		Serialize(node,false);
//	};
//
//	virtual ~GMaterial(void) {};
//
//
//	//base
//	void drawRE(GRenderElem* pElem, GRenderObject* pObj, bool bImmediate=false, eRIPassList pass=PASS_GENERAL, eRIList nList=RILIST_GENERAL, DWORD batchFlags=0);
//	GRenderAppearance* getAppearance() {return &RenderApp;};
//	///////////////////////
//
//	GMaterial* getPassMaterial(int id);
//
//	void SetSemantic(eRenderSemantic semanticId, ValueData& val);
//	void SetValue(const char* name, ValueData& val);
//
//	bool Serialize(pugi::xml_node& node, bool bStore);
//	bool SaveXML(const char* path);
//
//	sMaterialDecl GetMaterialDecl();
//protected:
//	int CreatePass(sMaterialDecl& decl);
//	int CreatePass(pugi::xml_node& node);
//	void CreateFromDecl(sMaterialDecl& decl);
//	bool setValue(UINT MatValId, ValueData& val);
//	void updateValues();
//	bool SerializeValue(pugi::xml_node& node, bool bStore, sMaterialValue& val);
//protected:
//	ValueList FrontendValues;
//	std::list<UINT>* SemanticValues;
//
//	std::vector<sMaterialValue> material_values;
//	std::vector<GMaterial*> passes;
//	GRenderAppearance RenderApp;
//};
//
