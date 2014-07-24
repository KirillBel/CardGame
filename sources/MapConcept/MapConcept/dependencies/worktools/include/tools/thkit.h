#pragma once
namespace thkit
{
	class Mutex
	{
	public:
		Mutex() {InitializeCriticalSectionAndSpinCount(&CS, 0x00000400);};
		~Mutex() {DeleteCriticalSection(&CS);};

		void Lock()	{EnterCriticalSection(&CS);};
		void Unlock() {LeaveCriticalSection(&CS);};
	private:
		CRITICAL_SECTION CS;
	};

	template<class LockType>
	class AutoLock
	{
	public:
		AutoLock(LockType& lock_) : lock(lock_) {lock.Lock();};
		~AutoLock() {lock.Unlock();};

	private:
		LockType& lock;
	};

	DWORD GetCurrentThread();

#define AUTO_LOCK(m) thkit::AutoLock<thkit::Mutex> __AutoLock(m);

	struct sThreadTask
	{
		sThreadTask()
		{
			currentPriority=0;
			priorityLevel=0;
			addTime=GetTickCount();
			startTime=addTime;

			TaskFunction=0;
			CallOnStart=0;
			CallOnFinish=0;
			taskNumber=-1;
			pTaskParam=0;
		};

		float currentPriority;
		BYTE priorityLevel;
		DWORD addTime;
		DWORD startTime;
		int taskNumber;
		void* pTaskParam;

		IRunnable* TaskFunction;
		IRunnable* CallOnStart;
		IRunnable* CallOnFinish;
	};

	class ThreadTaskWorker : public BaseThread
	{
	public:
		ThreadTaskWorker() 
		{
			currentTask=0;
		};
		~ThreadTaskWorker() {};

		void init(int workerId)
		{
			this->workerId=workerId;
		};

		void setCurrentTask(sThreadTask* currentTask)
		{
			this->currentTask=currentTask;
		};

		eThreadReturn run();

	private:
		int workerId;
		sThreadTask* currentTask;
	};

	class ThreadTaskMgr : public Singleton<ThreadTaskMgr>
	{
	public:
		ThreadTaskMgr() 
		{
			CriticalWaitTime=1000;
			TaskCounter=0;
			prevUpdateTime=GetTickCount();
		};
		~ThreadTaskMgr() 
		{
			Release();
		};

		static int GetNumCores();
		static int Create(UINT NumWorkers=0, DWORD CriticalWaitTime=1000);
		static void AddTask(IRunnable* TaskFunction, void* pParam, BYTE priorityLevel, IRunnable* CallOnStart=0,IRunnable* CallOnFinish=0);
		static ThreadTaskMgr& GetTaskMgr();
		
		void Release();
		int GetNumTasks();
		int GetNumWorkers();
		sThreadTask GetTask(int index);
		
		sThreadTask* worker_GetNewTask(int workerId);
	protected:
		void NewTask(sThreadTask* pTask);
		void sortTasks();
		void UpdateTasks();
	private:
		DWORD prevUpdateTime;
		Mutex lock;
		int TaskCounter;
		DWORD CriticalWaitTime;
		std::vector<sThreadTask*> tasks;
		std::vector<ThreadTaskWorker*> workers;
	};


	////ASYNC LOAD FILE///////
	struct sThAsyncLoadFileData
	{
		sThAsyncLoadFileData(const char* path)
		{
			rsize_t len=strlen(path)+1;
			this->path=new char[len];
			strcpy_s(this->path,len,path);
			path=0;
			pOutData=0;
			OutDataSize=0;
		};

		~sThAsyncLoadFileData()
		{
			if(path)
			{
				delete [] path;
				path=0;
			};
		};
		//in
		char* path;
		//out
		BYTE* pOutData;
		UINT OutDataSize;
	};

	void AsyncLoadFile(const char* path, IRunnable* pResultFunc, BYTE priorityLevel=128);

	template<class T> 
	bool WaitForTriggerValue(T& trig, T value, int time=-1)
	{
		DWORD waitStart=::GetTickCount();

		while(trig!=value)
		{
			if((time!=-1) && ((::GetTickCount()-waitStart)>time)) return false;
			Sleep(1);
		};
		return true;
	};
};

