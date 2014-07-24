#pragma once
class GCamera;

struct sFrustumPlane
{
	sFrustumPlane()
	{
		d=0;
	};

	sFrustumPlane(Vec3& normal,float d)
	{
		this->normal=normal;
		this->d=d;
	};

	sFrustumPlane(const Vec3& v0, const Vec3& v1, const Vec3& v2)
	{
		SetPlane(v0,v1,v2);
	};

	Vec3 normal;
	float d;

	void SetPlane(const Vec3& v0, const Vec3& v1, const Vec3& v2 ) {  
		normal = ((v1-v0)^(v2-v0)).getNormalized();
		d	=	-(normal | v0);
	};

	float GetDistFromPlane(const Vec3 &vPoint) {return ((normal|vPoint)+d);}

	inline sFrustumPlane operator - ( void ) const { return sFrustumPlane(-normal,-d); }
	inline sFrustumPlane operator - ( const sFrustumPlane &p) const { return sFrustumPlane( normal-p.normal, d-p.d); }
};

class GFrustum
{
public:
	GFrustum(void);
	~GFrustum(void);

	void set(GCamera& camera);
	void update(GCamera& camera);
	bool isBBoxVisible(AABB& bbox);
	AABB getBoundingBox();
	void draw();

protected:

	enum {
		FR_PLANE_FAR = 0,
		FR_PLANE_NEAR,
		FR_PLANE_LEFT,
		FR_PLANE_RIGHT,
		FR_PLANE_BOTTOM,
		FR_PLANE_TOP,
		FR_PLANE_COUNT
	};

	enum {
		FR_POINT_NLT = 0,
		FR_POINT_NRT,
		FR_POINT_NLB,
		FR_POINT_NRB,
		FR_POINT_FLT,
		FR_POINT_FRT,
		FR_POINT_FLB,
		FR_POINT_FRB,
		FR_POINT_COUNT
	};


	AABB boundingBox;
	Vec3 cameraPos;
	sFrustumPlane planes[FR_PLANE_COUNT];
	Vec3 points[FR_POINT_COUNT];

	Vec3 nearRT,farRT;

	uint32	m_idx1[FR_PLANE_COUNT],m_idy1[FR_PLANE_COUNT],m_idz1[FR_PLANE_COUNT]; 
	uint32	m_idx2[FR_PLANE_COUNT],m_idy2[FR_PLANE_COUNT],m_idz2[FR_PLANE_COUNT]; 
};

