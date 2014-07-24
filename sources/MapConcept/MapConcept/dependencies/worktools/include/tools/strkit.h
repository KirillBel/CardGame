#pragma once

namespace strkit
{
	char* stripFileExtension (char * szFilePath);
	std::string getParentDirectory (const std::string& strFilePath, int nGeneration = 1);
	const char* findExtension (const char* szFileName);
	const char* findFileNameInPath (const char* szFilePath);
	std::string makeAbsolutePath(const char* path);
	std::list<std::string> getFileDirectories(const char* path, bool bStripSlashes);
	std::string makeRelativePath(const char* rel, const char* relTo);
	std::string makeRelativePath(const char* rel);


	std::string toLower (const std::string& str);
	char toLower(char c);

	std::string format (const char* Str,...);

	UINT calculateHash(BYTE* pData, int size);
	UINT calculateHash(const char *str);
	UINT calculateHashLowerCase(const char *str);

	std::string toString(int var);
	std::string toString(float var);
	std::string toString(double var);
	std::string toString(long var);
	std::string toString(short var);
	std::string toString(unsigned int var);
	std::string toString(unsigned long var);
	std::string toString(unsigned short var);
	std::string toString(unsigned char var);
	std::string toString(long long var);
	std::string toString(bool var);
	std::string toString(unsigned long long var);
};