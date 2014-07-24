#pragma once

//----------------------------------------------------------------------
// Quaternion
//----------------------------------------------------------------------

template <typename F> class Quat_tpl
{
public:
	Vec3_tpl<F> v;	F w;

	inline Quat_tpl() {w=0;}

	inline Quat_tpl( F W, F X, F Y, F Z, bool bNorm = false ) 
		: w(W),v(X,Y,Z)	
	{
		if (bNorm) Normalize();
		assert(IsValid()); 
	}
	inline Quat_tpl( F angle, const Vec3_tpl<F> &axis) : w(angle),v(axis) {};


	/*explicit inline Quat_tpl( const Ang3_tpl<F>& rad ) {
		assert(rad.IsValid());
		SetRotationXYZ(rad);
	}*/

	explicit Quat_tpl(const Matrix44_tpl<F>& m);//!

	template <class T> inline  Quat_tpl( const Quat_tpl<T>& q )  { w=F(q.w); v.x=F(q.v.x); v.y=F(q.v.y); v.z=F(q.v.z);  assert(this->IsValid()); }
	inline Quat_tpl &operator=(const Quat_tpl &src) { v.x=src.v.x; v.y=src.v.y; v.z=src.v.z; w=src.w; return *this; }

	///////////////
	inline Quat_tpl<F>	operator - () const { return Quat_tpl<F>( -w,-v ); };
	inline Quat_tpl<F> operator ! () const { return Quat_tpl(w,-v); }

	void operator *= (F op) {	w*=op; v*=op;	}

	inline F operator | (const Quat_tpl<F>& p) 
	{
		assert(p.v.IsValid());
		return (v.x*p.v.x + v.y*p.v.y + v.z*p.v.z + w*p.w); 
	} 

	Quat_tpl<F> inline operator * (const Quat_tpl<F> &p) { 
		assert(p.IsValid());
		return Quat_tpl<F1>
			( 
			w*p.w  - (v.x*p.v.x + v.y*p.v.y + v.z*p.v.z),
			v.y*p.v.z-v.z*p.v.y + w*p.v.x + v.x*p.w,
			v.z*p.v.x-v.x*p.v.z + w*p.v.y + v.y*p.w,
			v.x*p.v.y-v.y*p.v.x + w*p.v.z + v.z*p.w  
			);
	}

	inline void operator *= (const Quat_tpl<F> &p) 
	{ 
		assert(p.IsValid());
		*this=w; w=w*p.w-(v|p.v); v=v*s0+v*p.w+(v%p.v); 
	}

	inline Quat_tpl<F> operator / (const Quat_tpl<F> &p) { 
		return (!p*(*this)); 
	}

	inline void operator /= (const Quat_tpl<F> &p) { 
		*this=(!p*q);
	}

	inline Quat_tpl<F> operator + (const Quat_tpl<F> &p) { 
		return Quat_tpl<F1>( w+p.w, v+p.v ); 
	}
	inline void operator += (const Quat_tpl<F> &p) { 
		w+=p.w; v+=p.v; 
	}

	inline Quat_tpl<F> operator - (const Quat_tpl<F> &p) { 
		return Quat_tpl<F1>( w-p.w, v-p.v); 
	}
	inline void operator -= (const Quat_tpl<F> &p) { 
		w-=p.w; v-=p.v; 
	}
	///
	// Exact compare of 2 quats.
	inline bool operator==(const Quat_tpl<F> &q) const { return IsEquivalent(q,0.0000001f); }
	inline bool operator!=(const Quat_tpl<F> &q) const { return !(*this == q); }

	
	

	// Description:
	//    Check if identity quaternion.
	inline bool IsIdentity() const { return w == 1 && v.x == 0 && v.y == 0 && v.z == 0; }

	inline void	SetIdentity(void) { w=1; v.x=0; v.y=0; v.z=0; }
	static Quat_tpl<F> CreateIdentity(void)	{ return Quat_tpl(1,0,0,0); }

	inline bool IsUnit(F e = VEC_EPSILON) const
	{
		return fabs_tpl( 1 - ((*this) | (*this)) ) < e;
	}

	inline bool IsValid(F e = VEC_EPSILON) const
	{
		if (!v.IsValid())	return false;
		if (!NumberValid(w)) return false;
		//if (!IsUnit(e))	return false;
		return true;
	}


	inline void SetRotationAA(F rad, const Vec3_tpl<F> &axis);
	static Quat_tpl<F> CreateRotationAA(F rad, const Vec3_tpl<F> &axis);

	inline void SetRotationAA(F cosha, F sinha, const Vec3_tpl<F> &axis);
	static Quat_tpl<F> CreateRotationAA(F cosha, F sinha, const Vec3_tpl<F> &axis);

	inline void SetRotationXYZ(const Ang3 &a);
	static Quat_tpl<F> CreateRotationXYZ(const Ang3 &a);

	inline void SetRotationX( f32 r );
	static Quat_tpl<F> CreateRotationX( f32 r );
	inline void SetRotationY( f32 r );
	static Quat_tpl<F> CreateRotationY( f32 r );
	inline void SetRotationZ( f32 r );
	static Quat_tpl<F> CreateRotationZ( f32 r );

	inline void SetRotationV0V1(const Vec3_tpl<F>& v0, const Vec3_tpl<F>& v1);
	static Quat_tpl<F> CreateRotationV0V1(const Vec3_tpl<F>& v0, const Vec3_tpl<F>& v1);

	inline void SetRotationVDir( const Vec3_tpl<F>& vdir );
	static Quat_tpl<F> CreateRotationVDir( const Vec3_tpl<F>& vdir); 
	inline void SetRotationVDir( const Vec3_tpl<F>& vdir, F roll );
	static Quat_tpl<F> CreateRotationVDir( const Vec3_tpl<F>& vdir, F roll ); 

	//A quaternion is a compressed matrix. Thus there is no problem extracting the rows & columns. 
	inline Vec3_tpl<F> GetColumn(uint32 i) 
	{
		if (i==0)	return GetColumn0();
		if (i==1)	return GetColumn1();
		if (i==2)	return GetColumn2();
		assert(0); //bad index
		return Vec3(0);
	}

	inline Vec3_tpl<F> GetColumn0() const {return Vec3_tpl<F>(2*(v.x*v.x+w*w)-1,2*(v.y*v.x+v.z*w),2*(v.z*v.x-v.y*w));}
	inline Vec3_tpl<F> GetColumn1() const {return Vec3_tpl<F>(2*(v.x*v.y-v.z*w),2*(v.y*v.y+w*w)-1,2*(v.z*v.y+v.x*w));}
	inline Vec3_tpl<F> GetColumn2() const {return Vec3_tpl<F>(2*(v.x*v.z+v.y*w),2*(v.y*v.z-v.x*w),2*(v.z*v.z+w*w)-1);}
	inline Vec3_tpl<F> GetRow0() const {return Vec3_tpl<F>(2*(v.x*v.x+w*w)-1,2*(v.x*v.y-v.z*w),2*(v.x*v.z+v.y*w));}
	inline Vec3_tpl<F> GetRow1() const {return Vec3_tpl<F>(2*(v.y*v.x+v.z*w),2*(v.y*v.y+w*w)-1,2*(v.y*v.z-v.x*w));}
	inline Vec3_tpl<F> GetRow2() const	{return Vec3_tpl<F>(2*(v.z*v.x-v.y*w),2*(v.z*v.y+v.x*w),2*(v.z*v.z+w*w)-1);}

	// These are just copy & pasted components of the GetColumn1() above.
	inline F GetFwdX() const { return (2*(v.x*v.y-v.z*w)); }
	inline F GetFwdY() const { return (2*(v.y*v.y+w*w)-1); }
	inline F GetFwdZ() const { return (2*(v.z*v.y+v.x*w)); }
	inline F GetRotZ() const { return atan2_tpl(-GetFwdX(), GetFwdY()); }

	inline void Invert( void ) { *this=!*this;	}
	inline Quat_tpl<F> GetInverted() const { return !(*this); } 

	inline void	Normalize(void);
	inline Quat_tpl<F> GetNormalized() const;

	inline void	NormalizeSafe(void);
	inline Quat_tpl<F> GetNormalizedSafe() const;

	inline void	NormalizeFast(void)
	{
			assert(this->IsValid()); 
			F fInvLen = isqrt_fast_tpl( v.x*v.x + v.y*v.y + v.z*v.z + w*w );
			v.x*=fInvLen; v.y*=fInvLen; v.z*=fInvLen; w *= fInvLen;
	}

	inline Quat_tpl<F> GetNormalizedFast() const;

	inline F GetLength() {return sqrt_tpl(w*w + v.x*v.x+v.y*v.y+v.z*v.z); };

	inline bool IsEquivalent( const Quat_tpl<F>& q, F e=VEC_EPSILON) const {
		Quat_tpl<F> p=-q;
		bool t0= (fabs_tpl(v.x-q.v.x)<=e) && (fabs_tpl(v.y-q.v.y)<=e) && (fabs_tpl(v.z-q.v.z)<=e) && (fabs_tpl(w-q.w)<=e);	
		bool t1= (fabs_tpl(v.x-p.v.x)<=e) && (fabs_tpl(v.y-p.v.y)<=e) && (fabs_tpl(v.z-p.v.z)<=e) && (fabs_tpl(w-p.w)<=e);	
		t0 |= t1;
		return t0;

	}

	inline void SetNlerp( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t  ); 
	static Quat_tpl<F> CreateNlerp( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ); 

	inline void SetNlerp2( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t  ); 
	static Quat_tpl<F> CreateNlerp2( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ); 

	inline void SetSlerp( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ); 
	static Quat_tpl<F> CreateSlerp( const Quat_tpl<F>& p, const Quat_tpl<F>& tq, F t );

	inline void SetExpSlerp( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ); 
	static Quat_tpl<F> CreateExpSlerp( const Quat_tpl<F>& p, const Quat_tpl<F>& tq, F t );

	static Quat_tpl<F> CreateSquad( const Quat_tpl<F>& p,const Quat_tpl<F>& a,const Quat_tpl<F>& b,const Quat_tpl<F>& q, F t );
	static Quat_tpl<F> CreateSquadRev( F angle, const Vec3& axis, const Quat_tpl<F>& p, const Quat_tpl<F>& a, const Quat_tpl<F>& b, const Quat_tpl<F>& q, F t );

	inline Quat_tpl<F> GetScaled(F scale) const
	{
		return CreateNlerp(IDENTITY, *this, scale);
	}

	inline void ClampAngle(F maxAngleDeg)
	{
		F maxAngleRad = DEG2RAD(maxAngleDeg);
		F wMax = cry_cosf((F)(2.0) * maxAngleRad);
		if (w < wMax)
		{
			w = wMax;
			Normalize();
		}
	}

};


