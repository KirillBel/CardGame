#pragma once

enum eDebugCrashType
{
	CRASH_SEGFAULT,
	CRASH_ABORT,
	CRASH_FLOATERROR,
	CRASH_ILLEGAL_INSTRUCTION,
	CRASH_TERMINATION_REQUEST,
};

void posix_segfault(int signum);
void posix_abort(int signum);
void posix_floaterror(int signum);
void posix_illegal_instruction(int signum);
void posix_termination_request(int signum);


class DebugCrashReport : public Singleton<DebugCrashReport>
{
public:
	DebugCrashReport(void);
	~DebugCrashReport(void);

	static void Init()
	{
		signal(SIGSEGV, posix_segfault);
		signal(SIGABRT, posix_abort);
		signal(SIGFPE, posix_floaterror);
		signal(SIGILL, posix_illegal_instruction);
		signal(SIGTERM, posix_termination_request);
		Instance();
	};

	void onCrash(eDebugCrashType type);
	
	

};

