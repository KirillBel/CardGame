#pragma once

enum eResLoadDataType
{
	RLDT_TEXTURE_LOAD_REQUEST,
	RLDT_TEXTURE_LOAD_RESULT,
	RLDT_MESH_LOAD_REQUEST,
	RLDT_MESH_LOAD_RESULT,
};

struct sResLoadData
{
	eResLoadDataType dataType;
	void* pData;
};

struct sResLoadTextureData
{
	eTextureType Type;
	UINT nWidth;
	UINT nHeight;
	UINT nDepth;
	UINT MipLevels;
	eTextureFmt eTF;
	eTextureUsage Assign;
	GDeviceTexture* pDeviceTexture;
	GTexture* pTex;
	std::string path;
};

struct sResLoadMeshData
{
	std::string path;
	bool bLoadTextures;
	GMesh* pMesh;
};

class GResourceLoader;
class ResLoadTh : public BaseThread
{
public:
	ResLoadTh(void) {parent=NULL;};
	virtual ~ResLoadTh(void) {};

	void setParent(GResourceLoader* parent) {this->parent=parent;};
	eThreadReturn run();

protected:
	GResourceLoader* parent;
};

class GResourceLoader
{
public:
	GResourceLoader(void);
	~GResourceLoader(void);

	void Create(UINT MaxWorkingThreads);
	void Release();

	GTexture* LoadTexture(const char* path,eTextureType Type, UINT nWidth, UINT nHeight, UINT nDepth, UINT MipLevels=1,eTextureFmt eTF=TEXFMT_A8R8G8B8, eTextureUsage Assign=TEXUSAGE_BASE);
	GTexture* LoadTextureNative(const char* path,eTextureType Type, UINT nWidth, UINT nHeight, UINT nDepth, UINT MipLevels=1,eTextureFmt eTF=TEXFMT_A8R8G8B8, eTextureUsage Assign=TEXUSAGE_BASE);

	bool LoadTexture(GTexture* pTex, const char* path, bool bAsyncLoad, eTextureType Type, UINT nWidth, UINT nHeight, UINT nDepth, UINT MipLevels=1,eTextureFmt eTF=TEXFMT_A8R8G8B8, eTextureUsage Assign=TEXUSAGE_BASE);
	bool LoadShader(GShader* pShader, const char* path, bool bAsyncLoad, DWORD flags);

	bool LoadMesh(GMesh* pTex, const char* path, bool bAsyncLoad=true, bool bLoadWithTextures=true);

	bool LoadToDeviceTextureNative(GDeviceTexture* pObject, const char* path,eTextureType Type, UINT nWidth, UINT nHeight, UINT nDepth, UINT MipLevels=1,eTextureFmt eTF=TEXFMT_A8R8G8B8);
	bool LoadToMesh(GMesh* pTex, const char* path, bool bLoadWithTextures=true);

	std::string GetCurLoadPath();

protected:
	void SetCurLoadPath(std::string& val);

protected:
	friend class ResLoadTh;
	bool push_task(sResLoadData Data);
	bool pop_task(sResLoadData* pData);
	void onResultReady(sResLoadData Data);
	std::list<sResLoadData> tasks;
	thkit::Mutex thMutex;
	thkit::Mutex thMutex1;
	thkit::Mutex thMutexLoadPath;
	volatile bool bEndFlag;
protected:
	std::vector<ResLoadTh> threads;
	UINT getNumFreeThreads();
	void wakeUpThread();
	std::string curr_loading_path;
};

