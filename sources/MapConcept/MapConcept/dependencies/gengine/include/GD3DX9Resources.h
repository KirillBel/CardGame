#pragma once

class GD3DX9DeviceTexture : public GDeviceTexture
{
public:
	GD3DX9DeviceTexture(int id) : GDeviceTexture(id)
	{
		texture=0;
	};

	bool Create(eTextureType type, UINT nWidth, UINT nHeight, UINT nDepth, UINT MipLevels,eTextureFmt fmt,eTextureUsage usage, LPCSTR pSrcFile=NULL);
	void Release();

	BYTE* Lock2D(UINT level, int& pitch, UINT flags);
	BYTE* Lock3D(UINT level, int& rowPitch,int& slicePitch, UINT flags);
	BYTE* LockCube(eCubeFace nFace,int nLevel, int& pitch, UINT flags);
	void Unlock(UINT level,eCubeFace nFace=CUBEFACE_POSX);

	bool Resize(UINT width, UINT height);


	LPDIRECT3DBASETEXTURE9 GetBaseTexture() {return texture;};
	LPDIRECT3DTEXTURE9 Get2DTexture() {return (type==TEXTYP_2D) ?  (LPDIRECT3DTEXTURE9)GetBaseTexture() : 0;};
	LPDIRECT3DVOLUMETEXTURE9 Get3DTexture() {return (type==TEXTYP_3D) ?  (LPDIRECT3DVOLUMETEXTURE9)GetBaseTexture() : 0;};
	LPDIRECT3DCUBETEXTURE9 GetCubeTexture() {return (type==TEXTYP_CUBE) ?  (LPDIRECT3DCUBETEXTURE9)GetBaseTexture() : 0;};

	UINT getWidth() {return nWidth;};
	UINT getHeight() {return nHeight;};
	UINT getDepth() {return nDepth;};
	UINT getMipLevels() {return MipLevels;};
	eTextureFmt getFormat() {return fmt;};
	eTextureType getType() {return type;};
	eTextureUsage getUsage() {return usage;};

	

protected:
	LPDIRECT3DBASETEXTURE9 texture;
	eTextureType type;

	UINT nWidth;
	UINT nHeight;
	UINT nDepth;
	UINT MipLevels;
	eTextureFmt fmt;
	D3DPOOL pool;
	eTextureUsage usage;
};

class GD3DX9DeviceVertexBuffer : public GDeviceVertexBuffer
{
public:
	GD3DX9DeviceVertexBuffer(int id) : GDeviceVertexBuffer(id)
	{
		buffer=0;
	};
	~GD3DX9DeviceVertexBuffer()
	{
	};

	bool Create(UINT SizeBytes, eBufferType type,bool bWriteonly,DWORD FVF);
	void Release();

	BYTE* Lock(UINT Offset, UINT SizeBytes,UINT flags);
	void Unlock();

	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() {return buffer;};
protected:
	LPDIRECT3DVERTEXBUFFER9 buffer;
};

class GD3DX9DeviceIndexBuffer : public GDeviceIndexBuffer
{
public:
	GD3DX9DeviceIndexBuffer(int id) : GDeviceIndexBuffer(id)
	{
		buffer=0;
	};

	bool Create(eIndexFormat format, UINT sizeBytes, eBufferType type,bool bWriteonly);
	void Release();

	BYTE* Lock(UINT Offset, UINT SizeBytes,UINT flags);
	void Unlock();

	LPDIRECT3DINDEXBUFFER9 GetIndexBuffer() {return buffer;};
protected:
	LPDIRECT3DINDEXBUFFER9 buffer;
};



class GD3DX9DeviceShader : public GDeviceShader
{
public:
	GD3DX9DeviceShader(int id) : GDeviceShader(id)
	{
		effect=0;
	};

	bool Create(LPCVOID pSrcFileData,UINT dataLen, DWORD ShaderFlags, std::string* pCompilationErrors=NULL);
	bool CreateFromFile(LPCSTR pSrcFile, DWORD ShaderFlags, std::string* pCompilationErrors=NULL);
	void Release();

	bool setValue(const char* Name, bool Val);
	bool setValue(const char* Name, int Val);
	bool setValue(const char* Name, SHORT Val);
	bool setValue(const char* Name, BYTE Val);
	bool setValue(const char* Name, float Val);
	bool setValue(const char* Name, Vec2& Val);
	bool setValue(const char* Name, Vec3& Val);
	bool setValue(const char* Name, Vec4& Val);
	bool setValue(const char* Name, Matrix44& Val);
	bool setValue(const char* Name, ColorF& Val);
	bool setValue(const char* Name, GDeviceTexture* Val);

	bool commitChanges();

	HSHADERCACHEBLOCK createParameterBlock();
	void discardParameterBlock(HSHADERCACHEBLOCK hBlock);
	bool beginParameterBlock(HSHADERCACHEBLOCK hBlock);
	bool endParameterBlock();
	bool applyParameterBlock(HSHADERCACHEBLOCK hBlock);

	LPD3DXEFFECT GetShader() {return effect;};
protected:
	LPD3DXEFFECT effect;
	DWORD Flags;
};

class GD3DXResourceMgr
{
public:
	GDeviceTexture* CreateTextureObject();
	GDeviceVertexBuffer* CreateVertexBufferObject();
	GDeviceIndexBuffer* CreateIndexBufferObject();
	GDeviceShader* CreateShaderObject();
	
	void ReleaseResource(int id);
	void ReleaseResource(GDeviceBaseResource* pRes);
	void Release();

protected:
	int __addNewResourceId();
private:
	std::vector<GDeviceBaseResource*> resources;
	std::list<int> free_ids;
	thkit::Mutex thLock;
};

extern GD3DXResourceMgr devResMgr;