#pragma once

#include <math.h>

#define PROFTIME double
//-------------------------------------------------------------------------

class IProfilerObject;

//-------------------------------------------------------------------------

struct sProfileInfo
{
	sProfileInfo()
	{
		name=0;
		pObject=0;
		func_name=0;
		file_path=0;
		line=0;
		id=-1;
	};

	sProfileInfo(int id, IProfilerObject* pData, const char* name, const char* func_name,const char* file_path,UINT line)
	{
		this->pObject=pData;
		this->name=name;
		this->func_name=func_name;
		this->file_path=file_path;
		this->line=line;
		this->id=id;
	};

	IProfilerObject* pObject;
	const char* func_name;
	const char* file_path;
	const char* name;
	UINT line;
	int id;
};

//-------------------------------------------------------------------------

class IProfilerObject
{
public:
	IProfilerObject() 
	{
		bRegistrated=false;
		regId=-1;
	};
	virtual ~IProfilerObject() {};

	virtual void begin() = 0;
	virtual void end() = 0;

	virtual std::string toString() = 0;

	int getId() {return regId;};
protected:
	void reg(const char* name, const char* func_name,const char* file_path,UINT line);
	void unreg();
	inline void onBegin(PROFTIME& time);
	inline void onEnd(PROFTIME& time);
	bool bRegistrated;
	int regId;
};

//-------------------------------------------------------------------------

class SimpleProfilerObject : public IProfilerObject
{
public:
	SimpleProfilerObject(const char* name, const char* func_name,const char* file_path,UINT line);
	~SimpleProfilerObject();
	void begin();
	void end();
	std::string toString();

public:
	PROFTIME TimeDelta;
	PROFTIME TimeCurrent;
};

//-------------------------------------------------------------------------

struct sProfilerCaptureInfo
{
	sProfilerCaptureInfo(bool bBegin, PROFTIME& time, IProfilerObject* pObject)
	{
		this->bBegin=bBegin;
		this->time=time;
		this->pObject=pObject;
		this->threadId=thkit::GetCurrentThread();
	};

	bool bBegin;
	PROFTIME time;
	DWORD threadId;
	IProfilerObject* pObject;
};

struct sProfilerCaptureTreeItem
{
	sProfilerCaptureTreeItem() 
	{
		pObj=NULL;
		threadId=0;
	};
	~sProfilerCaptureTreeItem()
	{
		for(int i=0;i<childs.size();i++)
		{
			SAFE_DELETE(childs[i]);
		}
		childs.clear();
	};

	double getBeginTimeMs() {return double(timeBegin);};
	double getEndTimeMs() {return double(timeEnd);};

	sProfileInfo info;
	IProfilerObject* pObj;
	PROFTIME timeBegin;
	PROFTIME timeEnd;
	DWORD threadId;
	std::vector<sProfilerCaptureTreeItem*> childs;
};

class ProfilerCapture
{
public:
	ProfilerCapture()
	{
		info.reserve(512);
	};

	bool save(const char* path);
	//bool load(const char* path);

	void pop(IProfilerObject* pObj, PROFTIME& time);
	void push(IProfilerObject* pObj, PROFTIME& time);

	sProfilerCaptureTreeItem* generateInfo();

	std::vector<sProfilerCaptureInfo> info;

protected:
	thkit::Mutex thLock;
};


//-------------------------------------------------------------------------

class Profiler : public Singleton<Profiler>
{
public:
	Profiler(void);
	~Profiler(void);

	static LARGE_INTEGER GetPerfCounterTime();
	static PROFTIME GetPerfCounterTimeMks();
	//static UINT64 GetPerfCounterTimeNs();
	static LARGE_INTEGER GetPerfCounterInterval(LARGE_INTEGER& PrevTime, LARGE_INTEGER& CurTime);
	//static double GetCurrentTime();
	static PROFTIME GetCurrentTimeMks();

	int RegObject(IProfilerObject* pObject,const char* name, const char* func_name,const char* file_path,UINT line);
	void UnregObject(IProfilerObject* pObject);

	void BeginCapture(ProfilerCapture* pCap);
	void EndCapture();

	sProfileInfo GetInfo(int id);
	sProfileInfo* GetProfItem(int index);
	int GetProfItemCount() {return ActiveProfArray.size();};

	static PROFTIME MinimalDelta;
protected:
	friend class IProfilerObject;
	void OnObjectBegin(IProfilerObject* pObj, PROFTIME& time);
	void OnObjectEnd(IProfilerObject* pObj, PROFTIME& time);

	static LARGE_INTEGER TimeStart;
	static LARGE_INTEGER PerfCounterFreq;
	static double PerfCounterFreqMks;
	

	ProfilerCapture* pCapture;

	std::map<IProfilerObject*, sProfileInfo*> ActiveProfMap;
	std::vector<sProfileInfo*> ActiveProfArray;
	std::list<UINT> freeIds;

	thkit::Mutex thLock;
};

struct sProfAutoDestructor
{
	sProfAutoDestructor(IProfilerObject* pData)
	{
		this->pData=pData;
		this->pData->begin();
	};

	~sProfAutoDestructor()
	{
		this->pData->end();
	};

	IProfilerObject* pData;
};

#define PROFILE_FUNCTION(name)  static SimpleProfilerObject ___Prof_FuncTimeData_##name(#name,__FUNCTION__,__FILE__,__LINE__); sProfAutoDestructor __Prof_AutoDestructor(&___Prof_FuncTimeData_##name);
#define PROFILE_FUNCTION_GET(name) ___Prof_FuncTimeData_##name

#define PROFILE_TIME_BEGIN(name) static SimpleProfilerObject ___Prof_TimeData_##name(#name,__FUNCTION__,__FILE__,__LINE__); ___Prof_TimeData_##name.begin();
#define PROFILE_TIME_END(name) ___Prof_TimeData_##name.end();
#define PROFILE_TIME_GET(name) ___Prof_TimeData_##name