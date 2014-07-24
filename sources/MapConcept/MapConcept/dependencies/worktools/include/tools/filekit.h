#pragma once
class File
{
public:
	File(void)
	{
	};

	~File(void)
	{
	};

	static bool isExists(const char* path)
	{
		return _access(path, 0) != -1;
	};

	static long getLength(const char* path)
	{
		struct _stat buf;
		int result;
		result = _stat( path, &buf );
		if(result!=0) return 0;
		return buf.st_size;
	};

	static bool load(const char* path, void** ppBuffer, int* pSize)
	{
		FILE* pFile=fopen(path,"rb");
		if(pFile==NULL) return false;

		(*pSize)=getLength(path);
		BYTE* pBuf=new BYTE[(*pSize)];
		fread(pBuf,1,(*pSize),pFile);
		(*ppBuffer)=pBuf;
		fclose(pFile);
		return true;
	};

	static bool save(const char* path, void* pBuffer, int size)
	{
		FILE* pFile=fopen(path,"w+b");
		if(pFile==NULL) return false;

		if(fwrite(pBuffer,size,1,pFile)==0) return false;
		fclose(pFile);
		return true;
	};

};

