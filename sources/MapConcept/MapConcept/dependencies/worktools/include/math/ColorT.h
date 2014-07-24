#pragma once


template <class T> class ColorT;

typedef ColorT<uint8>	ColorB; // [ 0,  255]
typedef ColorT<f32>		ColorF; // [0.0, 1.0]

template <class T> 
class ColorT
{
public:
	inline ColorT() : r(0), g(0), b(0), a(0) {};
	inline ColorT(T _x, T _y, T _z, T _w);
	inline ColorT(T _x, T _y, T _z);

	inline ColorT(const unsigned int abgr );
	inline ColorT(const f32 c );
	inline ColorT(const ColorF& c);
	inline ColorT(const ColorF& c, float fAlpha);
	inline ColorT(const Vec3& c, float fAlpha);

	inline ColorT(const Vec3 & vVec)
	{
		r = (T)vVec.x;
		g = (T)vVec.y;
		b = (T)vVec.z;
		a = (T)1.f;
	}

	inline static ColorT<T> Lerp(const ColorT<T> &ca, const ColorT<T> &cb, float s)
	{
		ColorT<T> col;
		col.r = (T)(ca.r + s * (cb.r - ca.r));
		col.g = (T)(ca.g + s * (cb.g - ca.g));
		col.b = (T)(ca.b + s * (cb.b - ca.b));
		col.a = (T)(ca.a + s * (cb.a - ca.a));
		return col;
	};

	inline void set(T _x, T _y = 0, T _z = 0, T _w = 0)
	{
		r = _x; g = _y; b = _z; a = _w;
	}

//-------------------UTILS---------------------------------------------------------
	inline T &operator [] (uint32 index)		  { assert(index<=3); return ((T*)this)[index]; }
	inline T operator [] (uint32 index) const { assert(index<=3); return ((T*)this)[index]; }

	inline Vec4 toVec4() const { return Vec4(r,g,b,a); }
	inline Vec3 toVec3() const { return Vec3(r, g, b); }

	inline unsigned int pack_rgb888() const;
	inline unsigned int pack_abgr8888() const;
	inline unsigned int pack_argb8888() const;
	inline unsigned int pack_rgba8888() const;
//-------------------END UTILS-----------------------------------------------------

//-------------------OPERATORS-----------------------------------------------------
	inline bool operator == (const ColorT & v) const
	{
		return (r == v.r) && (g == v.g) && (b == v.b) && (a == v.a);
	}
	inline bool operator != (const ColorT & v) const
	{
		return !((*this)==v);
	}

	inline ColorT& operator = (const Vec3 &v) { r=(T)v.x; g=(T)v.y; b=(T)v.z; a=(T)1.0f; return *this; }
	inline ColorT& operator = (const ColorT &c) { r=(T)c.r; g=(T)c.g; b=(T)c.b; a=(T)c.a; return *this; }
	
	inline ColorT operator + () const {return *this;};
	inline ColorT operator - () const {return ColorT<T>(-r, -g, -b, -a);};
	
	//Add,Sub number
	inline ColorT<T> operator+(T k) const { return ColorT<T>(x+k,y+k,z+k,w+k); }
	inline ColorT<T>& operator+=(T k) { x+=k;y+=k;z+=k;w+=k; return *this; }
	inline ColorT<T> operator-(T k) const { return ColorT<T>(x-k,y-k,z-k,w-k); }
	inline ColorT<T>& operator-=(T k) { x-=k;y-=k;z-=k;w-=k; return *this; }

	//Mul,Div number
	inline ColorT<T> operator*(T k) const { return ColorT<T>(x*k,y*k,z*k,w*k); }
	inline ColorT<T>& operator*=(T k) { x*=k;y*=k;z*=k;w*=k; return *this; }
	inline ColorT<T> operator/(T k) const { return ColorT<T>(x/k,y/k,z/k,w/k); }
	inline ColorT<T>& operator/=(T k) { x/=k;y/=k;z/=k;w/=k; return *this; }

	//Add,Sub ColorT
	inline ColorT<T> operator+(const ColorT<T> &k) const { return ColorT<T>(x+k.x,y+k.y,z+k.z,w+k.w); }
	inline ColorT<T>& operator+=(const ColorT<T> &k) { x+=k.x;y+=k.y;z+=k.z;w+=k.w; return *this; }
	inline ColorT<T> operator-(const ColorT<T> &k) const { return ColorT<T>(x-k.x,y-k.y,z-k.z,w-k.w); }
	inline ColorT<T>& operator-=(const ColorT<T> &k) { x-=k.x;y-=k.y;z-=k.z;w-=k.w; return *this; }