///////////////////////////////////////////////////////////////////////////////
// Typedefs                                                                  // 
///////////////////////////////////////////////////////////////////////////////

typedef Quat_tpl<f32>	Quat;


template<class F,class F2> inline Vec3_tpl<F> operator * (const Quat_tpl<F> &q, const Vec3_tpl<F2> &v) {
	assert(v.IsValid());
	assert(q.IsValid());
	//muls=15 / adds=15
	Vec3_tpl<F> out,r2;
	r2.x=(q.v.y*v.z-q.v.z*v.y)+q.w*v.x;
	r2.y=(q.v.z*v.x-q.v.x*v.z)+q.w*v.y;
	r2.z=(q.v.x*v.y-q.v.y*v.x)+q.w*v.z;
	out.x=(r2.z*q.v.y-r2.y*q.v.z); out.x+=out.x+v.x;
	out.y=(r2.x*q.v.z-r2.z*q.v.x); out.y+=out.y+v.y;
	out.z=(r2.y*q.v.x-r2.x*q.v.y); out.z+=out.z+v.z;
	return out;
}


/*!
* pre-multiply of a quaternion and a Vec3 (3D rotations with quaternions)
* 
* Example:
*  Quat q(1,0,0,0);
*  Vec3 v(33,44,55);
*	 Vec3 result = v*q;
*/
template<class F,class F2> inline Vec3_tpl<F2> operator * (const Vec3_tpl<F> &v, const Quat_tpl<F2> &q) {
	assert(v.IsValid());
	assert(q.IsValid());
	//muls=15 / adds=15
	Vec3_tpl<F> out,r2;
	r2.x=(q.v.z*v.y-q.v.y*v.z)+q.w*v.x;
	r2.y=(q.v.x*v.z-q.v.z*v.x)+q.w*v.y;
	r2.z=(q.v.y*v.x-q.v.x*v.y)+q.w*v.z;
	out.x=(r2.y*q.v.z-r2.z*q.v.y); out.x+=out.x+v.x;
	out.y=(r2.z*q.v.x-r2.x*q.v.z); out.y+=out.y+v.y;
	out.z=(r2.x*q.v.y-r2.y*q.v.x); out.z+=out.z+v.z;
	return out;
}


