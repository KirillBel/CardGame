#pragma once

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif

template<typename CheckType,typename InstanceType> 
bool instanceOf(InstanceType &Instance) 
{
  return (dynamic_cast<CheckType *>(&Instance) != NULL);
}

template<typename CheckType,typename InstanceType> 
bool instanceOf(InstanceType* pInstance) 
{
  return (dynamic_cast<CheckType *>(pInstance) != NULL);
}

static bool isFileExists(const char* path)
{
	return _access(path, 0) != -1;
};

template<typename T> 
inline UINT ObjectHash(T& object)
{
	return strkit::calculateHash((BYTE*)&object,sizeof(object));
};

#define assert(_Expression) _ASSERT(_Expression);
//(void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

///////////////////////////////////////////////////////////////////////
	struct IRunnable
	{
		virtual void run(void* pParam=0) = 0;
	};

	template<typename T>
	struct RunMethod_ClassFunc : public IRunnable
	{
		typedef void (T::*RunMethod)(void*);

		RunMethod_ClassFunc(T* classPtr,RunMethod runFunction)
		{
			this->classPtr=classPtr;
			this->runFunction=runFunction;
		};

		void run(void* pParam=0)
		{
			(classPtr->*runFunction)(pParam);
		};

		RunMethod runFunction;
		T* classPtr;
	};

	struct RunMethod_Func : public IRunnable
	{
		typedef void (*RunMethod)(void*);

		RunMethod_Func(RunMethod runFunction)
		{
			this->runFunction=runFunction;
		};

		void run(void* pParam=0)
		{
			(runFunction)(pParam);
		};

		RunMethod runFunction;
	};
/////////////////////////////////////////////////////////////////////////