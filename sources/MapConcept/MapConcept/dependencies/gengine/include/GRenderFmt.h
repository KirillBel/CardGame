#pragma once


namespace Render
{
	const int DEFAULT_VALUE = -1;
	const int BAD_ID = -1;

	enum eRenderSemantic
	{
		SEMANTIC_NONE=0,
		SEMANTIC_MATRIX_WVP,
		SEMANTIC_MATRIX_WORLD,
		SEMANTIC_MATRIX_VIEW,
		SEMANTIC_MATRIX_PROJECTION,
		SEMANTIC_MATRIX_WORLD_I,
		SEMANTIC_MATRIX_VIEW_I,
		SEMANTIC_MATRIX_PROJECTION_I,
		SEMANTIC_MATRIX_WORLD_IT,
		SEMANTIC_MATRIX_VIEW_IT,
		SEMANTIC_MATRIX_PROJECTION_IT,
		SEMANTIC_MATRIX_WORLD_VIEW,
		SEMANTIC_MATRIX_VIEW_PROJ,
		SEMANTIC_MATRIX_VIEW_PROJ_I,
		SEMANTIC_MATRIX_WORLD_VIEW_PROJ_I,
		SEMANTIC_VEC2_FRAME_SIZE,
		SEMANTIC_VEC2_VIEWPORT_SIZE,
		SEMANTIC_VEC3_EYE_POS,
		SEMANTIC_VEC3_LOOK,
		SEMANTIC_VEC3_UP,
		SEMANTIC_VEC3_RIGHT,
		SEMANTIC_VEC2_NEAR_FAR,
		SEMANTIC_FLOAT_TIMESEC,

		SEMANTIC_TEXTURE_DIFFUSE,
		SEMANTIC_TEXTURE_SPECULAR,
		SEMANTIC_TEXTURE_NORMAL,
		SEMANTIC_TEXTURE_GLOW,

		SEMANTIC_END
	};

	enum eCullMode
	{
		CULL_DEFAULT = Render::DEFAULT_VALUE,
		CULL_DISABLE,
		CULL_FRONT,
		CULL_BACK
	};

	enum eFillMode
	{
		FILL_DEFAULT = Render::DEFAULT_VALUE,
		FILL_SOLID,
		FILL_WIREFRAME,
		FILL_POINT,
	};

	enum eBoolMode
	{
		BOOL_DEFAULT = Render::DEFAULT_VALUE,
		BOOL_TRUE = 1,
		BOOL_FALSE = 0,
	};

	enum eTransparencyMode
	{
		TRANSPARENCY_DEFAULT = Render::DEFAULT_VALUE,
		TRANSPARENCY_OPAQUE,
		TRANSPARENCY_SOLID,
		TRANSPARENCY_TRANSLUCENT,
	};

	enum eLightType
	{
		LIGHTTYP_POINT,
		LIGHTTYP_SPOT,
		LIGHTTYP_DIRECTIONAL,
	};

	enum eBlendMode
	{
		BLEND_DEFAULT = Render::DEFAULT_VALUE,
		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRCCOLOR,
		BLEND_INVSRCCOLOR,
		BLEND_SRCALPHA,
		BLEND_INVSRCALPHA,
		BLEND_DESTALPHA,
		BLEND_INVDESTALPHA,
		BLEND_DESTCOLOR,
		BLEND_INVDESTCOLOR,
		BLEND_SRCALPHASAT,
		BLEND_BOTHSRCALPHA,
		BLEND_BOTHINVSRCALPHA,
	};

	enum eVertexStreamType
	{
		VSTREAM_XYZ,
		VSTREAM_XYZT,
		VSTREAM_UV,
		VSTREAM_NORMAL,
		VSTREAM_TANGENT,
		VSTREAM_BINORMAL,
		VSTREAM_COLOR,
		VSTREAM_PSIZE,
		VSTREAM_UNUSED
	};

	enum eBufferType
	{
		BUFTYP_STATIC,
		BUFTYP_DYNAMIC,
	};

	enum eMeshBufferType
	{
		MESHBUF_NONE,
		MESHBUF_VERTEX,
		MESHBUF_INDEX,
		MESHBUF_VERTEX_AND_INDEX,
	};

