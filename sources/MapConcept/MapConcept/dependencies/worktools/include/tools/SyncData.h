#pragma once

template<class F> 
class SyncData
{
public:
	SyncData(void) 
	{
		InitializeCriticalSectionAndSpinCount(&syncDataCS, 0x00000400);
	};
	virtual ~SyncData(void) {};

	void push(F& data)
	{
		EnterCriticalSection(&syncDataCS);
		syncDataList.push_back(data);
		LeaveCriticalSection(&syncDataCS);
	};

	bool pop(F& data)
	{
		if(syncDataList.empty())
			return false;

		EnterCriticalSection(&syncDataCS);
		data=(*syncDataList.begin());
		syncDataList.pop_front();
		LeaveCriticalSection(&syncDataCS);
		return true;
	};

	UINT getSize()
	{
		return syncDataList.size();
	};

private:
	CRITICAL_SECTION syncDataCS;
	std::list<F> syncDataList;
};
