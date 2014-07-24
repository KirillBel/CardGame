#pragma once

class AABB {

public:
	Vec3 min;
	Vec3 max;

	AABB() {};
	virtual ~AABB() {}

	inline AABB( float radius ) 
	{ 
		max = Vec3(radius); 
		min = -max; 
	}
	inline  AABB( const Vec3& v ) 
	{ 
		min = max = v; 
	};
	inline  AABB( const Vec3& v, float radius ) 
	{ 
		Vec3 ext(radius); 
		min = v-ext; 
		max = v+ext; 
	}
	inline  AABB( const Vec3 &vmin, const Vec3 &vmax ) 
	{ 
		min=vmin; 
		max=vmax; 
	}

	inline  AABB( const AABB &aabb ) 
	{
		min.x = aabb.min.x;			min.y = aabb.min.y;			min.z = aabb.min.z;
		max.x = aabb.max.x;			max.y = aabb.max.y;			max.z = aabb.max.z;
	}
	inline AABB( const Vec3* points, int num  )
	{
		reset();
		for ( int i=0; i<num; i++ )
			add( points[i] );
	}


	inline void reset()	{	min = Vec3(1e15f);	max = Vec3(-1e15f);	}
	inline bool isReset() const { return min.x > max.x; }
	inline float isResetSel(float ifReset, float ifNotReset) const { return (float)matkit::fsel(max.x - min.x, ifNotReset, ifReset); }
	inline bool isEmpty() const { return (Vec3(min)==Vec3(max)); }

	inline Vec3 getCenter() const { return (min+max)*0.5f; }
	inline Vec3 getSize() const { return (max-min) * isResetSel(0.0f, 1.0f); }
	inline float getRadius() const { return isResetSel(0.0f, (max-min).getLength()*0.5f); }
	inline float getRadiusSqr() const { return isResetSel(0.0f, ((max-min)*0.5f).getLengthSquared()); }
	inline float getVolume() const { return isResetSel(0.0f, (max.x-min.x) * (max.y-min.y) * (max.z-min.z)); }

	inline void add( const Vec3 &v )
	{
		min.minBounds(v);
		max.maxBounds(v);
	}

	inline void add( const Vec3& v, float radius )
	{
		Vec3 ext(radius);
		min.minBounds(v-ext);
		max.maxBounds(v+ext);
	}

	inline void add( const AABB& bb )
	{
		min.minBounds(bb.min);
		max.maxBounds(bb.max);
	}

	inline void move( const Vec3& v )
	{
		const float moveMult = isResetSel(0.0f, 1.0f);
		const Vec3 vMove = v * moveMult;
		min += vMove;
		max += vMove;
	}

	inline void setPos( const Vec3& v )
	{
		move(v-getCenter());
	}

	inline void expand( Vec3 const& v )
	{
		if (!isReset())
		{
			min -= v;
			max += v;
		}
	}

	inline Vec3 size()
	{
		return Vec3(abs(max.x-min.x),abs(max.y-min.y),abs(max.z-min.z));
	};

	inline void divide( const float& Val )
	{
		max/=Val;
		min/=Val;
	}

	bool isContainSphere( const Vec3 &pos,float radius ) const
	{
		if (pos.x-radius < min.x) return false;
		if (pos.y-radius < min.y) return false;
		if (pos.z-radius < min.z) return false;
		if (pos.x+radius > max.x) return false;
		if (pos.y+radius > max.y) return false;
		if (pos.z+radius > max.z) return false;
		return true;
	}

	//! Check if this bounding box contains a point within itself.
	bool isContainPoint( const Vec3 &pos) const
	{
		if (pos.x < min.x) return false;
		if (pos.y < min.y) return false;
		if (pos.z < min.z) return false;
		if (pos.x > max.x) return false;
		if (pos.y > max.y) return false;
		if (pos.z > max.z) return false;
		return true;
	};

	bool isContainsBox( AABB const& b ) const
	{
		return min.x <= b.min.x && min.y <= b.min.y && min.z <= b.min.z
				&& max.x >= b.max.x && max.y >= b.max.y && max.z >= b.max.z;
	}

	inline bool	isIntersectBox( const AABB &b ) const	
	{
		// Check for intersection on X axis.
		if ((min.x > b.max.x)||(b.min.x > max.x)) return false;
		// Check for intersection on Y axis.
		if ((min.y > b.max.y)||(b.min.y > max.y)) return false;
		// Check for intersection on Z axis.
		if ((min.z > b.max.z)||(b.min.z > max.z)) return false;
		// Boxes overlap in all 3 axises.
		return true;
	}

	float getDistanceSqr( Vec3 const& v ) const
	{
		Vec3 vNear = v;
		vNear.maxBounds(min);
		vNear.minBounds(max);
		return vNear.getDistanceSquared(v);
	}

	float getDistance( Vec3 const& v ) const
	{
		return sqrt(getDistanceSqr(v));
	}
};

inline bool equals( const AABB& a, const AABB& b, float epsilon=VEC_EPSILON )
{
	return equals(a.min, b.min, epsilon) && equals(a.max, b.max, epsilon);
}