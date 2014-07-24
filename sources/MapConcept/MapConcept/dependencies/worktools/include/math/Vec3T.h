#pragma once

template <typename F> 
class Vec3T
{
public:
	inline Vec3T(void) : x(0),y(0),z(0) {};
	inline Vec3T( F vx, F vy, F vz ) : x(vx),y(vy),z(vz)	{assert(this->isValid());}
	inline Vec3T( F v ) : x(v),y(v),z(v)					{assert(this->isValid());}
	
	template <class T> 
	inline Vec3T( const Vec2T<T>& v ) : x((F)v.x), y((F)v.y), z(0)		{ assert(this->isValid()); }
	template <class T> 
	inline Vec3T( const Vec3T<T>& v ) : x((F)v.x), y((F)v.y), z((F)v.z) { assert(this->isValid()); }
	template <class T> 
	inline Vec3T( const Vec4T<T>& v ) : x((F)v.x),y((F)v.y),z((F)v.z)	{assert(this->isValid());}

	inline static Vec3T<F> CreateProjection( const Vec3T& i, const Vec3T& n ) { return i-n*(n|i); }
	inline static Vec3T<F> CreateReflection( const Vec3T& i, const Vec3T &n ) {return (n*(i|n)*2)-i;};
	inline static Vec3T<F> CreateLerp( const Vec3T<F> &p, const Vec3T<F> &q, F t )
	{
		const Vec3T<F> diff = q-p;
		return p+(diff*t);
	}

	inline Vec3T<F>& set(const F xval,const F yval, const F zval) 
	{ 
		x=xval; y=yval; z=zval; 
		assert(this->isValid()); 
		return *this; 
	};

//-------------------UTILS--------------------------------------------------
	inline F &operator [] (uint32 index)			{ assert(index<=2); return ((F*)this)[index]; }
	inline F operator [] (uint32 index) const		{ assert(index<=2); return ((F*)this)[index]; }

	inline bool isZero(F e = (F)0.0) const {return  (fabs(x) <= e) && (fabs(y) <= e) && (fabs(z) <= e);};

	bool isValid() const
	{
		if (!NumberValid(x)) return false;
		if (!NumberValid(y)) return false;
		if (!NumberValid(z)) return false;
		return true;
	}

	inline bool equals(const Vec3T<F> & v1, F epsilon=VEC_EPSILON) const 
	{
		assert(v1.isValid()); 
		assert(this->isValid()); 
		return  ((fabs(x-v1.x) <= epsilon) && (fabs(y-v1.y) <= epsilon) && (fabs(z-v1.z) <= epsilon));	
	}

//-------------------END UTILS-----------------------------------------------------

//-------------------OPERATORS-----------------------------------------------------
	inline Vec3T<F> &operator=(const Vec3T<F> &src) { x=src.x; y=src.y; z=src.z; return *this; }
	inline bool operator==(const Vec3T<F> &vec) {return (this->x==vec.x) && (this->y==vec.y) &&  (this->z==vec.z);};
	inline bool operator!=(const Vec3T<F> &vec) { return !(*this == vec); }

	//Add,Sub number
	inline Vec3T<F> operator-() const { return Vec3T<F>(-x,-y,-z); }
	inline Vec3T<F> operator+(F k) const { return Vec3T<F>(x+k,y+k,z+k); }
	inline Vec3T<F>& operator+=(F k) { x+=k;y+=k;z+=k; return *this; }
	inline Vec3T<F> operator-(F k) const { return Vec3T<F>(x-k,y-k,z-k); }
	inline Vec3T<F>& operator-=(F k) { x-=k;y-=k;z-=k; return *this; }

	//Mul,Div number
	inline Vec3T<F> operator*(F k) const { return Vec3T<F>(x*k,y*k,z*k); }
	inline Vec3T<F>& operator*=(F k) { x*=k;y*=k;z*=k; return *this; }
	inline Vec3T<F> operator/(F k) const { return Vec3T<F>(x/k,y/k,z/k); }
	inline Vec3T<F>& operator/=(F k) { x/=k;y/=k;z/=k; return *this; }

	//Add,Sub Vec3T
	inline Vec3T<F> operator+(const Vec3T &k) const { return Vec3T<F>(x+k.x,y+k.y,z+k.z); }
	inline Vec3T<F>& operator+=(const Vec3T &k) { x+=k.x;y+=k.y;z+=k.z; return *this; }
	inline Vec3T<F> operator-(const Vec3T &k) const { return Vec3T<F>(x-k.x,y-k.y,z-k.z); }
	inline Vec3T<F>& operator-=(const Vec3T &k) { x-=k.x;y-=k.y;z-=k.z; return *this; }

	//Mul,Div Vec3T
	inline Vec3T<F> operator*(const Vec3T &k) const { return Vec3T<F>(x*k.x,y*k.y,z*k.z); }
	inline Vec3T<F>& operator*=(const Vec3T &k) { x*=k.x;y*=k.y;z*=k.z; return *this; }
	inline Vec3T<F> operator/(const Vec3T &k) const { return Vec3T<F>(x/k.x,y/k.y,z/k.z); }
	inline Vec3T<F>& operator/=(const Vec3T &k) { x/=k.x;y/=k.y;z/=k.z; return *this; }

