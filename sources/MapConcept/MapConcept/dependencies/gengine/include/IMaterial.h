#pragma once
class GRenderAppearance;
class IMaterial : public GSerializable
{
public:
	IMaterial(void);
	virtual ~IMaterial(void);

	virtual int GetSubMtlCount() = 0;
	virtual IMaterial* GetSubMtl( int nMtl ) = 0;
	virtual void SetSubMtl( int nMat,IMaterial *pMat ) = 0;

	virtual bool isScreenQuadPass(UINT nPass) {return false;};

	virtual UINT GetPassCount() = 0;
	virtual GRenderAppearance* GetPassAppearance(UINT nPass) = 0;

	virtual void drawRE(GRenderElem* pElem, GRenderObject* pObj, bool bImmediate=false, eRIPassList pass=PASS_GENERAL, eRIList nList=RILIST_GENERAL, DWORD batchFlags=0);

	virtual bool Serialize(pugi::xml_node& node, bool bStore) = 0;
};

class GBaseMaterial : public IMaterial
{
public:
	GBaseMaterial(const char* name) 
	{
		this->name=name;
	};
	virtual ~GBaseMaterial(void) {}; 

	int GetSubMtlCount() {return sub_materials.size();};
	IMaterial* GetSubMtl( int nMtl ) {return (IMaterial*)sub_materials[nMtl];};
	void SetSubMtl( int nMat,IMaterial *pMat ) {sub_materials[nMat]=(IMaterial*)pMat;};

	virtual bool Serialize(pugi::xml_node& node, bool bStore) = 0;

	virtual std::string getName() {return name;};
protected:
	std::vector<IMaterial*> sub_materials;
	std::string name;
};



