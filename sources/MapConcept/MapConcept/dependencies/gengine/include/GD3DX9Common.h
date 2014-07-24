#pragma once

inline DWORD toDXCullMode(eCullMode Fmt)
{
	switch(Fmt)
	{
	case CULL_DISABLE: return D3DCULL_NONE;
	case CULL_FRONT: return  D3DCULL_CW;
	case CULL_BACK: return  D3DCULL_CCW;
	};
	return D3DCULL_NONE;
};

inline eCullMode fromDXCullMode(DWORD Fmt)
{
	switch(Fmt)
	{
	case D3DCULL_NONE: return CULL_DISABLE;
	case D3DCULL_CW: return  CULL_FRONT;
	case D3DCULL_CCW: return  CULL_BACK;
	};
	return CULL_DEFAULT;
};

inline DWORD toDXFillMode(eFillMode Fmt)
{
	switch(Fmt)
	{
	case FILL_SOLID: return D3DFILL_SOLID;
	case FILL_WIREFRAME: return  D3DFILL_WIREFRAME;
	case FILL_POINT: return  D3DFILL_POINT;
	};
	return D3DFILL_SOLID;
};

inline eFillMode fromDXFillMode(DWORD Fmt)
{
	switch(Fmt)
	{
	case D3DFILL_SOLID: return FILL_SOLID;
	case D3DFILL_WIREFRAME: return  FILL_WIREFRAME;
	case D3DFILL_POINT: return  FILL_POINT;
	};
	return FILL_SOLID;
};

inline DWORD toDXBlendStates(eBlendMode Value)
{

	switch(Value)
	{
	case BLEND_ZERO:				return D3DBLEND_ZERO;
	case BLEND_ONE:					return D3DBLEND_ONE;
	case BLEND_SRCCOLOR:			return D3DBLEND_SRCCOLOR;
	case BLEND_INVSRCCOLOR:			return D3DBLEND_INVSRCCOLOR;
	case BLEND_SRCALPHA:			return D3DBLEND_SRCALPHA;
	case BLEND_INVSRCALPHA:			return D3DBLEND_INVSRCALPHA;
	case BLEND_DESTALPHA:			return D3DBLEND_DESTALPHA;
	case BLEND_INVDESTALPHA:		return D3DBLEND_INVDESTALPHA;
	case BLEND_DESTCOLOR:			return D3DBLEND_DESTCOLOR;
	case BLEND_INVDESTCOLOR:		return D3DBLEND_INVDESTCOLOR;
	case BLEND_SRCALPHASAT:			return D3DBLEND_SRCALPHASAT;
	case BLEND_BOTHSRCALPHA:		return D3DBLEND_BOTHSRCALPHA;
	case BLEND_BOTHINVSRCALPHA:		return D3DBLEND_BOTHINVSRCALPHA;
	};

	return D3DBLEND_ONE;
};

inline eBlendMode fromDXBlendStates(DWORD Value)
{

	switch(Value)
	{
	case D3DBLEND_ZERO:				return BLEND_ZERO;
	case D3DBLEND_ONE:				return BLEND_ONE;
	case D3DBLEND_SRCCOLOR:			return BLEND_SRCCOLOR;
	case D3DBLEND_INVSRCCOLOR:		return BLEND_INVSRCCOLOR;
	case D3DBLEND_SRCALPHA:			return BLEND_SRCALPHA;
	case D3DBLEND_INVSRCALPHA:		return BLEND_INVSRCALPHA;
	case D3DBLEND_DESTALPHA:		return BLEND_DESTALPHA;
	case D3DBLEND_INVDESTALPHA:		return BLEND_INVDESTALPHA;
	case D3DBLEND_DESTCOLOR:		return BLEND_DESTCOLOR;
	case D3DBLEND_INVDESTCOLOR:		return BLEND_INVDESTCOLOR;
	case D3DBLEND_SRCALPHASAT:		return BLEND_SRCALPHASAT;
	case D3DBLEND_BOTHSRCALPHA:		return BLEND_BOTHSRCALPHA;
	case D3DBLEND_BOTHINVSRCALPHA:	return BLEND_BOTHINVSRCALPHA;
	};

	return BLEND_ONE;
};

