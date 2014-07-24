#pragma once

class GSimpleMaterial : public GBaseMaterial
{
public:
	GSimpleMaterial(const char* name) : GBaseMaterial(name)
	{
	};

	~GSimpleMaterial(void) {};

	UINT GetPassCount() {return 1;};
	GRenderAppearance* GetPassAppearance(UINT nPass) {return &renderApp;};

	bool beginPass(UINT nPass) {return true;};
	bool endPass() {return true;};
	bool applyPass() {renderApp.apply(); return true;};

	virtual GRenderAppearance* getAppearance() {return &renderApp;};

	bool Serialize(pugi::xml_node& node, bool bStore);

protected:
	GRenderAppearance renderApp;
};