template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateRotationAA(F rad, const Vec3_tpl<F> &axis) { 	
	Quat_tpl<F> q;	q.SetRotationAA(rad,axis); 	return q;	
}
template<typename F> inline void Quat_tpl<F>::SetRotationAA(F rad, const Vec3_tpl<F> &axis) { 
	F s,c; sincos_tpl( rad*(F)0.5, &s,&c);	SetRotationAA(c,s, axis); 
}

template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateRotationAA(F cosha, F sinha, const Vec3_tpl<F> &axis) { 
	Quat_tpl<F> q;	q.SetRotationAA(cosha,sinha,axis); 	return q;	
}
template<typename F> inline void Quat_tpl<F>::SetRotationAA(F cosha, F sinha, const Vec3_tpl<F> &axis)	{ 
	assert(axis.IsUnit(0.001f));
	w=cosha; v=axis*sinha; 
}

/*!
* Create rotation-quaternion that around the fixed coordinate axes.
*
* Example:
*		Quat q=Quat::CreateRotationXYZ( Ang3(1,2,3) );
*   or
*		q.SetRotationXYZ( Ang3(1,2,3) );
*/
template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateRotationXYZ(const Ang3 &a) 
{	
	assert(a.IsValid());
	Quat_tpl<F> q;	q.SetRotationXYZ(a); 	return q;	
}
template<typename F>  void Quat_tpl<F>::SetRotationXYZ(const Ang3 &a)	
{ 
	assert(a.IsValid());
	F sx,cx;  sincos_tpl(F(a.x*(F)0.5),&sx,&cx);
	F sy,cy;  sincos_tpl(F(a.y*(F)0.5),&sy,&cy);
	F sz,cz;  sincos_tpl(F(a.z*(F)0.5),&sz,&cz);
	w		= cx*cy*cz + sx*sy*sz;
	v.x = cz*cy*sx - sz*sy*cx;
	v.y = cz*sy*cx + sz*cy*sx;
	v.z = sz*cy*cx - cz*sy*sx;
}