	//Mul,Div ColorT
	inline ColorT<T> operator*(const ColorT<T> &k) const { return ColorT<T>(x*k.x,y*k.y,z*k.z,w*k.w); }
	inline ColorT<T>& operator*=(const ColorT<T> &k) { x*=k.x;y*=k.y;z*=k.z;w*=k.w; return *this; }
	inline ColorT<T> operator/(const ColorT<T> &k) const { return ColorT<T>(x/k.x,y/k.y,z/k.z,w/k.w); }
	inline ColorT<T>& operator/=(const ColorT<T> &k) { x/=k.x;y/=k.y;z/=k.z;w/=k.w; return *this; }

//----------------END OPERATORS----------------------------------------------------

//-------------------BASIC----------------------------------------------------------
	void clamp(float Min = 0, float Max = 1.0f)  
	{
		r = CLAMP(r, Min, Max);
		g = CLAMP(g, Min, Max);
		b = CLAMP(b, Min, Max);
		a = CLAMP(a, Min, Max);
	}

	float luminance() const	
	{
		return r*0.30f + g*0.59f + b*0.11f;
	};

	inline float getMax() const
	{
		return max(r, max(b, g));
	}

	inline float getMin() const
	{
		return min(r, min(b, g));
	}

	float normalize () 
	{
		float max = getMax();

		if (max == 0)
			return 0;

		*this /= max;

		return max;
	}

	inline void maximum(const ColorT<T> &ca, const ColorT<T> &cb)
	{
		r = max(ca.r, cb.r);
		g = max(ca.g, cb.g);
		b = max(ca.b, cb.b);
		a = max(ca.a, cb.a);
	}

	inline void minimum(const ColorT<T> &ca, const ColorT<T> &cb)
	{
		r = min(ca.r, cb.r);
		g = min(ca.g, cb.g);
		b = min(ca.b, cb.b);
		a = min(ca.a, cb.a);
	}

	inline void abs()
	{
		r = fabs(r);
		g = fabs(g);
		b = fabs(b);
		a = fabs(a);
	}

	inline void adjustContrast(T c)
	{
		r = 0.5f + c * (r - 0.5f);
		g = 0.5f + c * (g - 0.5f);
		b = 0.5f + c * (b - 0.5f);
		a = 0.5f + c * (a - 0.5f);
	}

	inline void adjustSaturation(T s)
	{
		T grey = r * 0.2125f + g * 0.7154f + b * 0.0721f;    
		r = grey + s * (r - grey);
		g = grey + s * (g - grey);
		b = grey + s * (b - grey);
		a = grey + s * (a - grey);
	}

	inline void negative()
	{
		r = T(1.0f) - r;
		g = T(1.0f) - g;
		b = T(1.0f) - b;
		a = T(1.0f) - a;
	}

	inline void grey()
	{
		T m = (r + g + b) / T(3);
		r = m;
		g = m;
		b = m;
		a = a;
	}

public:
	T	r,g,b,a;
};



//--------------Template spec-----------------

template<>
inline ColorT<f32>::ColorT(f32 _x, f32 _y, f32 _z, f32 _w)	
{
	r = _x; g = _y; b = _z; a = _w;
}

template<>
inline ColorT<f32>::ColorT(f32 _x, f32 _y, f32 _z)
{
	r = _x; g = _y; b = _z; a = 1.f;
}

template<>
inline ColorT<uint8>::ColorT(uint8 _x, uint8 _y, uint8 _z, uint8 _w)
{
	r = _x; g = _y; b = _z; a = _w;
}

template<>
inline ColorT<uint8>::ColorT(uint8 _x, uint8 _y, uint8 _z)
{
	r = _x; g = _y; b = _z; a = 255;
}

//-----------------------------------------------------------------------------

template<>
inline ColorT<f32>::ColorT(const unsigned int abgr)	
{
	r = (abgr & 0xff) / 255.0f;
	g = ((abgr>>8) & 0xff) / 255.0f;
	b = ((abgr>>16) & 0xff) / 255.0f;
	a = ((abgr>>24) & 0xff) / 255.0f;
}

template<>
inline ColorT<uint8>::ColorT(const unsigned int c)
{
	*(unsigned int*)(&r)=c;
}

//-----------------------------------------------------------------------------

template<>
inline ColorT<f32>::ColorT(const float c)	
{
	r=c;	g=c;	b=c;	a=c;
}
template<>
inline ColorT<uint8>::ColorT(const float c)	
{
	r = (uint8)(c*255);	g = (uint8)(c*255);	b = (uint8)(c*255);	a = (uint8)(c*255);
}
//-----------------------------------------------------------------------------

template<>
inline ColorT<f32>::ColorT(const ColorF& c)	
{
	r=c.r;	g=c.g;	b=c.b;	a=c.a;
}
template<>
inline ColorT<uint8>::ColorT(const ColorF& c)	{
	r = (uint8)(c.r*255);	g = (uint8)(c.g*255);	b = (uint8)(c.b*255);	a = (uint8)(c.a*255);
}

