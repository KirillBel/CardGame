#pragma once

class GSerializable
{
public:
	GSerializable(void) {};
	virtual ~GSerializable(void) {};

	virtual bool Serialize(pugi::xml_node& node, bool bStore) = 0;
};

