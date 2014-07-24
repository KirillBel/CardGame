#pragma once


namespace FreeImageFunc
{
	void Initialize();
	void Release();

	FREE_IMAGE_FORMAT CheckImageFile(const char* path,eTextureType Type);
	FIBITMAP* GenericLoader(const char* lpszPathName,eTextureType Type, int flag);
	FIBITMAP* ConvertToFmt(FIBITMAP* src, eTextureFmt eTF);

	bool LoadToDeviceTexture2D(GDeviceTexture* pObject, const char* path,UINT nWidth, UINT nHeight,UINT MipLevels=1,eTextureFmt eTF=TEXFMT_A8R8G8B8);
};