#pragma once

struct sRenderOp;
class GShader : public GResource
{
public:
	GShader(void);
	~GShader(void);

	static GShader* CreateShader(const char* name, void* pData, UINT size);
	static GShader* LoadShader(const char* name);
	static GShader* CreateFromXML(pugi::xml_node& node);
	static GShader* GetShader(RCID id);
	static void ReloadShaders();
	
	
	void SetShader(void* pData, UINT size);
	void SetShader(GDeviceShader* pShader);
	bool Create();
	bool Load();
	bool Load(const char* path);
	GDeviceShader* GetDeviceShader() {return pDeviceShader;};

	void drawRenderItems(int nShaderTechnique, std::list<sRenderOp*>& items);

	//Shader specific
	bool applyShaderResources(sShaderResources* pResources);
	bool updateResourcesCacheBlock(sShaderResources* pResources);
	bool setDefaultValues();
	bool applyShaderSemantics();

	bool begin(UINT *pPasses);
	bool beginPass(UINT Num);
	bool endPass();
	bool end();

	bool getValueList(ValueList& params);
	
	//Resource specific
	const char* GetName() const {return shaderName.c_str();};
	const char* GetClassNameStr() const;
	int Release();
	void Invalidate();
	bool Restore();
	bool IsValid() const;
	GResource* Clone(const char* newName);
	std::string toString() const;

	std::string getLoadPath() const {return load_path;};
	bool isLoaded() const {return !load_path.empty();};
	bool isAsyncLoad() const {return bAsyncLoad;};

	//serialize
	bool SaveXML(pugi::xml_node& node);
protected:
	static GShader* CreateShaderObject(const char* name, bool& bFound);
	
	//Parameter blocks
	bool initDefParamBlock();
	bool setShaderResources(sShaderResources* pResources);
	bool updateParameterBlock(HSHADERCACHEBLOCK blockId, sShaderResources* pResources);
protected:
	GDeviceShader* pDeviceShader;
	HSHADERCACHEBLOCK defParamBlockId;
	BYTE* pSrcData;
	UINT SrcDataLen;
	std::string shaderName;

	std::string load_path;
	bool bLoaded;
	bool bAsyncLoad;
	bool bNowShaderBegin;

	std::vector<std::pair<eRenderSemantic,D3DXHANDLE>> usedSemantics;
};

