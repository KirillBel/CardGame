#pragma once

enum eRIPassList
{
	PASS_GENERAL = 0,
	PASS_END
};

enum eRIList
{
	RILIST_INVALID = 0,
	RILIST_GENERAL,
	RILIST_TERRAIN,
	RILIST_DECAL,
	RILIST_SKY,
	RILIST_TRANSPARENT,
	RILIST_POSTPROCESS,
	RILIST_END
};

struct sRenderOp
{
	sRenderOp(GRenderElem* pElem, GRenderObject* pObj, GRenderAppearance* pAppearance, eRIPassList pass, eRIList nList, DWORD batchFlags)
	{
		this->pElem=pElem;
		this->pObj=pObj;
		this->pRA=pAppearance;

		this->batchFlags=batchFlags;
		this->lightMask=pObj->lightMask;

		this->nodeDist=pObj->distance;
		this->sortVal= pRA ? pRA->GetSortValue() : 0;
	};

	GRenderElem* pElem;
	GRenderObject* pObj;
	GRenderAppearance* pRA;

	DWORD batchFlags;
	float nodeDist;
	DWORD lightMask;
	DWORD sortVal;
	SHORT occQuery;
};


class GRenderPipeline
{
public:
	GRenderPipeline(void) 
	{
		Clear();
	};
	~GRenderPipeline(void) {};

	void add(GRenderElem* pElem, GRenderObject* pObj, GRenderAppearance* pApp, eRIPassList pass, eRIList nList, DWORD batchFlags)
	{
		sRenderOp* pRI=createItem(sRenderOp(pElem,pObj,pApp,pass,nList,batchFlags),pass,nList);
		BatchFlags[(int)pass][(int)nList]|=batchFlags;
	};

	sRenderOp* get(eRIPassList pass, eRIList nList, int Num);
	UINT getCount(eRIPassList pass, eRIList nList);

	void sortByDist(sRenderOp *pFirst, int Num, bool InvertedOrder=false);
	void sortByLight(sRenderOp *pFirst, int Num);

	void Clear()
	{
		for(int i=0;i<eRIPassList::PASS_END;i++)
		{
			for(int j=0;j<eRIList::RILIST_END;j++)
			{
				RenderItems[i][j].clear();
				BatchFlags[i][j]=0;
			};
		};
	};

protected:
	sRenderOp* createItem(sRenderOp& item, eRIPassList pass, eRIList nList)
	{
		std::vector<sRenderOp>& arr=RenderItems[(int)pass][(int)nList];
		int size=arr.size();
		arr.push_back(item);
		return &arr[size];
	};
protected:
	std::vector<sRenderOp> RenderItems[eRIPassList::PASS_END][eRIList::RILIST_END];
	DWORD BatchFlags[eRIPassList::PASS_END][eRIList::RILIST_END];
};