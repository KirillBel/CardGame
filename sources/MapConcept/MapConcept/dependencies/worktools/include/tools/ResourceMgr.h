#pragma once
class ResourceMgr;
class IResource;
typedef int RESID;

enum eResourceState
{
	STATE_EMPTY,
	STATE_WAIT_DATA,
	STATE_DATA_OK,
	STATE_DATA_FAILED,
	STATE_READY,
};

struct sResourceDesc
{
	sResourceDesc()
	{
		resourceID=-1;
		resourceType=-1;
		resourceObject=0;
		resourceLastUsedTime=::GetTickCount();
		resourceState=STATE_EMPTY;
		pResource=NULL;
		pUserData=0;
	};

	RESID resourceID;
	int resourceType;
	std::string resourcePath;
	DWORD resourceLastUsedTime;
	eResourceState resourceState;

	void* resourceObject;
	void* pUserData;

	IResource* pResource;
};

struct sResourceData
{
	sResourceData()
	{
		id=-1;
		data=0;
		size=-1;
	};

	sResourceData(RESID resourceID_,std::string& path_, void* resourceData_,int resourceDataSize_)
	{
		id=resourceID_;
		data=resourceData_;
		size=resourceDataSize_;
		path=path_;
	};

	RESID id;
	void* data;
	std::string path;
	int size;
};

class IResource
{
public:
	IResource(void)
	{
		pMgr=0;
		resourceID=-1;
	};
	virtual ~IResource(void) {};

	void* getResourceObject();
	eResourceState getResourceState();
	int getResourceType();

	RESID getResourceID() {return resourceID;};

private:
	friend class ResourceMgr;
	ResourceMgr* pMgr;
	RESID resourceID;
};

class ResourceTh : public BaseThread
{
public:
	ResourceTh(ResourceMgr* pResMan, int taskLimit_)
	{
		pRM=pResMan;
		taskLimit=taskLimit_;
		taskSize=0;
		InitializeCriticalSectionAndSpinCount(&CS1, 0x00000400);
	};

	eThreadReturn run();
	void pushTask(int resourceID, const char* path, bool bWaitEnd);
	bool popTask(std::pair<int, std::string>* pTask);
private:
	ResourceMgr* pRM;
	std::list<std::pair<int, std::string>> tasks;
	CRITICAL_SECTION CS1;
	int taskLimit;
	int taskSize;
};

class ResourceMgr
{
public:
	ResourceMgr(void);
	~ResourceMgr(void);

	void* getResourceObject(RESID id);
	IResource* getResource(RESID id);
	IResource* getResource(const char* path);
	int getResourceType(RESID id);
	eResourceState getResourceState(RESID id);
	void checkResource(RESID id);

protected:
	int loadResource(IResource* pRes, bool bWait, int resourceType,  const char* path, void* defaultObject=NULL,void* pUserData=NULL);
	int createResourceDesc(IResource* pRes, int resourceType,  void* defaultObject=NULL,void* pUserData=NULL);
	sResourceDesc& getDesc(RESID id) {return resources[id];};
protected:
	friend class ResourceTh;
	void processNewData();
	void pushData(sResourceData data);
	bool popData(sResourceData* pData);
	virtual void onResourceDataLoaded(sResourceData& data) = 0;
private:
	
	void load(RESID resID, const char* path, bool bWait, void* pUserData=NULL);
private:
	CRITICAL_SECTION CS1;
	ResourceTh* pThread;
	std::vector<sResourceDesc> resources;
	std::list<sResourceData> resourceData;
	std::map<UINT, RESID> pathID;
};

