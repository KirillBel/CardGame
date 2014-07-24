#pragma once
class GMaterialMgr
{
public:
	GMaterialMgr(void);
	~GMaterialMgr(void);

	GMaterial* CreateMaterial(const char* name);
	GMaterial* CreateMaterial(pugi::xml_node& node);
	GMaterial* LoadMaterial(const char* path);


	GBaseMaterial* GetMaterial(const char* name);
	int GetMaterialIndex(GBaseMaterial* pMat);
	UINT GetMaterialCount() {return materials.size();};
	GBaseMaterial* GetMaterial(UINT index) {return materials[index];};

	void RemoveMaterial(GBaseMaterial* pMat);
protected:
	void InsertMaterial(GBaseMaterial* pMat);
	
protected:
	std::map<std::string,GBaseMaterial*> material_map;
	std::vector<GBaseMaterial*> materials;
	std::list<UINT> freeIds;
};

