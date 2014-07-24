#pragma once

template <typename F> 
class Vec4T
{
public:
	inline Vec4T() : x(0),y(0),z(0) {};
	inline Vec4T( F vx, F vy, F vz, F vw ) : x(vx),y(vy),z(vz),w(vw) {};

	template <class T> 
	inline Vec4T( const Vec2T<T>& v ) : x((F)v.x), y((F)v.y), z(0), w(0)		{ assert(this->isValid()); }
	template <class T> 
	inline Vec4T( const Vec3T<T>& v ) : x((F)v.x), y((F)v.y), z((F)v.z), w(0)	{ assert(this->isValid()); }
	template <class T> 
	inline Vec4T( const Vec4T<T>& v ) : x((F)v.x),y((F)v.y),z((F)v.z),w((F)v.w)	{assert(this->isValid());}

	inline static Vec4T<F> CreateLerp( const Vec4T<F> &p, const Vec4T<F> &q, F t ) {	return p*(1.0f-t) + q*t;}

	inline Vec4T<F>& set(const F xval,const F yval, const F zval, const F wval) 
	{ 
		x=xval; y=yval; z=zval; w=wval; 
		assert(this->isValid()); 
		return *this; 
	}

//----------------------UTILS--------------------------------------------------
	inline F &operator [] (UINT index)		{ assert(index<=3); return ((F*)this)[index]; }
	inline F operator [] (UINT index) const { assert(index<=3); return ((F*)this)[index]; }

	bool isValid() const
	{
		if (!NumberValid(x)) return false;
		if (!NumberValid(y)) return false;
		if (!NumberValid(z)) return false;
		if (!NumberValid(w)) return false;
		return true;
	};

	inline bool equals(const Vec4T<F> & v1, F epsilon=VEC_EPSILON) const 
	{
		assert(v1.isValid()); 
		assert(this->isValid()); 
		return  ((fabs(x-v1.x) <= epsilon) && (fabs(y-v1.y) <= epsilon) && (fabs(z-v1.z) <= epsilon) && (fabs(w-v1.w) <= epsilon));	
	}

//-------------------END UTILS-----------------------------------------------------

//-------------------OPERATORS-----------------------------------------------------
	template <class T> 
	inline Vec4T<F>& operator = (const Vec4T<T> &v1) 
	{
		(*this).x=F(v1.x); (*this).y=F(v1.y); (*this).z=F(v1.z); (*this).w=F(v1.w);
		return (*this);
	}
	inline bool operator==(const Vec4T<F> &vec)
	{
		return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
	}
	inline bool operator!=(const Vec4T<F> &vec) { return !(*this == vec); }

	//Add,Sub number
	inline Vec4T<F> operator-() const { return Vec4T<F>(-x,-y,-z,-w); }
	inline Vec4T<F> operator+(F k) const { return Vec4T<F>(x+k,y+k,z+k,w+k); }
	inline Vec4T<F>& operator+=(F k) { x+=k;y+=k;z+=k;w+=k; return *this; }
	inline Vec4T<F> operator-(F k) const { return Vec4T<F>(x-k,y-k,z-k,w-k); }
	inline Vec4T<F>& operator-=(F k) { x-=k;y-=k;z-=k;w-=k; return *this; }

	//Mul,Div number
	inline Vec4T<F> operator*(F k) const { return Vec4T<F>(x*k,y*k,z*k,w*k); }
	inline Vec4T<F>& operator*=(F k) { x*=k;y*=k;z*=k;w*=k; return *this; }
	inline Vec4T<F> operator/(F k) const { return Vec4T<F>(x/k,y/k,z/k,w/k); }
	inline Vec4T<F>& operator/=(F k) { x/=k;y/=k;z/=k;w/=k; return *this; }

	//Add,Sub Vec4T
	inline Vec4T<F> operator+(const Vec4T &k) const { return Vec4T<F>(x+k.x,y+k.y,z+k.z,w+k.w); }
	inline Vec4T<F>& operator+=(const Vec4T &k) { x+=k.x;y+=k.y;z+=k.z;w+=k.w; return *this; }
	inline Vec4T<F> operator-(const Vec4T &k) const { return Vec4T<F>(x-k.x,y-k.y,z-k.z,w-k.w); }
	inline Vec4T<F>& operator-=(const Vec4T &k) { x-=k.x;y-=k.y;z-=k.z;w-=k.w; return *this; }

	//Mul,Div Vec4T
	inline Vec4T<F> operator*(const Vec4T &k) const { return Vec4T<F>(x*k.x,y*k.y,z*k.z,w*k.w); }
	inline Vec4T<F>& operator*=(const Vec4T &k) { x*=k.x;y*=k.y;z*=k.z;w*=k.w; return *this; }
	inline Vec4T<F> operator/(const Vec4T &k) const { return Vec4T<F>(x/k.x,y/k.y,z/k.z,w/k.w); }
	inline Vec4T<F>& operator/=(const Vec4T &k) { x/=k.x;y/=k.y;z/=k.z;w/=k.w; return *this; }

	//Dot,Cross
	inline F operator|(const Vec4T &k) const { return this->dot(k); }
	inline F& operator|=(const Vec4T &k) { *this=this->dot(k); return *this; }

//----------------END OPERATORS----------------------------------------------------

//-------------------BASIC----------------------------------------------------------
	inline F dot (const Vec4T<F> &vec2)	const	
	{ 
		return x*vec2.x + y*vec2.y + z*vec2.z + w*vec2.w; 
	}

	inline F getLength() const 
	{ 
		return sqrt(dot(*this)); 
	}

	inline void	normalize() 
	{
		assert(this->IsValid()); 
		F fInvLen = matkit::isqrt_safe( x*x + y*y + z*z + w*w );
		x*=fInvLen; y*=fInvLen; z*=fInvLen; w*= fInvLen;
	}

	inline void	getNormalized() 
	{
		assert(this->IsValid()); 
		F fInvLen = matkit::isqrt_safe( x*x + y*y + z*z + w*w );
		return (*this)*fInvLen;
	}

	inline void setLerp( const Vec4T<F> &p, const Vec4T<F> &q, F t ) {	*this = p*(1.0f-t) + q*t;}

public:
	F x,y,z,w;
};

typedef Vec4T<f32>		Vec4;	
typedef Vec4T<int32>	Vec4i;
typedef Vec4T<f64>		Vec4r;