/*!
* Create rotation-quaternion that about the x-axis.
*
* Example:
*		Quat q=Quat::CreateRotationX( radiant );
*   or
*		q.SetRotationX( Ang3(1,2,3) );
*/
template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateRotationX( f32 r ) 
{	
	Quat_tpl<F> q;	q.SetRotationX(r); 	return q;	
}
template<typename F> inline void Quat_tpl<F>::SetRotationX( f32 r ) 
{ 
	F s,c; 
	sincos_tpl(F(r*(F)0.5),&s,&c); w=c; v.x=s; v.y=0; v.z=0;	
}

template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateRotationY( f32 r ) 
{	
	Quat_tpl<F> q;	q.SetRotationY(r); 	return q;	
}
template<typename F> inline void Quat_tpl<F>::SetRotationY( f32 r ) 
{ 
	F s,c; sincos_tpl(F(r*(F)0.5),&s,&c); w=c; v.x=0; v.y=s; v.z=0;	
}

template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateRotationZ( f32 r ) 
{	
	Quat_tpl<F> q;	q.SetRotationZ(r); 	return q;	
}
template<typename F> inline void Quat_tpl<F>::SetRotationZ( f32 r ) 
{ 
	F s,c; sincos_tpl(F(r*(F)0.5),&s,&c); w=c; v.x=0; v.y=0; v.z=s;	
}



