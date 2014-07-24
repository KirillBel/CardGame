#pragma once

class CollectorLogger : public Logger::ILoggerListener, public SyncData<Logger::sLoggerMessage>
{
public:
	CollectorLogger(void) 
	{
		msgLimit=0;
	};
	virtual ~CollectorLogger(void) {};

	void setMessageLimit(UINT limit)
	{
		msgLimit=limit;
		
		if(msgLimit)
		{
			Logger::sLoggerMessage tmp;
			while(getSize()>msgLimit)
				pop(tmp);
		};
	};

	void processMessage(Logger::sLoggerMessage& Msg)
	{
		if(msgLimit)
		{
			Logger::sLoggerMessage tmp;
			while(getSize()>msgLimit)
				pop(tmp);
		};

		push(Msg);
	};

private:
	UINT msgLimit;
};
