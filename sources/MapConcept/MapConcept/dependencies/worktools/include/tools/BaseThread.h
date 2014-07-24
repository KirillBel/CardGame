#pragma once

enum eThreadReturn
{
	THRET_END_FAILED,
	THRET_END_SUCCESS,
	THRET_CONTINUE
};

enum eThreadSyncronizeState
{
	THSYNC_NONE,
	THSYNC_REQUESTED,
	THSYNC_ACCEPTED,
};

enum eThreadPriority
{
	THPRIO_PRIORITY_ABOVE_NORMAL,
	THPRIO_PRIORITY_BELOW_NORMAL,
	THPRIO_PRIORITY_HIGHEST,
	THPRIO_PRIORITY_IDLE,
	THPRIO_PRIORITY_LOWEST,
	THPRIO_PRIORITY_NORMAL,
	THPRIO_PRIORITY_TIME_CRITICAL,
};

class BaseThread
{
public:
	BaseThread(void);
	virtual ~BaseThread(void);

	bool start();
	bool stop(bool bCanTerminate=false,DWORD waitTime=500);
	bool join(int waitMsek=-1);
	void suspend();
	void resume();
	bool syncronize(bool bStart, DWORD maxTimeWait=0xFFFFFFFF);
	eThreadSyncronizeState getSyncState() {return syncState;};
	void requestSync() {syncState=THSYNC_REQUESTED;};
	void endSync() {syncState=THSYNC_NONE;};
	bool isRunning();
	eThreadReturn getEndFlag() {return endFlag;};

	void setThreadPriority(eThreadPriority prio);

	virtual eThreadReturn run() = 0;
	virtual void onThreadCreate() {};
	virtual void onThreadRelease() {};

	static DWORD WINAPI threadEntryPoint(LPVOID pArg)
	{
		BaseThread* pParent=(BaseThread*)pArg;
		pParent->bThreadRun=true;
		pParent->onThreadCreate();

		eThreadReturn ret=THRET_CONTINUE;
		while(ret==THRET_CONTINUE)
		{
			ret=pParent->run();
			pParent->checkThreadFlags();
		};
		pParent->endFlag=ret;
		pParent->onThreadEnd();
		ExitThread(0);
		return 0;
	};

protected:
	void checkThreadFlags();
private:
	void terminate();
	void onThreadEnd();
private:
	bool bDestroyFlag;
	eThreadSyncronizeState syncState;
	eThreadPriority threadPrio;
	eThreadReturn endFlag;
	bool bThreadRun;

	HANDLE hThread;
	DWORD threadID;
};