/*!
*
* Create rotation-quaternion that rotates from one vector to another.
* Both vectors are assumed to be nomalised.
*
* Example:
*		Quat q=Quat::CreateRotationV0V1( v0,v1 );
*		q.SetRotationV0V1( v0,v1 );
*/
template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateRotationV0V1(const Vec3_tpl<F>& v0, const Vec3_tpl<F>& v1) {	
	Quat_tpl<F> q;	q.SetRotationV0V1(v0,v1); 	return q;	
}
template<typename F> void Quat_tpl<F>::SetRotationV0V1(const Vec3_tpl<F>& v0, const Vec3_tpl<F>& v1)	
{ 
	assert(v0.IsUnit(0.01f));
	assert(v1.IsUnit(0.01f));
	real dot = v0.x*v1.x+v0.y*v1.y+v0.z*v1.z+1.0;
	if (dot > real(0.0001f) ) 
	{
		real vx=v0.y*v1.z-v0.z*v1.y; 
		real vy=v0.z*v1.x-v0.x*v1.z; 
		real vz=v0.x*v1.y-v0.y*v1.x; 
		real d = isqrt_tpl(dot*dot + vx*vx+vy*vy+vz*vz);
		w=F(dot*d);	v.x=F(vx*d); v.y=F(vy*d); v.z=F(vz*d);
		return;
	}
	w=0; v=v0.GetOrthogonal().GetNormalized();
}


/*!
*
* \param vdir  normalized view direction.
* \param roll  radiant to rotate about Y-axis.
*
*  Given a view-direction and a radiant to rotate about Y-axis, this function builds a 3x3 look-at quaternion 
*  using only simple vector arithmetic. This function is always using the implicit up-vector Vec3(0,0,1). 
*  The view-direction is always stored in column(1).
*  IMPORTANT: The view-vector is assumed to be normalized, because all trig-values for the orientation are beeing 
*  extracted  directly out of the vector. This function must NOT be called with a view-direction 
*  that is close to Vec3(0,0,1) or Vec3(0,0,-1). If one of these rules is broken, the function returns a quaternion 
*  with an undifined rotation about the Z-axis.
*
*  Rotation order for the look-at-quaternion is Z-X-Y. (Zaxis=YAW / Xaxis=PITCH / Yaxis=ROLL)
*
*  COORDINATE-SYSTEM       
*                                   
*  z-axis                                 
*    ^                               
*    |                               
*    |  y-axis                   
*    |  /                         
*    | /                           
*    |/                             
*    +--------------->   x-axis     
*                                  
*  Example:
*		Quat LookAtQuat=Quat::CreateRotationVDir( Vec3(0,1,0) );
*   or
*		Quat LookAtQuat=Quat::CreateRotationVDir( Vec3(0,1,0), 0.333f );
*/
template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateRotationVDir( const Vec3_tpl<F>& vdir ) {	Quat_tpl<F> q;	q.SetRotationVDir(vdir); return q;	}
template<typename F> void Quat_tpl<F>::SetRotationVDir( const Vec3_tpl<F>& vdir )
{
	assert(vdir.IsUnit(0.01f));
	//set default initialisation for up-vector	
	w=F(0.70710676908493042);	v.x=F((vdir.z+vdir.z)*0.35355338454246521);	v.y=F(0.0); 	v.z=F(0.0); 
	real l = sqrt(vdir.x*vdir.x+vdir.y*vdir.y);
	if (l>(real)0.00001)	
	{
		//calculate LookAt quaternion
		Vec3r hv	=	Vec3r(vdir.x/l,vdir.y/l+1.0f,l+1.0f);
		real r = sqrt(hv.x*hv.x + hv.y*hv.y);
		real s	= sqrt(hv.z*hv.z + vdir.z*vdir.z);
		//generate the half-angle sine&cosine
		real hacos0=0.0;			real hasin0=-1.0;			
		if (r>(real)0.00001) { hacos0=hv.y/r; hasin0=-hv.x/r; }	//yaw
		real hacos1=hv.z/s;	real hasin1=vdir.z/s;					//pitch
		w=F(hacos0*hacos1); v.x=F(hacos0*hasin1);	v.y=F(hasin0*hasin1);	v.z=F(hasin0*hacos1);  
	}
}

template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateRotationVDir( const Vec3_tpl<F>& vdir, F roll ) {	Quat_tpl<F> q; q.SetRotationVDir(vdir,roll);	return q;	}
template<typename F> void Quat_tpl<F>::SetRotationVDir( const Vec3_tpl<F>& vdir, F r )
{
	SetRotationVDir(vdir);
	real sy,cy;  sincos_tpl(r*(F)0.5,&sy,&cy);
	real vx=v.x,vy=v.y;  
	v.x=F(vx*cy-v.z*sy); v.y=F(w*sy+vy*cy); v.z=F(v.z*cy+vx*sy); w=F(w*cy-vy*sy);
}