inline D3DFORMAT toDXTextureFormat(eTextureFmt Fmt)
{
	switch(Fmt)
	{
	case TEXFMT_UNKNOWN:	return D3DFMT_UNKNOWN;
	case TEXFMT_A8:		return D3DFMT_A8;
	case TEXFMT_R16F:		return D3DFMT_R16F;
	case TEXFMT_R32F:		return D3DFMT_R32F;
	case TEXFMT_R8G8B8:	return D3DFMT_R8G8B8;
	case TEXFMT_A8R8G8B8: return D3DFMT_A8R8G8B8;
	case TEXFMT_X8R8G8B8:	return D3DFMT_X8R8G8B8;
	case TEXFMT_DXT1:		return D3DFMT_DXT1;
	case TEXFMT_DXT3:		return D3DFMT_DXT3;
	case TEXFMT_DXT5:		return D3DFMT_DXT5;

	case TEXFMT_D16:		return D3DFMT_D16;
	case TEXFMT_D24S8:	return D3DFMT_D24S8;

	case TEXFMT_D32:		return D3DFMT_D32;
	};
	return D3DFMT_UNKNOWN;
}

inline eTextureFmt fromDXTextureFormat(D3DFORMAT Fmt)
{
	switch(Fmt)
	{
	case D3DFMT_UNKNOWN:	return TEXFMT_UNKNOWN;
	case D3DFMT_A8:			return TEXFMT_A8;
	case D3DFMT_R16F:		return TEXFMT_R16F;
	case D3DFMT_R32F:		return TEXFMT_R32F;
	case D3DFMT_R8G8B8:		return TEXFMT_R8G8B8;
	case D3DFMT_A8R8G8B8:	return TEXFMT_A8R8G8B8;
	case D3DFMT_X8R8G8B8:	return TEXFMT_X8R8G8B8;
	case D3DFMT_DXT1:		return TEXFMT_DXT1;
	case D3DFMT_DXT3:		return TEXFMT_DXT3;
	case D3DFMT_DXT5:		return TEXFMT_DXT5;

	case D3DFMT_D16:		return TEXFMT_D16;
	case D3DFMT_D24S8:		return TEXFMT_D24S8;

	case D3DFMT_D32:		return TEXFMT_D32;
	};
	return TEXFMT_UNKNOWN;
}

inline eTextureUsage fromDXTextureUsage(DWORD Usage)
{
	if(Usage & D3DUSAGE_DEPTHSTENCIL)
		return TEXUSAGE_DEPTH_TARGET;
	if(Usage & D3DUSAGE_RENDERTARGET)
		return TEXUSAGE_COLOR_TARGET;
	if(Usage & D3DUSAGE_DYNAMIC)
		return TEXUSAGE_DYNAMIC;
	return TEXUSAGE_BASE;
};

inline DWORD toDXTextureUsage(eTextureUsage Usage)
{
	switch(Usage)
	{
	case TEXUSAGE_DEPTH_TARGET: return D3DUSAGE_DEPTHSTENCIL;
	case TEXUSAGE_COLOR_TARGET: return D3DUSAGE_RENDERTARGET;
	case TEXUSAGE_DYNAMIC:		return D3DUSAGE_DYNAMIC;
	};
	return 0;
};

inline DWORD toDXLockFlags(UINT flags)
{
	DWORD val=0;
	if(flags & LOCK_READONLY)
		val|=D3DLOCK_READONLY;
	if(flags & LOCK_DISCARD)
		val|=D3DLOCK_DISCARD;
	if(flags & LOCK_NOOVERWRITE)
		val|=D3DLOCK_NOOVERWRITE;
	return val;
};

inline D3DDECLTYPE toDXValueType(eValueType Fmt)
{
	switch(Fmt)
	{
	case VALUE_TYPE_FLOAT: return D3DDECLTYPE_FLOAT1;
	case VALUE_TYPE_VEC2F: return D3DDECLTYPE_FLOAT2;
	case VALUE_TYPE_VEC3F: return D3DDECLTYPE_FLOAT3;
	case VALUE_TYPE_VEC4F: return D3DDECLTYPE_FLOAT4;
	case VALUE_TYPE_COLORB: return D3DDECLTYPE_D3DCOLOR;
	case VALUE_TYPE_INT: return D3DDECLTYPE_SHORT4;
	case VALUE_TYPE_UINT: return D3DDECLTYPE_UBYTE4;
	case VALUE_TYPE_SHORT: return D3DDECLTYPE_SHORT2;
	};

	_ASSERT(false);
	return D3DDECLTYPE_UNUSED;
};

