#pragma once

namespace matkit
{
	
//-----------------template functions-----------------------
	template<class T> 
	inline void swap_tpl(T& a, T& b)
	{
		T c=a;
		a=b;
		b=c;
	};
	
	template<class T> 
	inline bool equals_tpl(T a, T b, T tolerance)
	{
		return (a + tolerance >= b) && (a - tolerance <= b);
	};
	
	template<class T> 
	inline T lerp_tpl(T x1,T x2,T c)
	{
		return x1+c*(x2-x1);
	};
	
	template<class T> 
	inline void limit_tpl(T& val, const T& min, const T& max)
	{
		if (val < min) val = min;
		else if (val > max)	val = max;
	}
	
	template<class T> 
	inline T clamp_tpl( T X, T Min, T Max ) 
	{	
		return X<Min ? Min : X<Max ? X : Max; 
	}

	template<class T> 
	inline T min_tpl( const T& a, const T& b )
	{
		return b < a ? b : a;
	}

	template<class T> 
	inline T max_tpl( const T& a, const T& b )
	{
		return a < b ? b : a;
	}

	template<class T> 
	inline T in_range( const T& x, const T& min, const T& max )
	{
		return (x>=min_tpl(min,max)) && (x<=max_tpl(min,max));
	}
	
	template<class T> 
	inline T arrayMin(T *f, int size)
	{
		assert(f && size);
		T m=f[0];
		for(int i=1;i<size;i++)
		{
			m=min_tpl(m,f[i]);
		};
		return m;
	}

	template<class T> 
	inline T arrayMax(T *f, int size)
	{
		assert(f && size);
		T m=f[0];
		for(int i=1;i<size;i++)
		{
			m=max_tpl(m,f[i]);
		};
		return m;
	}

	inline bool isPower2(unsigned int val)
	{
		return val && !(val & (val - 1));
	};

	inline unsigned int nextPowerOf2(unsigned int n)
    {
		if(n==0) return 0;
        n--;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n++;
        return n;
    }

	inline unsigned int prevPowerOf2(unsigned int n)
	{
		if(n==0) return 0;
		n |= (n >> 1);
		n |= (n >> 2);
		n |= (n >> 4);
		n |= (n >> 8);
		n |= (n >> 16);
		return n - (n >> 1);
	};

	inline unsigned int nearPowerOf2(double n)
	{
		return pow(2, ceil(log(n)/log(2.0)));
	};

	template<class T> 
	inline T sign(T val)
	{
		return (val>0) ? (T)1 : (T)-1;
	};

	template<class T> 
	inline T signz(T val)
	{
		return (val>0) ? (T)1 : (T)0;
	};
	
	template<class T> 
	inline bool isneg(T val)
	{
		return val<0;
	};

	template<class T> 
	inline bool ispos(T val)
	{
		return val>0;
	};

	template<class T> 
	inline bool iszero(T val)
	{
		return val==0;
	};

//-----------------Random---------------------------
	template<class T> 
	inline T random(T min, T max)
	{
		return (double(rand()) / (RAND_MAX + 1) * (max - min)
				+ min);
	}
	
	inline f32 random32f(f32 min, f32 max)
	{
		return (f32(rand()) / (RAND_MAX + 1.0f) * (max - min)
				+ min);
	}

	inline f32 random32f()
	{
		return (f32(rand()) / (RAND_MAX + 1.0f) * (1.f - 0.f));
	}

	inline f64 random64f(f64 min, f64 max)
	{
		return (f64(rand()) / (RAND_MAX + 1.0) * (max - min)
				+ min);
	}

	inline f64 random64f()
	{
		return (f64(rand()) / (RAND_MAX + 1.0) * (1.0 - 0.0));
	}

//-----------------Gauss-----------------------------------------

	inline f64 gauss(f64 x, f64 m=0, f64 d=1.0)
	{
		const static f64 root2pi = 2.506628274631000502415765284811;
		return (1.0/(d*root2pi))*exp(-(((x-m)*(x-m))/(2.0*d*d)));
	};
	
	inline f64 randomGauss()
	{
		return (gauss(random64f(-3,3),0,1)/4.0)+0.5;
	}

	inline f64 randomGauss(f64 min, f64 max)
	{
		return min+randomGauss()*(max-min);
	}

//-----------------Basic----------------------------------------

	inline f32 clamp( f32 X, f32 Min, f32 Max ) 
	{	
		X = (X+Max-fabsf(X-Max))*0.5f;  //return the min
		X = (X+Min+fabsf(X-Min))*0.5f;  //return the max 
		return X;
	}

	inline f64 clamp( f64 X, f64 Min, f64 Max ) 
	{	
		X = (X+Max-fabs(X-Max))*0.5;  //return the min
		X = (X+Min+fabs(X-Min))*0.5;  //return the max 
		return X;
	}

	template<class T> 
	inline bool isNaN(const T& x)
	{
		if(x!=x)
			return true;
		return false;
	};

	inline bool isFinite(const float& a)
	{
		return (0 == ((_FPCLASS_SNAN | _FPCLASS_QNAN | _FPCLASS_NINF | _FPCLASS_PINF) & _fpclass(a) ));
	}

	inline bool isFinite(const double& a)
	{
		return (0 == ((_FPCLASS_SNAN | _FPCLASS_QNAN | _FPCLASS_NINF | _FPCLASS_PINF) & _fpclass(a) ));
	}

	inline f32 fsel(const f32 _a, const f32 _b, const f32 _c) { return (_a < 0.0f) ? _c : _b;}

	inline f64 logBase(f64 a, f64 base)
	{
	   return log(a) / log(base);
	}

	inline f64 divInt(f64 Val, f64 Div)
	{
		return f64(int(Val/Div));
	}

	inline f64 divTail(f64 Val, f64 Div)
	{
		return Val-Div*divInt(Val,Div);
	}
	
	inline void sincosf (f32 angle, f32& Sin, f32& Cos) {	Sin = f32(sin(angle));	Cos = f32(cos(angle));	}
	inline void sincos  (f64 angle, f64& Sin, f64& Cos) {	Sin = f64(sin(angle));  Cos = f64(cos(angle));	}
	
	inline f32 isqrt(f32 x) {return 1.f/sqrtf(x);}
	inline f64 isqrt(f64 x) {return 1.0/sqrt(x);}
	
	inline f32 isqrt_safe(f32 x) {return 1.f/sqrtf(x+FLT_MIN);}
	inline f64 isqrt_safe(f64 x) {return 1.0/sqrt(x+DBL_MIN);}

	inline int32 incm3(int32 i) { return i+1 & (i-2)>>31; }
	inline int32 decm3(int32 i) { return i-1 + ((i-1)>>31&3); }
	
	inline f32 reciprocal( const f32 f )
	{
		return 1.f/f;
	};

//-----------------Angles---------------------------------------

	inline float angleDifference(float a, float b)
	{
		float diff = fmodf(fabsf(a - b), FL_PI);
		return (float)fsel(diff - FL_PI, FL_PI - diff, diff);
	}

	inline f32 angle_snap360( f32 val ) {
		if( val < 0.0f )
			val =f32( 360.0f + fmodf(val,360.0f));
		else
			if(val >= 360.0f)
				val =f32(fmodf(val,360.0f));
		return val;
	}

	//! normalize the val to -180, 180 range 
	inline f32 angle_snap180( f32 val ) {
		if( val > -180.0f && val < 180.0f)
			return val;
		val = angle_snap360( val );
		if( val>180.0f )
			return -(360.0f - val);
		return val;
	}

};