/*!
* normalize quaternion.
* 
* Example 1:
*  Quat q;
*  q.Normalize();
*
* Example 2:
*  Quat q=Quat(1,2,3,4);
*  Quat qn=q.GetNormalized();
*/
template <typename F> inline void	Quat_tpl<F>::Normalize(void)	
{
	F d = isqrt_tpl(w*w + v.x*v.x+v.y*v.y+v.z*v.z);
	w*=d;	v.x*=d; v.y*=d; v.z*=d;
}
template <typename F> inline Quat_tpl<F> Quat_tpl<F>::GetNormalized() const
{
	Quat_tpl<F> t=*this; 
	t.Normalize(); 
	return t;	
}


template <typename F> void	Quat_tpl<F>::NormalizeSafe(void)	
{
	F d = w*w + v.x*v.x+v.y*v.y+v.z*v.z;
	if (d > 1e-8f)
	{
		d = isqrt_tpl(d);
		w*=d;	v.x*=d; v.y*=d; v.z*=d;
	}
	else
	{
		SetIdentity();
	}
}
template <typename F> inline Quat_tpl<F> Quat_tpl<F>::GetNormalizedSafe() const
{  
	Quat_tpl<F> t = *this; t.NormalizeSafe(); return t;	
}

// Exponent of Quaternion.
template <typename F> inline Quat_tpl<F> exp(const Vec3_tpl<F>& v) 
{
	F lensqr = v.len2();
	if (lensqr > F(0))
	{
		F len = sqrt_tpl(lensqr);
		F s,c; sincos_tpl(len,&s,&c); 
		s /= len;
		return Quat_tpl<F>( c, v.x*s, v.y*s, v.z*s );
	}
	return Quat_tpl<F> (IDENTITY);
}

// logarithm of a quaternion, imaginary part (the real part of the logarithm is always 0)
template <typename F> inline Vec3_tpl<F> log (const Quat_tpl<F>& q) 
{
	assert(q.IsValid());
	F lensqr = q.v.len2();
	if (lensqr > F(0))


// Exponent of Quaternion.
{
		F len = sqrt_tpl(lensqr);
		F angle = atan2_tpl(len, q.w) / len;
		return q.v * angle;
}

// logarithm of a quaternion, imaginary part (the real part of the logarithm is always 0)
	return Vec3_tpl<F>(0);
}

/*!
* linear-interpolation between quaternions (lerp)
* 
* Example:
*  CQuaternion result,p,q;
*  result=qlerp( p, q, 0.5f );
*/
template<typename F> void Quat_tpl<F>::SetNlerp( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ) 
{	
	Quat_tpl<F> q=tq;
	assert(p.IsValid());
	assert(q.IsValid());
	if( (p|q) < 0 ) { q=-q;	} 

	Vec3_tpl<F> vDiff = q.v - p.v;

	v = p.v + (vDiff * t);
	w	= p.w + ((q.w - p.w) * t);

	// 	v.x = p.v.x*(1.0f-t) + q.v.x*t;
	// 	v.y = p.v.y*(1.0f-t) + q.v.y*t;
	// 	v.z = p.v.z*(1.0f-t) + q.v.z*t;
	// 	w		= p.w  *(1.0f-t) + q.w*t;

	Normalize();
}

template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateNlerp( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ) 
{
	Quat_tpl<F> d;  d.SetNlerp(p,tq,t); 	return d;
}

template<typename F> void Quat_tpl<F>::SetNlerp2( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ) 
{
	Quat_tpl<F> q=tq;
	assert(p.IsValid());
	assert(tq.IsValid());
	F cosine=(p|q);
	if(cosine<0) q=-q; 
	F k=(1-fabs_tpl(cosine))*0.4669269f;
	F s = 2*k*t*t*t - 3*k*t*t + (1+k)*t;
	v.x = p.v.x*(1.0f-s) + q.v.x*s;
	v.y = p.v.y*(1.0f-s) + q.v.y*s;
	v.z = p.v.z*(1.0f-s) + q.v.z*s;
	w		= p.w  *(1.0f-s) + q.w*s;
	Normalize();
}
template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateNlerp2( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ) 
{
	Quat_tpl<F> d;  d.SetNlerp2(p,tq,t); 	return d;
}

