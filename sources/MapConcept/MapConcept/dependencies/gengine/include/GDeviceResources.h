#pragma once

typedef union _HSHADERCACHEBLOCK{
    struct {
        int ShaderId;
        int BlockId;
    } u;
    LONGLONG QuadPart;
} HSHADERCACHEBLOCK;

class GDeviceBaseResource
{
public:
	GDeviceBaseResource(int id)
	{
		this->id=id;
		bCreated=false;
	};

	virtual ~GDeviceBaseResource()
	{
	};

	virtual void Release() = 0;
	virtual bool IsCreated() {return bCreated;};
	virtual bool IsValid() {return IsCreated();};

	int GetId() {return id;};

protected:
	int id;
	bool bCreated;
};

class GDeviceTexture : public GDeviceBaseResource
{
public:
	GDeviceTexture(int id) : GDeviceBaseResource(id)
	{
	};

	virtual bool Create(eTextureType type, UINT nWidth, UINT nHeight, UINT nDepth, UINT MipLevels,eTextureFmt fmt,eTextureUsage usage, LPCSTR pSrcFile=NULL) = 0;

	virtual BYTE* Lock2D(UINT level, int& pitch, UINT flags) = 0;
	virtual BYTE* Lock3D(UINT level, int& rowPitch,int& slicePitch, UINT flags) = 0;
	virtual BYTE* LockCube(eCubeFace nFace,int nLevel, int& pitch, UINT flags) = 0;
	virtual void Unlock(UINT level,eCubeFace nFace=CUBEFACE_POSX) = 0;

	virtual UINT getWidth() = 0;
	virtual UINT getHeight() = 0;
	virtual UINT getDepth() = 0;
	virtual UINT getMipLevels() = 0;
	virtual eTextureFmt getFormat() = 0;
	virtual eTextureType getType() = 0;
	virtual eTextureUsage getUsage() = 0;
protected:
};

class GDeviceVertexBuffer : public GDeviceBaseResource
{
public:
	GDeviceVertexBuffer(int id) : GDeviceBaseResource(id)
	{
	};

	virtual bool Create(UINT SizeBytes, eBufferType type,bool bWriteonly,DWORD FVF) = 0;

	virtual BYTE* Lock(UINT Offset, UINT SizeBytes,UINT flags) = 0;
	virtual void Unlock() = 0;
protected:
	
};

class GDeviceIndexBuffer : public GDeviceBaseResource
{
public:
	GDeviceIndexBuffer(int id) : GDeviceBaseResource(id)
	{
	};

	virtual bool Create(eIndexFormat format, UINT sizeBytes, eBufferType type,bool bWriteonly) = 0;

	virtual BYTE* Lock(UINT Offset, UINT SizeBytes,UINT flags) = 0;
	virtual void Unlock() = 0;
protected:
};

#define DEVRES_SHADER_DEBUG			0x2
#define DEVRES_SHADER_NOPARAMBLOCKS	0x4
#define DEVRES_SHADER_NOVALUES		0x8
#define DEVRES_SHADER_ENABLE_BACKWARDS_COMPATIBILITY		0x10

class GDeviceShader : public GDeviceBaseResource
{
public:
	GDeviceShader(int id) : GDeviceBaseResource(id)
	{
		CurrParamBlockId.u.ShaderId=id;
		CurrParamBlockId.u.BlockId=-1;
	};

	virtual bool Create(LPCVOID pSrcFileData,UINT dataLen, DWORD ShaderFlags, std::string* pCompilationErrors=NULL) = 0;
	virtual bool CreateFromFile(LPCSTR pSrcFile, DWORD ShaderFlags, std::string* pCompilationErrors=NULL) = 0;

	virtual bool setValue(const char* Name, bool Val) = 0;
	virtual bool setValue(const char* Name, int Val) = 0;
	virtual bool setValue(const char* Name, SHORT Val) = 0;
	virtual bool setValue(const char* Name, BYTE Val) = 0;
	virtual bool setValue(const char* Name, float Val) = 0;
	virtual bool setValue(const char* Name, Vec2& Val) = 0;
	virtual bool setValue(const char* Name, Vec3& Val) = 0;
	virtual bool setValue(const char* Name, Vec4& Val) = 0;
	virtual bool setValue(const char* Name, Matrix44& Val) = 0;
	virtual bool setValue(const char* Name, ColorF& Val) = 0;
	virtual bool setValue(const char* Name, GDeviceTexture* Val) = 0;

	virtual bool commitChanges() = 0;

	virtual HSHADERCACHEBLOCK createParameterBlock() = 0;
	virtual void discardParameterBlock(HSHADERCACHEBLOCK hBlock) = 0;
	virtual bool beginParameterBlock(HSHADERCACHEBLOCK hBlock) = 0;
	virtual bool endParameterBlock() = 0;
	virtual bool applyParameterBlock(HSHADERCACHEBLOCK hBlock) = 0;

protected:
	HSHADERCACHEBLOCK CurrParamBlockId;
	std::vector<D3DXHANDLE> parameterBlocks;
};