	//Dot,Cross
	inline F operator|(const Vec3T &k) const { return this->dot(k); }
	inline F& operator|=(const Vec3T &k) { *this=this->dot(k); return *this; }
	inline Vec3T<F> operator^(const Vec3T &k) const { return this->cross(k); }
	inline Vec3T<F>& operator^=(const Vec3T &k) { *this=this->cross(k); return *this; }

//----------------END OPERATORS----------------------------------------------------

//-------------------BASIC----------------------------------------------------------
	inline void setLength(F fLen)
	{ 
		F fLenMe = getLengthSquared();
		if(fLenMe<0.00001f*0.00001f)
			return;
		fLenMe = fLen * matkit::isqrt(fLenMe);
		x*=fLenMe; y*=fLenMe; z*=fLenMe;
	}

	inline void clampLength(F maxLength)
	{
		F sqrLength = GetLengthSquared();
		if (sqrLength > (maxLength * maxLength))
		{
			F scale = maxLength * matkit::isqrt(sqrLength);
			x *= scale; y *= scale; z *= scale;
		}
	}

	inline F	getLength() const {return sqrtf(x*x+y*y+z*z);};
	inline F	getLengthSquared() const {return x*x+y*y+z*z;};
	inline F	getLength2D() const {return sqrtf(x*x+y*y);};	
	inline F	getLengthSquared2D() const {return x*x+y*y;};

	inline F getDistance(const Vec3T<F> vec1) const
	{
		return  sqrtf((x-vec1.x)*(x-vec1.x)+(y-vec1.y)*(y-vec1.y)+(z-vec1.z)*(z-vec1.z)); 
	}
	inline F getDistanceSquared ( const Vec3T<F> &v) const
	{		
		return  (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z); 
	}
	inline F getDistanceSquared2D ( const Vec3T<F> &v) const
	{		
		return  (x-v.x)*(x-v.x) + (y-v.y)*(y-v.y);
	}

	inline void	normalize() 
	{
		assert(this->isValid()); 
		F fInvLen = matkit::isqrt( x*x+y*y+z*z );
		x*=fInvLen; y*=fInvLen; z*=fInvLen; 
	}

	inline Vec3T getNormalized() const 
	{ 
		F fInvLen = matkit::isqrt( x*x+y*y+z*z );
		return *this * fInvLen;
	}

	inline F getVolume() const { return x*y*z; }

	inline Vec3T<F> abs() const {return Vec3T(fabs(x),fabs(y),fabs(z));};

	inline void minBounds(const Vec3T<F> &other)
	{ 
		x = matkit::min_tpl(other.x,x);
		y = matkit::min_tpl(other.y,y);
		z = matkit::min_tpl(other.z,z);
	}

	inline void maxBounds(const Vec3T<F> &other)
	{
		x = matkit::max_tpl(other.x,x);
		y = matkit::max_tpl(other.y,y);
		z = matkit::max_tpl(other.z,z);
	}

	inline void setProjection( const Vec3T& i, const Vec3T& n ) { *this = i-n*(n|i); }
	inline void setReflection( const Vec3T& i, const Vec3T &n ) {*this=(n*(i|n)*2)-i;};
	
	inline void setLerp( const Vec3T<F> &p, const Vec3T<F> &q, F t )
	{
		const Vec3T<F> diff = q-p;
		*this = p + (diff*t);
	}
	
	inline Vec3T getRotated(const Vec3T &axis, F angle) const
	{ 
		return getRotated(axis,cos(angle),sin(angle)); 
	}

	inline Vec3T getRotated(const Vec3T &axis, F cosa,F sina) const {
		Vec3T zax = axis*(*this|axis); 
		Vec3T xax = *this-zax; 
		Vec3T yax = axis.cross(xax);
		return xax*cosa + yax*sina + zax;
	}

	inline Vec3T getRotated(const Vec3T &center,const Vec3T &axis, F angle) const { 
		return center+(*this-center).GetRotated(axis,angle); 
	}

	inline Vec3T getRotated(const Vec3T &center,const Vec3T &axis, F cosa,F sina) const { 
		return center+(*this-center).GetRotated(axis,cosa,sina); 
	}

	inline Vec3T compMul( const Vec3T& rhs ) const { 
		return( Vec3T( x * rhs.x, y * rhs.y, z * rhs.z ) ); 
	}

	inline F dot (const Vec3T<F> & v)	const
	{
		const Vec3T<F> vec2 = v;
		return x*vec2.x + y*vec2.y + z*vec2.z;
	}

	inline Vec3T<F> cross (const Vec3T<F> & vec2) const
	{
		return Vec3T<F>( y*vec2.z  -  z*vec2.y,     z*vec2.x -    x*vec2.z,   x*vec2.y  -  y*vec2.x);
	}

	inline static Vec3T<F> random()
	{
		return Vec3T<F>((F)rand(),(F)rand(),(F)rand());
	};

	inline static Vec3T<F> random(F min, F max)
	{
		return (random() / (RAND_MAX + 1.0) * (max - min)
				+ min);
	};

public:
	F x,y,z;
};

typedef Vec3T<f32>		Vec3;
typedef Vec3T<int32>	Vec3i;
typedef Vec3T<f64>		Vec3r;