/*!
* spherical-interpolation between quaternions (geometrical slerp)
* 
* Example:
*  Quat result,p,q;
*  result.SetSlerp( p, q, 0.5f );
*/
template<typename F> void Quat_tpl<F>::SetSlerp( const Quat_tpl<F> &tp, const Quat_tpl<F> &tq, F t ) 
{
	assert(tp.IsValid());
	assert(tq.IsUnit());
	Quat_tpl<F> p=tp,q=tq;
	Quat_tpl<F> q2;

	F cosine = (p|q);
	if (cosine < 0.0f ) { cosine=-cosine; q=-q;	} //take shortest arc
	if (cosine > 0.9999f)
	{
		SetNlerp(p,q,t);
		return;
	}
	// from now on, a division by 0 is not possible any more
	q2.w		= q.w-p.w*cosine;
	q2.v.x	= q.v.x-p.v.x*cosine;
	q2.v.y	= q.v.y-p.v.y*cosine;
	q2.v.z	= q.v.z-p.v.z*cosine;
	F sine	= sqrt(q2|q2);
	F s,c;	

	sincos_tpl(atan2f(sine,cosine)*t,&s,&c);
	w =   F(p.w  *c + q2.w  *s/sine);
	v.x = F(p.v.x*c + q2.v.x*s/sine);
	v.y = F(p.v.y*c + q2.v.y*s/sine);
	v.z = F(p.v.z*c + q2.v.z*s/sine);
}

template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateSlerp( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ) 
{
	Quat_tpl<F> d;  d.SetSlerp(p,tq,t); 	return d;
}


/*!
* spherical-interpolation between quaternions (algebraic slerp_a)
* I have included this function just for the sake of completeness, because 
* its the only useful application to check if exp & log really work. 
* Both slerp-functions are returning the same result.
*	
* Example:
*  Quat result,p,q;
*  result.SetExpSlerp( p,q,0.3345f );
*/
template <typename F> inline void Quat_tpl<F>::SetExpSlerp( const Quat_tpl<F> &p, const Quat_tpl<F> &tq, F t ) 
{
	assert(p.IsValid());
	assert(tq.IsValid());
	Quat_tpl<F> q=tq;
	if((p|q)<0) { q=-q;	} 
	*this = p * exp( log(!p*q)*t );			//algebraic slerp (1)

	//...and some more exp-slerp-functions producing all the same result
	//*this = exp( log (p* !q) * (1-t)) * q;		    //algebraic slerp (2)
	//*this = exp( log (q* !p) * t) * p;			//algebraic slerp (3)
	//*this = q * exp( log (!q*p) * (1-t));	//algebraic slerp (4)
}
template<typename F> inline Quat_tpl<F> Quat_tpl<F>::CreateExpSlerp( const Quat_tpl<F> &p, const Quat_tpl<F> &q, F t ) 
{
	Quat_tpl<F> d;  d.SetExpSlerp(p,q,t); 	return d;
}


//! squad(p,a,b,q,t) = slerp( slerp(p,q,t),slerp(a,b,t), 2(1-t)t).
template <typename F>
inline Quat_tpl<F> Quat_tpl<F>::CreateSquad( const Quat_tpl<F>& p,const Quat_tpl<F>& a,const Quat_tpl<F>& b,const Quat_tpl<F>& q, F t )
{
	F k = 2.0f*(1.0f-t)*t;
	return CreateSlerp( CreateSlerp(p,q,t), CreateSlerp(a,b,t), k );
}


