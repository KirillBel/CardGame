#pragma once
class GResource;

typedef UINT RCNameHash;
typedef int RCID;

typedef std::vector<GResource*> ResourceArr;
typedef std::map<RCNameHash,GResource*> ResourceMap;
typedef ResourceMap::iterator ResourceMapIt;
typedef std::vector<RCID> ResourceIDs;

struct sResourceContainer
{
	ResourceArr ResArr;
	ResourceMap ResMap;
	ResourceIDs AvailableIDs;

	sResourceContainer()
	{
		ResArr.reserve(512);
	};

	~sResourceContainer();
};

typedef std::map<RCNameHash,sResourceContainer *> ResourceClassMap;
typedef ResourceClassMap::iterator ResourceClassMapIt;

class GResource
{
public:
	GResource(void) : referenceCounter(1), className(0), resourceName(0), resourceId(0)  {};
	virtual ~GResource(void) {};

	static RCNameHash GetRCNameHash(const char* name) {return strkit::calculateHashLowerCase(name);};

	static sResourceContainer* GetResourceClass(RCNameHash ClassName);
	static GResource *GetResource(const RCNameHash className, RCID nID, bool bAddRef=false);
	static GResource *GetResource(const RCNameHash className, const RCNameHash Name, bool bAddRef=false);
	static void ReleaseClass(const RCNameHash className, bool bForceRelease=false);
	static void ReloadClass(const RCNameHash className);
	static void InvalidateResources();
	static void ReleaseResources();

	inline int AddRef() {return ++referenceCounter;};
	inline int GetRefCounter() {return referenceCounter;};
	inline int RemoveRef() {return --referenceCounter;};
	inline RCID GetID() const {return resourceId;};
	
	virtual int Release();
	virtual GResource* Clone(const char* newName) {return NULL;};

	bool Register(RCNameHash resName, RCNameHash className);
	bool UnRegister();
	bool IsResourceValid() const;
	bool IsContainerValid() const;
	

	RCNameHash GenerateUniqueName() const;

	virtual const char* GetClassNameStr() const = 0;
	virtual const char* GetName() const = 0;
	RCNameHash GetClassName() const {return GetRCNameHash(GetClassNameStr());};
	RCNameHash GetResourceName() const {return resourceName;};
	
	virtual void Invalidate() = 0;
	virtual bool Restore() = 0;
	virtual bool IsValid() const = 0;
	virtual std::string toString() const {return "Unknown";};

private:
	static ResourceClassMap ResourceClasses;
	static thkit::Mutex ResourceLock;
protected:
	RCNameHash className;
	RCNameHash resourceName;

	RCID resourceId;

	volatile int referenceCounter;
};