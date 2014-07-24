#pragma once

#define FLAG_RO_CLIP_BBOX 0x1
#define FLAG_RO_CLIP_DISTANCE 0x2
#define FLAG_RO_DRAW_BBOX 0x4

class GRenderObject
{
public:
	GRenderObject(void) 
	{
		Init();
	};
	virtual ~GRenderObject(void) {}; 

	void Init()
	{
		worldMatrix.setIdentity();
		distance=0xFFFFFFFF;
		lightMask=0;
		lodLevel=0;
		id=-1;
		flags=0;
	};

	Matrix44 worldMatrix;
	float distance;
	UINT lightMask;
	BYTE lodLevel;
	int id;
	DWORD flags;
};

