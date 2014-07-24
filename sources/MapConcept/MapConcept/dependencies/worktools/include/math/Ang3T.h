#pragma once


template <typename F> 
class Ang3T
{
public:
	inline Ang3T() : x(0), y(0), z(0) {};
	inline Ang3T( F vx, F vy, F vz )	: x(vx), y(vy), z(vz) {assert(this->isValid()); };
	
	
	template<class F1> 
	inline Ang3T(const Vec3T<F1>& v) : x((F)v.x), y((F)v.y), z((F)v.z) { assert(this->isValid()); }
	template<class F1> 
	inline Ang3T( const Ang3T<F1>& v ) : x((F)v.x), y((F)v.y), z((F)v.z) { assert(this->isValid()); }

	template<class F1> 
	inline Ang3T( const QuatT<F1>& q )
	{
		assert(q.isValid());
		y = F( asin(max((F)-1.0,min((F)1.0,-(q.v.x*q.v.z-q.w*q.v.y)*2))) );
		if (fabs(fabs(y)-(F)((F)FL_PI*(F)0.5))<(F)0.01)	
		{
			x = F(0);
			z = F(atan2(-2*(q.v.x*q.v.y-q.w*q.v.z),1-(q.v.x*q.v.x+q.v.z*q.v.z)*2));
		} 
		else 
		{
			x = F(atan2((q.v.y*q.v.z+q.w*q.v.x)*2, 1-(q.v.x*q.v.x+q.v.y*q.v.y)*2));
			z = F(atan2((q.v.x*q.v.y+q.w*q.v.z)*2, 1-(q.v.z*q.v.z+q.v.y*q.v.y)*2));
		}
	};

	template<class F1> 
	inline Ang3T( const Matrix44T<F1>& m )
	{
		assert( m.isOrthonormalRH(0.001f) );
		y = F( asin(max((F)-1.0,min((F)1.0,-m.m20))) );
		if (fabs(fabs(y)-(F)((F)g_PI*(F)0.5))<(F)0.01)	
		{
			x = F(0);
			z = F(atan2(-m.m01,m.m11));
		} 
		else 
		{
			x = F(atan2(m.m21, m.m22));
			z = F(atan2(m.m10, m.m00));
		}
	}

	template<typename F1>	
	static inline F CreateRadZ( const Vec2T<F1>& v0, const Vec2T<F1>& v1 )
	{
		F cz	= v0.x*v1.y-v0.y*v1.x; 
		F c		=	v0.x*v1.x+v0.y*v1.y;
		return F( atan2(cz,c) );
	}

	template<typename F1>	
	static inline F CreateRadZ( const Vec3T<F1>& v0, const Vec3T<F1>& v1 )
	{
		F cz	= v0.x*v1.y-v0.y*v1.x; 
		F c		=	v0.x*v1.x+v0.y*v1.y;
		return F( atan2(cz,c) );
	}

	inline void set(F xval,F yval,F zval) { x=xval; y=yval; z=zval; assert(this->isValid());}

//-------------------UTILS---------------------------------------------------------
	inline F &operator [] (uint32 index)		  { assert(index<=2); return ((F*)this)[index]; }
	inline F operator [] (uint32 index) const { assert(index<=2); return ((F*)this)[index]; }

	inline bool equals( const Ang3T<F>& v1, F epsilon=VEC_EPSILON) const {
		return  ((fabs(x-v1.x) <= epsilon)  && (fabs(y-v1.y) <= epsilon) && (fabs(z-v1.z) <= epsilon));	
	}

	inline Vec3T<F> toVec3()
	{
		return Vec3T<F>(x,y,z);
	};

	bool isValid() const
	{
		if (!NumberValid(x)) return false;
		if (!NumberValid(y)) return false;
		if (!NumberValid(z)) return false;
		return true;
	}
//-------------------END UTILS-----------------------------------------------------

//-------------------OPERATORS-----------------------------------------------------
	inline bool operator==(const Ang3T<F> &vec) { return x == vec.x && y == vec.y && z == vec.z; }
	inline bool operator!=(const Ang3T<F> &vec) { return !(*this == vec); }
	
	inline Ang3T<F> operator * (F k) const { return Ang3T<F>(x*k,y*k,z*k); }
	inline Ang3T<F>& operator *= (F k) { x*=k;y*=k;z*=k; return *this; }
	inline Ang3T<F> operator / (F k) const { k=(F)1.0/k; return Ang3T<F>(x*k,y*k,z*k); }
	inline Ang3T<F> operator - ( void ) const { return Ang3T<F>(-x,-y,-z); }

	inline Ang3T<F> operator + (const Ang3T<F> &v1) {
		return Ang3T<F>(x+v1.x, y+v1.y, z+v1.z);
	}
	inline Ang3T<F> operator - (const Ang3T<F> &v1) {
		return Ang3T<F>(x-v1.x, y-v1.y, z-v1.z);
	}

	inline Ang3T<F>& operator += (const Ang3T<F> &v1) {
		x+=v1.x; y+=v1.y; z+=v1.z; return *this;
	}
	inline Ang3T<F>& operator -= (const Ang3T<F> &v1) {
		x-=v1.x; y-=v1.y; z-=v1.z; return *this;
	}
//----------------END OPERATORS----------------------------------------------------

//-------------------BASIC----------------------------------------------------------

	template<typename F1>	
	inline static Ang3T<F> GetAnglesXYZ( const QuatT<F1>& q ) {	return Ang3T<F>(q); }
	template<typename F1>	
	inline void setAnglesXYZ( const QuatT<F1>& q )	{	*this=Ang3T<F>(q);	}

	template<typename F1>	
	inline static Ang3T<F> GetAnglesXYZ( const Matrix44T<F1>& m ) {	return Ang3T<F>(m); }
	template<typename F1>	
	inline void setAnglesXYZ( const Matrix44T<F1>& m )	{	*this=Ang3T<F>(m);	}


	inline bool isInRangePI() const {
		F pi=(F)(FL_PI+0.001); 
		return  (  (x>-pi)&&(x<pi) && (y>-pi)&&(y<pi) && (z>-pi)&&(z<pi) );	
	}

	void rangePI() {
		if (x< (F)FL_PI) x+=(F)FL_PI2;
		if (x> (F)FL_PI) x-=(F)FL_PI2;
		if (y< (F)FL_PI) y+=(F)FL_PI2;
		if (y> (F)FL_PI) y-=(F)FL_PI2;
		if (z< (F)FL_PI) z+=(F)FL_PI2;
		if (z> (F)FL_PI) z-=(F)FL_PI2;
	}
public:
	F x,y,z;
};

typedef Ang3T<f32>		Ang3;
typedef Ang3T<int32>		Ang3i;
typedef Ang3T<f64>		Ang3r;
