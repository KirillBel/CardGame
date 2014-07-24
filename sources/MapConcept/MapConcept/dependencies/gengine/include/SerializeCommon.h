#pragma once

namespace GSerialize
{
	bool SerializeObject(pugi::xml_node& node, bool bStore, sRenderState& rs);
	bool SerializeObject(pugi::xml_node& node, bool bStore, sShaderItem& shader);
	bool SerializeObject(pugi::xml_node& node, bool bStore, sShaderResources* pRes);
	bool SerializeObject(pugi::xml_node& node, bool bStore, eFillMode& val);
	bool SerializeObject(pugi::xml_node& node, bool bStore, eCullMode& val);
	bool SerializeObject(pugi::xml_node& node, bool bStore, eBoolMode& val);
	bool SerializeObject(pugi::xml_node& node, bool bStore, eBlendMode& val);
	bool SerializeObject(pugi::xml_node& node, bool bStore, eTransparencyMode& val);

	const char* ToString(eFillMode val);
	const char* ToString(eCullMode val);
	const char* ToString(eBoolMode val);
	const char* ToString(eBlendMode val);
	const char* ToString(eTransparencyMode val);

	bool FromString(const char* str, eFillMode& val);
	bool FromString(const char* str, eCullMode& val);
	bool FromString(const char* str, eBoolMode& val);
	bool FromString(const char* str, eBlendMode& val);
	bool FromString(const char* str, eTransparencyMode& val);

	bool SerializeObjectDecl(pugi::xml_node& node, bool bStore, GTexture** ppTexture);
};