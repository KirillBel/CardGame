#pragma once

class GIndexBuffer : public GResource
{
public:
	GIndexBuffer(void)
	{
		pDeviceIB=0;
	};

	~GIndexBuffer(void);

	static GIndexBuffer* CreateIndexBuffer(eIndexFormat format, UINT sizeIndices, eBufferType type,bool bWriteonly);
	static GIndexBuffer* GetIndexBuffer(RCID id);

	void SetIndexBuffer(eIndexFormat format, UINT sizeIndices, eBufferType type,bool bWriteonly);
	bool Create();
	//DEVICE INDEPENDENT
	UINT getSize() const;
	UINT getNumIndices() const;
	eBufferType getType() const;
	eIndexFormat getFormat() const;
	bool isWriteOnly() const;
	GDeviceIndexBuffer* GetDeviceIB() {return pDeviceIB;};

	//DEVICE DEPENDENT
	BYTE* Lock(UINT Offset, UINT SizeBytes,UINT flags);
	void Unlock();
	bool Resize(UINT newSizeIndices);
	bool SetData(void* pData, UINT dataSize, UINT offset);

	//Resource specific
	const char* GetName() const {return toString().c_str();};
	const char* GetClassNameStr() const;
	int Release();
	void Invalidate();
	bool Restore();
	bool IsValid() const;
	GResource* Clone(const char* newName);
	std::string toString() const;

	//serialize
	bool Serialize(pugi::xml_node& node, bool bStore);

private:
	static GIndexBuffer* CreateIndexBufferObject(const char* name, bool& bFound);
private:
	GDeviceIndexBuffer* pDeviceIB;
	eIndexFormat format;
	UINT sizeIndices;
	eBufferType type;
	bool bWriteonly;
};