template<>
inline ColorT<f32>::ColorT(const ColorF& c, float fAlpha)
{
	r=c.r;	g=c.g;	b=c.b;	a=fAlpha;
}

template<>
inline ColorT<f32>::ColorT(const Vec3& c, float fAlpha)
{
	r=c.x;	g=c.y;	b=c.z;	a=fAlpha;
}

template<>
inline ColorT<uint8>::ColorT(const ColorF& c, float fAlpha)
{
	r = (uint8)(c.r*255);	g = (uint8)(c.g*255);	b = (uint8)(c.b*255);	a = (uint8)(fAlpha*255);
}
template<>
inline ColorT<uint8>::ColorT(const Vec3& c, float fAlpha)	
{
	r = (uint8)(c.x*255);	g = (uint8)(c.y*255);	b = (uint8)(c.z*255);	a = (uint8)(fAlpha*255);
}

//-----------------------------------------------------------------------------

//------------------------Pack color-------------------------------------------
///////////////////////////////////////////////
template <class T>
inline unsigned int ColorT<T>::pack_rgb888() const
{
	unsigned char cr;
	unsigned char cg;
	unsigned char cb;
	if(sizeof(r) == 1) // char and unsigned char
	{
		cr = (unsigned char)r;
		cg = (unsigned char)g;
		cb = (unsigned char)b;
	}
	else if(sizeof(r) == 2) // short and unsigned short
	{
		cr = (unsigned short)(r)>>8;
		cg = (unsigned short)(g)>>8;
		cb = (unsigned short)(b)>>8;
	}
	else // float or double
	{
		cr = (unsigned char)(r * 255.0f);
		cg = (unsigned char)(g * 255.0f);
		cb = (unsigned char)(b * 255.0f);
	}
	return (cr << 16) | (cg << 8) | cb;
}

template <class T>
inline unsigned int ColorT<T>::pack_rgba8888() const
{
	unsigned char cr;
	unsigned char cg;
	unsigned char cb;
	unsigned char ca;
	if(sizeof(r) == 1) // char and unsigned char
	{
		cr = (unsigned char)r;
		cg = (unsigned char)g;
		cb = (unsigned char)b;
		ca = (unsigned char)a;
	}
	else if(sizeof(r) == 2) // short and unsigned short
	{
		cr = (unsigned short)(r)>>8;
		cg = (unsigned short)(g)>>8;
		cb = (unsigned short)(b)>>8;
		ca = (unsigned short)(a)>>8;
	}
	else // float or double
	{
		cr = (unsigned char)(r * 255.0f);
		cg = (unsigned char)(g * 255.0f);
		cb = (unsigned char)(b * 255.0f);
		ca = (unsigned char)(a * 255.0f);
	}
	//return (ca << 24) | (cr << 16) | (cg << 8) | cb;
	return (cr << 24) | (cg << 16) | (cb << 8) | ca;
}

///////////////////////////////////////////////
template <class T>
inline unsigned int ColorT<T>::pack_abgr8888() const
{
	unsigned char cr;
	unsigned char cg;
	unsigned char cb;
	unsigned char ca;
	if(sizeof(r) == 1) // char and unsigned char
	{
		cr = (unsigned char)r;
		cg = (unsigned char)g;
		cb = (unsigned char)b;
		ca = (unsigned char)a;
	}
	else if(sizeof(r) == 2) // short and unsigned short
	{
		cr = (unsigned short)(r)>>8;
		cg = (unsigned short)(g)>>8;
		cb = (unsigned short)(b)>>8;
		ca = (unsigned short)(a)>>8;
	}
	else // float or double
	{
		cr = (unsigned char)(r * 255.0f);
		cg = (unsigned char)(g * 255.0f);
		cb = (unsigned char)(b * 255.0f);
		ca = (unsigned char)(a * 255.0f);
	}
	return (ca << 24) | (cb << 16) | (cg << 8) | cr;
}


///////////////////////////////////////////////
template <class T>
inline unsigned int ColorT<T>::pack_argb8888() const
{
	unsigned char cr;
	unsigned char cg;
	unsigned char cb;
	unsigned char ca;
	if(sizeof(r) == 1) // char and unsigned char
	{
		cr = (unsigned char)r;
		cg = (unsigned char)g;
		cb = (unsigned char)b;
		ca = (unsigned char)a;
	}
	else if(sizeof(r) == 2) // short and unsigned short
	{
		cr = (unsigned short)(r)>>8;
		cg = (unsigned short)(g)>>8;
		cb = (unsigned short)(b)>>8;
		ca = (unsigned short)(a)>>8;
	}
	else // float or double
	{
		cr = (unsigned char)(r * 255.0f);
		cg = (unsigned char)(g * 255.0f);
		cb = (unsigned char)(b * 255.0f);
		ca = (unsigned char)(a * 255.0f);
	}
	return (ca << 24) | (cr << 16) | (cg << 8) | cb;
}