//! Quaternion interpolation for angles > 2PI.
template <typename F>
Quat_tpl<F> Quat_tpl<F>::CreateSquadRev(	F angle,				// angle of rotation 
											const Vec3& axis,	// the axis of rotation 
											const Quat_tpl<F>& p,		// start quaternion 
											const Quat_tpl<F>& a, 	// start tangent quaternion 
											const Quat_tpl<F>& b, 	// end tangent quaternion 
											const Quat_tpl<F>& q,		// end quaternion 
											F t	)						// Time parameter, in range [0,1]
		{
	F s,v;
	F omega = 0.5f*angle;
	F nrevs = 0;
	Quat_tpl<F> r,pp,qq;

	if (omega < (gf_PI - 0.00001f)) { 
		return CreateSquad( p,a,b,q,t );
	}

	while (omega > (gf_PI - 0.00001f)) { 
		omega -= gf_PI;
		nrevs += 1.0f;
	}
	if (omega < 0) omega = 0;
	s = t*angle/gf_PI;		// 2t(omega+Npi)/pi

	if (s < 1.0f) {
		pp = p*Quat_tpl<F>(0.0f,axis);//pp = p.Orthog( axis );
		r = CreateSquad(p,a,pp,pp,s);	// in first 90 degrees.
	}	else {
		v = s + 1.0f - 2.0f*(nrevs+(omega/gf_PI));
		if (v <= 0.0f)  {
			// middle part, on great circle(p,q).
			while (s >= 2.0f) s -= 2.0f;
			pp = p*Quat_tpl<F>(0.0f,axis);//pp = p.Orthog(axis);
			r = CreateSlerp(p,pp,s);
		}	else {
			// in last 90 degrees.
			qq = -q*Quat_tpl<F>(0.0f,axis);
			r = CreateSquad(qq,qq,b,q,v);
		}
	}
	return r;
}




//////////////////////////////////////////////////////////////////////////
//! Logarithm of Quaternion difference.
template <typename F>
inline Quat_tpl<F> LnDif( const Quat_tpl<F> &q1,const Quat_tpl<F> &q2 ){
	return Quat(0,log(q2/q1));
}


/*!
*
* quaternion copy constructor; Quat q=mat33
* We take only the rotation of the 3x3 part 
* 
* Example 1:
* \code
*  Matrix33 mat33;
*  mat33.rotationXYZ33(0.5f, 2.5f, 1.5f);
*  Quat q=mat33;
* \endcode
* 
* Example 2:
* \code
*  CMatrix34 mat34;
*  mat34.rotationXYZ34(0.5f, 2.5f, 1.5f);
*  Quat q=Matrix33(mat34);
* \endcode
*/
template<typename F> Quat_tpl<F> inline GetQuatFromMat44(const Matrix44_tpl<F>& m)	{ 
	//check if we have an orthonormal-base (assuming we are using a right-handed coordinate system)
	//assert removed by ivo: it was impossible to load a level because of this assert!
	//but be warned, if you convert a non-uniform-scaled matrix into a quaternion 
	//you get a worthless quaternion!
	assert( m.IsOrthonormalRH(0.1f) );
	F s,p,tr = m.m00 + m.m11 + m.m22;

	//check the diagonal
	if(tr > (F)0.0) {
		s=sqrt_tpl(tr+1.0f);	assert(s); p=0.5f/s;
		return Quat_tpl<F>( s*0.5f, (m.m21-m.m12)*p, (m.m02-m.m20)*p, (m.m10-m.m01)*p );
	}
	//diagonal is negative. now we have to find the biggest element on the diagonal
	//check if "m00" is the biggest element
	if	( (m.m00>=m.m11) && (m.m00>=m.m22) ) {
		s=sqrt_tpl(m.m00-m.m11-m.m22+1.0f);	assert(s);	p=0.5f/s;
		return Quat_tpl<F>( (m.m21-m.m12)*p, s*0.5f, (m.m10+m.m01)*p, (m.m20+m.m02)*p );
	}
	//check if "m11" is the biggest element
	if	( (m.m11>=m.m00) && (m.m11>=m.m22) ) {
		s=sqrt_tpl(m.m11-m.m22-m.m00+1.0f);	assert(s); p=0.5f/s;
		return Quat_tpl<F>( (m.m02-m.m20)*p, (m.m01+m.m10)*p, s*0.5f, (m.m21+m.m12)*p );
	}
	//check if "m22" is the biggest element
	if	( (m.m22>=m.m00) && (m.m22>=m.m11) ) {
		s=sqrt_tpl(m.m22-m.m00-m.m11+1.0f);	assert(s); p=0.5f/s;
		return Quat_tpl<F>( (m.m10-m.m01)*p, (m.m02+m.m20)*p, (m.m12+m.m21)*p, s*0.5f );
	}
	assert(0); return Quat_tpl<F>(IDENTITY);//if it ends here, then we have no valid rotation matrix
}

