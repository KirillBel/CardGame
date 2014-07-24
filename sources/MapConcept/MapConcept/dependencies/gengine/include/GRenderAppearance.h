#pragma once

class GRenderCallback
{
public:
	GRenderCallback(void) {};
	virtual ~GRenderCallback(void) {};
};

#define RA_SORT_NONE				0x0
#define RA_SORT_DISTANCE			0x1
#define RA_SORT_INVERT_DISTANCE		0x2
#define RA_SORT_DISTANCE_INVERTED	RA_SORT_DISTANCE_INVERTED | RA_SORT_INVERT_DISTANCE
#define RA_SORT_SHADER				0x4
#define RA_SORT_RT					0x5

class GRenderAppearance : public GSerializable
{
public:
	GRenderAppearance(void);
	virtual ~GRenderAppearance(void);

	void SetShader(GShader* pShader);

	sShaderResources* GetShaderResources();
	ValueList& GetShaderParams(bool bSetUpdateFlag);

	sShaderItem& GetShaderItem();
	GShader* GetShader();
	UINT GetShaderPassMask();
	int GetShaderTechnique();

	void SetTexture(UINT nSlot, GTexture* pTex);
	void SetColorTarget(UINT nSlot, GTexture* pTex);
	void SetDepthTarget(GTexture* pTex);

	sRenderState& GetRenderStates();
	void SetRenderStates(sRenderState& rs);

	void SetRenderCallback(GRenderCallback* pCallback);

	DWORD GetSortFlags();
	void SetSortFlags(DWORD flags);

	UINT GetSortValue();

	void apply();

	//serialize
	bool Serialize(pugi::xml_node& node, bool bStore);
protected:
	sShaderItem shader;
	sRenderState states;
	std::vector<GTexture*> textures;
	std::vector<GTexture*> color_targets;
	GTexture* depth_target;
	DWORD sortFlags;
	DWORD batchFlags;

	GRenderCallback* pRenderCallback;
};