inline eValueType toSrcValueType(D3DDECLTYPE Fmt)
{
	switch(Fmt)
	{
	case D3DDECLTYPE_FLOAT1: return VALUE_TYPE_FLOAT;
	case D3DDECLTYPE_FLOAT2: return VALUE_TYPE_VEC2F;
	case D3DDECLTYPE_FLOAT3: return VALUE_TYPE_VEC3F;
	case D3DDECLTYPE_FLOAT4: return VALUE_TYPE_VEC4F;
	case D3DDECLTYPE_D3DCOLOR: return VALUE_TYPE_COLORB;
	case D3DDECLTYPE_SHORT4: return VALUE_TYPE_INT;
	case D3DDECLTYPE_UBYTE4: return VALUE_TYPE_UINT;
	case D3DDECLTYPE_SHORT2: return VALUE_TYPE_SHORT;
	};

	_ASSERT(false);
	return VALUE_TYPE_UNKNOWN;
};

inline D3DDECLUSAGE toDXVStreamType(eVertexStreamType Type)
{
	switch(Type)
	{
	case VSTREAM_XYZ: return D3DDECLUSAGE_POSITION;
	case VSTREAM_XYZT: return D3DDECLUSAGE_POSITIONT;
	case VSTREAM_UV: return D3DDECLUSAGE_TEXCOORD;
	case VSTREAM_NORMAL: return D3DDECLUSAGE_NORMAL;
	case VSTREAM_TANGENT: return D3DDECLUSAGE_TANGENT;
	case VSTREAM_BINORMAL: return D3DDECLUSAGE_BINORMAL;
	case VSTREAM_COLOR: return D3DDECLUSAGE_COLOR;
	case VSTREAM_PSIZE: return D3DDECLUSAGE_PSIZE;
	case VSTREAM_UNUSED: return D3DDECLUSAGE_SAMPLE;
	};
	return D3DDECLUSAGE_SAMPLE;
};

inline D3DPRIMITIVETYPE toDXPrimitiveType(ePrimitiveType Type)
{
	switch(Type)
	{
	case PRIM_POINTLIST: return D3DPT_POINTLIST;
	case PRIM_LINELIST: return D3DPT_LINELIST;
	case PRIM_LINESTRIP: return D3DPT_LINESTRIP;
	case PRIM_TRIANGLELIST: return D3DPT_TRIANGLELIST;
	case PRIM_TRIANGLESTRIP: return D3DPT_TRIANGLESTRIP;
	case PRIM_TRIANGLEFAN: return D3DPT_TRIANGLEFAN;
	};
	return D3DPT_FORCE_DWORD;
};

inline D3DFORMAT toDXIndexFormat(eIndexFormat Fmt)
{
	switch(Fmt)
	{
	case INDEX_16I: return D3DFMT_INDEX16;
	case INDEX_32I: return D3DFMT_INDEX32;
	};
	return D3DFMT_INDEX16;
}

/////////////////////////////////////////////////////

static Matrix44 DXConvertMatrix=Matrix44(1,0,0,0,	0,0,1,0,	0,1,0,0,	0,0,0,1);
static Matrix44 DXConvertMatrixInv=DXConvertMatrix.getInverted();
static Matrix44 SrcConvertMatrix=Matrix44(1,0,0,0,	0,0,1,0,	0,1,0,0,	0,0,0,1);
static Matrix44 SrcConvertMatrixInv=SrcConvertMatrix.getInverted();

inline void toDXMatrix(const Matrix44& SrcMatr,D3DXMATRIXA16& DestMatr)
{
	//Matrix44 mat=DXConvertMatrixInv*SrcMatr*DXConvertMatrix;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			DestMatr(i,j)=SrcMatr(i,j);
		};
	};
};

inline D3DXMATRIXA16 toDXMatrixA(Matrix44& SrcMatr)
{
	D3DXMATRIXA16 DestMatr;
	//Matrix44 mat=DXConvertMatrixInv*SrcMatr*DXConvertMatrix;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			DestMatr(i,j)=SrcMatr(i,j);
		};
	};
	return DestMatr;
};

