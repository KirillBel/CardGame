#pragma once




class GRenderSemantic
{
public:
	GRenderSemantic(void);
	~GRenderSemantic(void);

	eRenderSemantic FromString(const char* str);
	const char* ToString(eRenderSemantic val);

	eValueType GetType(eRenderSemantic val);

	bool SetValue(eRenderSemantic id, ValueData& val);
	ValueData& GetValue(eRenderSemantic id);
	bool SetValue(const char* name, ValueData& val);
	ValueData& GetValue(const char* name);

protected:
	void Init();
	void Insert(eRenderSemantic id, const char* name, eValueType type);
protected:
	const char* SemanticNames[SEMANTIC_END];
	eValueType SemanticTypes[SEMANTIC_END];
	ValueData SemanticData[SEMANTIC_END];
	std::map<UINT,eRenderSemantic> StringMap;
};

