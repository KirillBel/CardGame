#pragma once

class GShader;

struct sShaderResources
{
	sShaderResources()
	{
		bUpdateCacheBlock=false;
		shaderParamBlockId.QuadPart=-1;
	};

	ValueList params;
	bool bUpdateCacheBlock;
	HSHADERCACHEBLOCK shaderParamBlockId;
};

struct sShaderItem
{
	sShaderItem()
	{
		pShader=NULL;
		pResources=NULL;
		passMask=0xFFFFFFFF;
		nTechnique=-1;
	};

	sShaderItem(GShader* pShader)
	{
		this->pShader=pShader;
		this->pResources=NULL;
		passMask=0xFFFFFFFF;
		nTechnique=-1;
	};

	sShaderItem(GShader* pShader, sShaderResources* pResources)
	{
		this->pShader=pShader;
		this->pResources=pResources;
		passMask=0xFFFFFFFF;
		nTechnique=-1;
	};

	GShader* pShader;
	sShaderResources* pResources;
	UINT passMask;
	int nTechnique;
};