#pragma once
class CBinaryData
{
public:
	CBinaryData(void);
	~CBinaryData(void);

	void Clear();

	CBinaryData& operator<<(int val);
	CBinaryData& operator<<(char val);
	CBinaryData& operator<<(float val);
	CBinaryData& operator<<(double val);
	CBinaryData& operator<<(short val);
	CBinaryData& operator<<(long long val);
	CBinaryData& operator<<(bool val);
	CBinaryData& operator<<(BYTE val);
	CBinaryData& operator<<(WORD val);
	CBinaryData& operator<<(DWORD val);
	CBinaryData& operator<<(UINT val);
	CBinaryData& operator<<(char* val);
	CBinaryData& operator<<(std::string& val);
	CBinaryData& operator<<(CBinaryData& val);

	CBinaryData& operator>>(int& val);
	CBinaryData& operator>>(char& val);
	CBinaryData& operator>>(float& val);
	CBinaryData& operator>>(double& val);
	CBinaryData& operator>>(short& val);
	CBinaryData& operator>>(long long& val);
	CBinaryData& operator>>(bool& val);
	CBinaryData& operator>>(BYTE& val);
	CBinaryData& operator>>(WORD& val);
	CBinaryData& operator>>(DWORD& val);
	CBinaryData& operator>>(UINT& val);
	CBinaryData& operator>>(std::string& val);
	CBinaryData& operator>>(char** val);

	template <class F> 
	inline CBinaryData& operator>>(F val)
	{
		int ValSize=sizeof(val);
		_ASSERT(!((ReadCounter+ValSize)>Data.size()));
		_ASSERT((ReadCounter!=Data.size()));
		memcpy(&val,&(Data[ReadCounter]),ValSize);
		ReadCounter+=ValSize;
		return (*this);
	};

	bool ReadToBuf(void* pBuf, UINT BufSize);
	void WriteFromBuf(void* pBuf, UINT BufSize);
	bool ReadToBuf(CBinaryData& Buf, UINT Size);
	void WriteFromBuf(CBinaryData& Buf, UINT Size);
	void SetReadPtr(UINT Val);

	void Dump(void** ppBuf, UINT* pSize);
	void* Ptr() {return &(Data[0]);};
	UINT GetSize() {return Data.size();};
	UINT GetReadPtr() {return ReadCounter;};
	UINT GetWritePtr() {return Data.size();};

	CBinaryData& operator = (CBinaryData& other);

	bool ReadBegin();
	bool ReadEnd();

	bool Save(char* Path);
	bool Load(char* Path);

	UINT Hash(int from);
	bool TestHash(UINT Hash, int from,int to);
private:
	std::vector<BYTE> Data;

	UINT ReadCounter;
};

