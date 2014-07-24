#pragma once


template<class F> 
class  Vec2T
{
public:
	inline Vec2T(void) : x(0), y(0) {};
	inline Vec2T(F vx,F vy)  : x(vx), y(vy) {assert(this->isValid());}

	inline static Vec2T<F> CreateLerp( const Vec2T<F> &p, const Vec2T<F> &q, F t ) {	return p*(1.0f-t) + q*t;}

	inline Vec2T<F>& set(F nx,F ny) 
	{ 
		x=F(nx); y=F(ny); 
		assert(this->isValid()); 
		return *this; 
	};

//-------------------UTILS--------------------------------------------------

	inline F &operator [] (uint32 index)			{ assert(index<=1); return ((F*)this)[index]; }
	inline F operator [] (uint32 index) const		{ assert(index<=1); return ((F*)this)[index]; }

	inline bool isZero(F e = (F)0.0) const
	{
		return  (fabs(x) <= e) && (fabs(y) <= e);
	}

	inline Vec2T<F>& flip() { x=-x;y=-y; return *this; }
	inline Vec2T<F>& zero() { x=y=0; return *this; }
	inline Vec2T<F> rot90ccw() { return Vec2T<F>(-y,x); }
	inline Vec2T<F> rot90cw() { return Vec2T<F>(y,-x); }

	bool isValid() const
	{
		if (!NumberValid(x)) return false;
		if (!NumberValid(y)) return false;
		return true;
	}

	inline bool equals(const Vec2T<F> & v1, F epsilon=VEC_EPSILON) const 
	{
		assert(v1.isValid()); 
		assert(this->isValid()); 
		return  ((fabs(x-v1.x) <= epsilon) && (fabs(y-v1.y) <= epsilon));	
	}

//-------------------END UTILS-----------------------------------------------------

//-------------------OPERATORS-----------------------------------------------------

	inline Vec2T<F> &operator=(const Vec2T<F> &src) { x=src.x; y=src.y; return *this; }
	inline bool operator==(const Vec2T<F> &vec) {return (this->x==vec.x) && (this->y==vec.y);};
	inline bool operator!=(const Vec2T<F> &vec) { return !(*this == vec); }

	//Add,Sub number
	inline Vec2T<F> operator-() const { return Vec2T<F>(-x,-y); }
	inline Vec2T<F> operator+(F k) const { return Vec2T<F>(x+k,y+k); }
	inline Vec2T<F>& operator+=(F k) { x+=k;y+=k; return *this; }
	inline Vec2T<F> operator-(F k) const { return Vec2T<F>(x-k,y-k); }
	inline Vec2T<F>& operator-=(F k) { x-=k;y-=k; return *this; }

	//Mul,Div number
	inline Vec2T<F> operator*(F k) const { return Vec2T<F>(x*k,y*k); }
	inline Vec2T<F>& operator*=(F k) { x*=k;y*=k; return *this; }
	inline Vec2T<F> operator/(F k) const { return Vec2T<F>(x/k,y/k); }
	inline Vec2T<F>& operator/=(F k) { x/=k;y/=k; return *this; }

	//Add,Sub vec2
	inline Vec2T<F> operator+(Vec2T<F> &k) const { return Vec2T<F>(x+k.x,y+k.y); }
	inline Vec2T<F>& operator+=(Vec2T<F> &k) { x+=k.x;y+=k.y; return *this; }
	inline Vec2T<F> operator-(Vec2T<F> &k) const { return Vec2T<F>(x-k.x,y-k.y); }
	inline Vec2T<F>& operator-=(Vec2T<F> &k) { x-=k.x;y-=k.y; return *this; }

	//Mul,Div vec2
	inline Vec2T<F> operator*(Vec2T<F> &k) const { return Vec2T<F>(x*k.x,y*k.y); }
	inline Vec2T<F>& operator*=(Vec2T<F> &k) { x*=k.x;y*=k.y; return *this; }
	inline Vec2T<F> operator/(Vec2T<F> &k) const { return Vec2T<F>(x/k.x,y/k.y); }
	inline Vec2T<F>& operator/=(Vec2T<F> &k) { x/=k.x;y/=k.y; return *this; }

	//Dot,Cross
	inline F operator|(Vec2T<F> &k) const { return this->dot(k); }
	inline F& operator|=(Vec2T<F> &k) { *this=this->dot(k); return *this; }
	inline Vec2T<F> operator^(Vec2T<F> &k) const { return this->cross(k); }
	inline Vec2T<F>& operator^=(Vec2T<F> &k) { *this=this->cross(k); return *this; }

//----------------END OPERATORS----------------------------------------------------

//-------------------BASIC----------------------------------------------------------
	Vec2T<F>& normalize() 
	{ 
		F fInvLen = matkit::isqrt_safe(x*x+y*y);
		x *= fInvLen; y *= fInvLen; 
		return *this; 
	}

	Vec2T<F> getNormalized()
	{ 
		F fInvLen = matkit::isqrt_safe(x*x+y*y);
		return *this * fInvLen;
	}

	inline F getLength() const { return sqrt(x*x+y*y); }
	inline F getLength2() const { return x*x+y*y; }

	void setLength(F fLen)
	{ 
		F fLenMe = getLength2();
		if(fLenMe<0.00001f*0.00001f)
			return;
		fLenMe = fLen * matkit::isqrt(fLenMe);
		x*=fLenMe; y*=fLenMe;
	}

	inline F getDistance(const Vec2T<F> vec1) const
	{
		return  sqrtf((x-vec1.x)*(x-vec1.x)+(y-vec1.y)*(y-vec1.y)); 
	}

	inline Vec2T<F> scale(F Scale)
	{
		return Vec2T<F>(x*Scale,y*Scale);
	};

	inline void rotate(F angle)
    {
        set(x*cos(angle)-y*sin(angle),x*sin(angle)+y*cos(angle));
    };

    inline Vec2T<F> getRotated(F angle)
    {
        Vec2T<F> v(*this);
        v.rotate(angle);
        return v;
    };

    inline F getAngle()
    {
		return (F)atan2((float)y, (float)x);
    };

	inline F dot(const Vec2T<F> &rhs) const {return x*rhs.x + y*rhs.y;}
	inline F cross (const Vec2T<F> &v) const {return x*v.y - y*v.x;}
//-----------------END BASIC--------------------------------------------------------

public:
	F x;
	F y;
};


typedef Vec2T<f32> Vec2;
typedef Vec2T<int32> Vec2i;
typedef Vec2T<f64> Vec2r;