inline void toDXMatrixNoCorrect(Matrix44& SrcMatr,D3DXMATRIXA16& DestMatr)
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			DestMatr(i,j)=SrcMatr(i,j);
		};
	};
};

inline void toSrcMatrix(D3DXMATRIXA16& SrcMatr,Matrix44& DestMatr)
{
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			DestMatr(i,j)=SrcMatr(i,j);
		};
	};

	//DestMatr=SrcConvertMatrixInv*DestMatr*SrcConvertMatrix;
};

//////////////////////////////////////////////////

inline IDirect3DVertexDeclaration9* toDXVertexDecl(GVertexDecl* pDecls, int size, LPDIRECT3DDEVICE9 pDevice)
{
	int numActiveDecls=0;
	int numElements=0;
	for(int i=0;i<size;i++)
	{
		if(pDecls[i].getNumElements())
		{
			numActiveDecls++;
			numElements+=pDecls[i].getNumElements();
		};
	};

	D3DVERTEXELEMENT9* DynVBElements=new D3DVERTEXELEMENT9[numElements+1];
	int nElem=0;

	for(int i=0;i<size;i++)
	{
		if(!pDecls[i].getNumElements())
			continue;

		for(int j=0;j<pDecls[i].getNumElements();j++)
		{
			_ASSERT(nElem<numElements);
			DynVBElements[nElem].Stream=i;
			DynVBElements[nElem].Offset=pDecls[i].getValue(j).Offset;
			DynVBElements[nElem].Type=toDXValueType(pDecls[i].getValue(j).ValType);
			DynVBElements[nElem].Method=D3DDECLMETHOD_DEFAULT;
			DynVBElements[nElem].Usage=toDXVStreamType(pDecls[i].getValue(j).StreamType);
			DynVBElements[nElem].UsageIndex=pDecls[i].getValue(j).usageIndex;
			nElem++;
		};
	};

	_ASSERT(nElem==numElements);
	DynVBElements[nElem].Stream=0xFF;
	DynVBElements[nElem].Offset=0;
	DynVBElements[nElem].Type=D3DDECLTYPE_UNUSED;
	DynVBElements[nElem].Method=0;
	DynVBElements[nElem].Usage=0;
	DynVBElements[nElem].UsageIndex=0;

	IDirect3DVertexDeclaration9* DxDecl=0;
	HRESULT hr=pDevice->CreateVertexDeclaration(DynVBElements, &DxDecl);
	delete [] DynVBElements;
	
	if(FAILED(hr))
	{
		LOGERROR("Can't create vertex declaration");
		return 0;
	};
	return DxDecl;
};

inline IDirect3DVertexDeclaration9* toDXVertexDecl(GVertexDecl& Decl, LPDIRECT3DDEVICE9 pDevice)
{
	D3DVERTEXELEMENT9* DynVBElements=new D3DVERTEXELEMENT9[Decl.getNumElements()+1];
	
	for(int i=0;i<Decl.getNumElements();i++)
	{
		DynVBElements[i].Stream=0;
		DynVBElements[i].Offset=Decl.getValue(i).Offset;
		DynVBElements[i].Type=toDXValueType(Decl.getValue(i).ValType);
		DynVBElements[i].Method=D3DDECLMETHOD_DEFAULT;
		DynVBElements[i].Usage=toDXVStreamType(Decl.getValue(i).StreamType);
		DynVBElements[i].UsageIndex=Decl.getValue(i).usageIndex;
	};
	int nElems=Decl.getNumElements();
	DynVBElements[nElems].Stream=0xFF;
	DynVBElements[nElems].Offset=0;
	DynVBElements[nElems].Type=D3DDECLTYPE_UNUSED;
	DynVBElements[nElems].Method=0;
	DynVBElements[nElems].Usage=0;
	DynVBElements[nElems].UsageIndex=0;

	IDirect3DVertexDeclaration9* DxDecl=0;
	HRESULT hr=pDevice->CreateVertexDeclaration(DynVBElements, &DxDecl);
	delete [] DynVBElements;
	
	if(FAILED(hr))
	{
		LOGERROR("Can't create vertex declaration");
		return 0;
	};
	return DxDecl;
};