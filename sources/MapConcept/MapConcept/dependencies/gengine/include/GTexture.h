#pragma once

inline const char* ToStrTextureFmt(eTextureFmt fmt)
{
	switch(fmt)
	{
	SWCASE_ENUM_MACRO(TEXFMT_UNKNOWN);
	SWCASE_ENUM_MACRO(TEXFMT_A8);
	SWCASE_ENUM_MACRO(TEXFMT_R16F);
	SWCASE_ENUM_MACRO(TEXFMT_R32F);
	SWCASE_ENUM_MACRO(TEXFMT_R8G8B8);
	SWCASE_ENUM_MACRO(TEXFMT_A8R8G8B8);
	SWCASE_ENUM_MACRO(TEXFMT_X8R8G8B8);
	SWCASE_ENUM_MACRO(TEXFMT_DXT1);
	SWCASE_ENUM_MACRO(TEXFMT_DXT3);
	SWCASE_ENUM_MACRO(TEXFMT_DXT5);
	SWCASE_ENUM_MACRO(TEXFMT_D16);
	SWCASE_ENUM_MACRO(TEXFMT_D24S8);
	SWCASE_ENUM_MACRO(TEXFMT_D32);
	};
	return "TEXFMT_UNKNOWN";
};

inline eTextureFmt FromStrTextureFmt(const char* str)
{
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_UNKNOWN);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_A8);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_R16F);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_R32F);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_R8G8B8);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_A8R8G8B8);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_X8R8G8B8);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_DXT1);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_DXT3);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_DXT5);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_D16);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_D24S8);
	SWCASE_ENUM_MACRO_INV(str,TEXFMT_D32);

	return TEXFMT_UNKNOWN;
};

inline const char* ToStrTextureType(eTextureType type)
{
	switch(type)
	{
	SWCASE_ENUM_MACRO(TEXTYP_UNKNOWN);
	SWCASE_ENUM_MACRO(TEXTYP_2D);
	SWCASE_ENUM_MACRO(TEXTYP_3D);
	SWCASE_ENUM_MACRO(TEXTYP_CUBE);
	};
	return "TEXTYP_UNKNOWN";
}

inline eTextureType FromStrTextureType(const char* str)
{
	SWCASE_ENUM_MACRO_INV(str,TEXTYP_UNKNOWN);
	SWCASE_ENUM_MACRO_INV(str,TEXTYP_2D);
	SWCASE_ENUM_MACRO_INV(str,TEXTYP_3D);
	SWCASE_ENUM_MACRO_INV(str,TEXTYP_CUBE);
	return TEXTYP_UNKNOWN;
}

inline const char* ToStrTextureUsage(eTextureUsage usg)
{
	switch(usg)
	{
	SWCASE_ENUM_MACRO(TEXUSAGE_UNKNOWN);
	SWCASE_ENUM_MACRO(TEXUSAGE_BASE);
	SWCASE_ENUM_MACRO(TEXUSAGE_DYNAMIC);
	SWCASE_ENUM_MACRO(TEXUSAGE_COLOR_TARGET);
	SWCASE_ENUM_MACRO(TEXUSAGE_DEPTH_TARGET);
	};
	return "TEXUSAGE_UNKNOWN";
};

inline eTextureUsage FromStrTextureUsage(const char* str)
{
	SWCASE_ENUM_MACRO_INV(str,TEXUSAGE_UNKNOWN);
	SWCASE_ENUM_MACRO_INV(str,TEXUSAGE_BASE);
	SWCASE_ENUM_MACRO_INV(str,TEXUSAGE_DYNAMIC);
	SWCASE_ENUM_MACRO_INV(str,TEXUSAGE_COLOR_TARGET);
	SWCASE_ENUM_MACRO_INV(str,TEXUSAGE_DEPTH_TARGET);
	return TEXUSAGE_UNKNOWN;
};

//struct sTextureData
//{
//	bool bLoaded;
//	bool bAsyncLoad;
//	std::string textureName;
//
//	std::string SrcFile;
//	eTextureType type;
//	UINT nWidth;
//	UINT nHeight;
//	UINT nDepth;
//	UINT MipLevels;
//	eTextureFmt fmt;
//	eTextureUsage usage;
//};

class GTexture : public GResource
{
public:
	GTexture(void);
	~GTexture(void);