	enum eIndexFormat
	{
		INDEX_16I,
		INDEX_32I
	};

	enum eTextureType
	{
		TEXTYP_UNKNOWN,
		TEXTYP_2D,
		TEXTYP_3D,
		TEXTYP_CUBE,
	};

	enum eTextureUsage
	{
		TEXUSAGE_UNKNOWN,
		TEXUSAGE_BASE,
		TEXUSAGE_DYNAMIC,
		TEXUSAGE_COLOR_TARGET,
		TEXUSAGE_DEPTH_TARGET,
	};

	enum eTextureFmt
	{
		TEXFMT_UNKNOWN = 0,
		TEXFMT_A8,
		TEXFMT_R16F,
		TEXFMT_R32F,

		TEXFMT_R8G8B8,
		TEXFMT_A8R8G8B8,
		TEXFMT_X8R8G8B8,
		TEXFMT_DXT1,
		TEXFMT_DXT3,
		TEXFMT_DXT5,

		TEXFMT_D16,
		TEXFMT_D24S8,

		TEXFMT_D32,
		TEXFMT_LAST
	};

	enum ePrimitiveType
	{
		PRIM_DEFAULT = Render::DEFAULT_VALUE,
		PRIM_POINTLIST,
		PRIM_LINELIST,
		PRIM_LINESTRIP,
		PRIM_TRIANGLELIST,
		PRIM_TRIANGLESTRIP,
		PRIM_TRIANGLEFAN,
	};


	enum eCubeFace
	{
		CUBEFACE_POSX     = 0,
		CUBEFACE_NEGX     = 1,
		CUBEFACE_POSY     = 2,
		CUBEFACE_NEGY     = 3,
		CUBEFACE_POSZ     = 4,
		CUBEFACE_NEGZ     = 5,
	};

	const int LOCK_READONLY = 0x1;
	const int LOCK_DISCARD = 0x2;
	const int LOCK_NOOVERWRITE =0x4;


	const int CLEAR_COLOR = 0x1;
	const int CLEAR_DEPTH = 0x2;
	const int CLEAR_STENCIL =0x4;

	const int CLEAR_STD = (CLEAR_COLOR | CLEAR_DEPTH);

	struct sRenderState
	{
		sRenderState()
		{
			FillMode=FILL_DEFAULT;
			CullMode=CULL_DEFAULT;
			ZEnable=BOOL_DEFAULT;
			ZWriteEnable=BOOL_DEFAULT;
			AlphablendEnable=BOOL_DEFAULT;
			SrcBlend=BLEND_DEFAULT;
			DstBlend=BLEND_DEFAULT;
			TransparencyMode=TRANSPARENCY_DEFAULT;
			UpdateHash();
		};

		uint32 UpdateHash()
		{
			hash=0;

			int* pData=(int*)this;
			uint32 size=sizeof(*this)/4-1;

			for(uint32 i=0;i<size;i++)
			{
				hash|=(int)pData[i]<<i*4;
			};
			return hash;
		};

		eFillMode FillMode;
		eCullMode CullMode;
		eBoolMode ZEnable;
		eBoolMode ZWriteEnable;
		eBoolMode AlphablendEnable;

		eBlendMode SrcBlend;
		eBlendMode DstBlend;

		eTransparencyMode TransparencyMode;

		uint32 hash;

	};

	inline sRenderState SetWhenNotDefault(sRenderState& Old, sRenderState& New)
	{
		sRenderState NewState=Old;
		if(New.AlphablendEnable!=DEFAULT_VALUE) NewState.AlphablendEnable=New.AlphablendEnable;
		if(New.CullMode!=DEFAULT_VALUE) NewState.CullMode=New.CullMode;
		if(New.DstBlend!=DEFAULT_VALUE) NewState.DstBlend=New.DstBlend;
		if(New.FillMode!=DEFAULT_VALUE) NewState.FillMode=New.FillMode;
		if(New.SrcBlend!=DEFAULT_VALUE) NewState.SrcBlend=New.SrcBlend;
		if(New.ZEnable!=DEFAULT_VALUE) NewState.ZEnable=New.ZEnable;
		if(New.ZWriteEnable!=DEFAULT_VALUE) NewState.ZWriteEnable=New.ZWriteEnable;
		return NewState;
	};
};