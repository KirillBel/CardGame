#pragma once
#include <fstream>

void HTMLLoggerColor(ColorF rgb);

class HTMLLogger : public Logger::ILoggerListener
{
public:
	HTMLLogger(void);
	virtual ~HTMLLogger(void);

	void Init(char* FilePath, char* LogName);
	void Release();
	void processMessage(Logger::sLoggerMessage& Msg);

	virtual void WriteData(const char* string);
	virtual void FlushData();

protected:
	void Write(const char* color, const char* string);
	void StartBanner(const char *logname);
	void EndBanner();
	void WriteTime();

	std::ofstream  LogFile;
	bool bLogInit;
};