	static GTexture* CreateTextureObject(const char* name, bool& bFound);
	static GTexture* CreateTextureFromName(const char* name,bool bAsync, eTextureType Type, UINT nWidth=0, UINT nHeight=0, UINT nDepth=0, UINT MipLevels=1,eTextureFmt eTF=TEXFMT_A8R8G8B8, eTextureUsage Assign=TEXUSAGE_BASE);
	static GTexture* CreateTexture(const char* name,eTextureType Type, UINT nWidth, UINT nHeight, UINT nDepth, UINT MipLevels=1,eTextureFmt eTF=TEXFMT_A8R8G8B8, eTextureUsage Assign=TEXUSAGE_BASE);
	static GTexture* CreateTexture(const char* name,GDeviceTexture* pDeviceTexture);
	static GTexture* CreateColorTarget(const char* name, UINT nWidth, UINT nHeight, eTextureFmt eTF=TEXFMT_A8R8G8B8, UINT MipLevels=1);
	static GTexture* CreateDepthTarget(const char* name, UINT nWidth, UINT nHeight, eTextureFmt eTF);
	static GTexture* CreateTexture2D(const char* name, UINT nWidth, UINT nHeight, UINT MipLevels,eTextureFmt eTF, eTextureUsage Assign);
	static GTexture* CreateTexture3D(const char* name, UINT nWidth, UINT nHeight,UINT nDepth, UINT MipLevels,eTextureFmt eTF, eTextureUsage Assign);
	static GTexture* CreateTextureCube(const char* name, UINT nSize, UINT MipLevels,eTextureFmt eTF, eTextureUsage Assign);
	static GTexture* LoadTexture(const char* name,eTextureType Type, UINT nWidth=0, UINT nHeight=0,UINT nDepth=0,eTextureFmt eTF=TEXFMT_UNKNOWN,UINT MipLevels=0, bool bAsyncLoad=false);
	static GTexture* LoadTexture2D(const char* path,int MipLevels=4,eTextureFmt eTF=TEXFMT_A8R8G8B8, bool bAsyncLoad=false);
	static GTexture* CreateFromXML(pugi::xml_node& node, bool bFailIfNameNotFound);

	static GTexture* GetTexture(const char* name);
	static GTexture* GetTexture(RCID id);

	void SetTexture(eTextureType Type, UINT nWidth, UINT nHeight, UINT nDepth, UINT MipLevels=1,eTextureFmt eTF=TEXFMT_A8R8G8B8, eTextureUsage Assign=TEXUSAGE_BASE);
	void SetTexture(GDeviceTexture* pTex);
	bool Create();
	bool Load(bool bAsyncLoad=false);
	bool Load(const char* path,bool bAsyncLoad=false);
	//Device dependent
	
	int getWidth() const;
	int getHeight() const;
	int getDepth() const;
	int getMipLevels() const;
	eTextureType getTextureType() const;
	eTextureUsage getTextureUsage() const;
	eTextureFmt getTextureFormat() const;
	bool isColorTarget() const;
	bool isDepthTarget() const;
	GDeviceTexture* getDeviceTexture() {return pDeviceTexture;};
	BYTE* Lock2D(UINT level, int& pitch, UINT flags);
	BYTE* Lock3D(UINT level, int& rowPitch,int& slicePitch, UINT flags);
	BYTE* LockCube(eCubeFace nFace,int nLevel, int& pitch, UINT flags);
	void Unlock(UINT level,eCubeFace nFace=CUBEFACE_POSX);
	bool Save(const char* path);

	bool IsDefaultTexture() const {return bDefaultTexture;};
	bool IsLoaded() const {return bLoaded;};
	bool IsAsyncLoad() const {return bAsyncLoad;};
	const char* GetLoadPath() const {return SrcFile.c_str();};

	//Resource specific
	const char* GetName() const {return textureName.c_str();};
	const char* GetClassNameStr() const;
	int Release();
	void Invalidate();
	bool Restore();
	bool IsValid() const;
	GResource* Clone(const char* newName);
	std::string toString() const;

	//other
	bool waitForTextureReady(DWORD MaxTime);

	//serialize
	bool SaveXML(pugi::xml_node& node, bool bOnlyName=false);

	void SetToDefaultTexture(GTexture* pDefaultTex=pDefaultTexture);
protected:
	static GTexture* pDefaultTexture;
protected:
	bool bDefaultTexture;
	GDeviceTexture* pDeviceTexture;
	bool bLoaded;
	bool bAsyncLoad;
	bool bResizeToScreen;
	std::string textureName;

	std::string SrcFile;
	eTextureType type;
	UINT nWidth;
	UINT nHeight;
	UINT nDepth;
	UINT MipLevels;
	eTextureFmt fmt;
	eTextureUsage usage;
};

