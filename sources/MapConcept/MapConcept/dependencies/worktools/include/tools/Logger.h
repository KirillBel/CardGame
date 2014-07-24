#pragma once

namespace Logger
{

#define _PLACE_LOGDATA(x) x

#ifdef LOGGER_OUT_FILE_FUNC_LINE
	#define _PLACE_LOGDATA(x) "[%s, %s, %d] "x,__FILE__,__FUNCTION__,__LINE__
#endif

#ifdef LOGGER_OUT_FUNC_LINE
	#define _PLACE_LOGDATA(x) "[%s, %d] "x,__FUNCTION__,__LINE__
#endif

#ifdef LOGGER_OUT_FUNC
	#define _PLACE_LOGDATA(x) "[%s] "x,__FUNCTION__
#endif

#define LOGINFO(x,...) Logger::Info(Logger::CONTEXT_ID_GLOBAL, _PLACE_LOGDATA(x),__VA_ARGS__);
#define LOGWARNING(x,...) Logger::Warning(Logger::CONTEXT_ID_GLOBAL,_PLACE_LOGDATA(x),__VA_ARGS__);
#define LOGERROR(x,...) Logger::Error(Logger::CONTEXT_ID_GLOBAL,_PLACE_LOGDATA(x),__VA_ARGS__);
#define LOGCRITICAL(x,...) Logger::CriticalError(Logger::CONTEXT_ID_GLOBAL,_PLACE_LOGDATA(x),__VA_ARGS__);

#define LOGINFO_COL(col,x,...) Logger::Message(Logger::CONTEXT_ID_GLOBAL,Logger::TYPE_INFO,col,_PLACE_LOGDATA(x),__VA_ARGS__);
#define LOGWARNING_COL(col,x,...) Logger::Warning(Logger::CONTEXT_ID_GLOBAL,Logger::TYPE_WARNING,col,_PLACE_LOGDATA(x),__VA_ARGS__);
#define LOGERROR_COL(col,x,...) Logger::Error(Logger::CONTEXT_ID_GLOBAL,Logger::TYPE_ERROR,col,_PLACE_LOGDATA(x),__VA_ARGS__);
#define LOGCRITICAL_COL(col,x,...) Logger::CriticalError(Logger::CONTEXT_ID_GLOBAL,Logger::TYPE_CRITICAL,col,_PLACE_LOGDATA(x),__VA_ARGS__);

#define LOGINFOEX(y,x,...) Logger::Info(y, _PLACE_LOGDATA(x),__VA_ARGS__);
#define LOGWARNINGEX(y,x,...) Logger::Warning(y,_PLACE_LOGDATA(x),__VA_ARGS__);
#define LOGERROREX(y,x,...) Logger::Error(y,_PLACE_LOGDATA(x),__VA_ARGS__);
#define LOGCRITICALEX(y,x,...) Logger::CriticalError(y,_PLACE_LOGDATA(x),__VA_ARGS__);

	const int TYPE_UNKNOWN = 0;
	const int TYPE_INFO = 1;
	const int TYPE_WARNING = 2;
	const int TYPE_ERROR = 3;
	const int TYPE_CRITICAL = 4;

	const int CONTEXT_ID_OVERALL = -1;
	const int CONTEXT_ID_GLOBAL = 0;

	struct sLoggerMessage
	{
		sLoggerMessage()
		{
			type=TYPE_UNKNOWN;
			context=0;
		};
		std::string data;
		ColorB color;
		int type;
		int context;
		SYSTEMTIME time;

		std::string toString();
	};

	class ILoggerListener
	{
	public:
		ILoggerListener(void) {};
		virtual ~ILoggerListener(void) {};

		virtual void processMessage(sLoggerMessage& message) = 0;
	};

	class LoggerCore : public Singleton<LoggerCore>
	{
	public:
		LoggerCore(void);
		~LoggerCore(void);

		void attachOverallListener(ILoggerListener* pListener);
		void attachGlobalListener(ILoggerListener* pListener);
		void attachContextListener(ILoggerListener* pListener, int contId);
		void detachListener(ILoggerListener* pListener);

		void addMessage(int context, int type, ColorB& color, const char* str);
		void addMessageFmt(int context, int type, ColorB& color, const char* str, ...);
		void addMessageVA(int context, int type, ColorB& color, const char* str, va_list& args);

		UINT getInfoCount() {return MsgInfoCnt;};
		UINT getErrorCount() {return MsgErrorCnt;};
		UINT getWarningCount() {return MsgWarningCnt;};
		UINT getUnknownCount() {return MsgUnknownCnt;};
	protected:
		std::vector<ILoggerListener*>& getContext(int contextId);
		int findListener(ILoggerListener* pListener,int contextId);
	private:
		UINT messageLimit;
		std::list<sLoggerMessage> messages;
		std::map<int, std::vector<ILoggerListener*>> listeners;

		CRITICAL_SECTION CS1, CS2, CS3;

		int MsgWarningCnt;
		int MsgErrorCnt;
		int MsgInfoCnt;
		int MsgUnknownCnt;
	};

	inline LoggerCore& GetLogger() 
	{
		return (LoggerCore&)LoggerCore::Instance();
	};

	void Info(int context,const char* str, ...);
	void Warning(int context, const char* str, ...);
	void Error(int context, const char* str, ...);
	void CriticalError(int context, const char* str, ...);
	void Message(int context, int type,ColorB& color, const char* str, ...);
	
	void AttachOverallListener(ILoggerListener* pListener);
	void AttachGlobalListener(ILoggerListener* pListener);
	void AttachContextListener(ILoggerListener* pListener, int contId);
	void DetachListener(ILoggerListener* pListener);

	class ConsoleLogger : public Logger::ILoggerListener
	{
	public:
		ConsoleLogger(void) {};
		~ConsoleLogger(void) {};

		void processMessage(Logger::sLoggerMessage& message)
		{
			printf(message.toString().c_str());
		};
	};

	class DebugTraceLogger : public Logger::ILoggerListener
	{
	public:
		DebugTraceLogger(void) {};
		~DebugTraceLogger(void) {};

		void processMessage(Logger::sLoggerMessage& message)
		{
			OutputDebugString(message.toString().c_str());
		};
	};
};

