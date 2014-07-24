#pragma once


class GVertexBuffer : public GResource
{
public:
	GVertexBuffer(void)
	{
		pDeviceVB=0;
	};

	~GVertexBuffer(void);

	static GVertexBuffer* CreateVertexBuffer(GVertexDecl& decl, UINT sizeVertices, eBufferType type,bool bWriteonly);
	static GVertexBuffer* GetVertexBuffer(RCID id);

	void SetVertexBuffer(GVertexDecl& decl, UINT sizeVertices, eBufferType type,bool bWriteonly);
	bool Create();
	//DEVICE INDEPENDENT
	GVertexDecl getVertexDecl() const {return decl;};
	UINT getSize() const;
	UINT getNumVertices() const;
	eBufferType getType() const;
	bool isWriteOnly() const;
	GDeviceVertexBuffer* GetDeviceVB() {return pDeviceVB;};

	//DEVICE DEPENDENT
	BYTE* Lock(UINT Offset, UINT SizeBytes,UINT flags);
	void Unlock();
	bool Resize(UINT newSizeVertices);
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

protected:
	static GVertexBuffer* CreateVertexBufferObject(const char* name, bool& bFound);
private:
	GDeviceVertexBuffer* pDeviceVB;
	GVertexDecl decl;
	UINT sizeVertices;
	eBufferType type;
	bool bWriteonly;
};

