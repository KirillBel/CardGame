#pragma once

//MATRIX
//			0	1	2	3
//		 -----------------
//	0	|	0	1	2	3
//	1	|	4	5	6	7
//	2	|	8	9	10	11
//	3	|	12	13	14	16

template<typename F> 
class Matrix44T
{
public:
	F m00,m01,m02,m03;
	F m10,m11,m12,m13;
	F m20,m21,m22,m23;
	F m30,m31,m32,m33;

	template<class T>
	inline static Matrix44T CreateScale(const Vec3T<T>& scale)
	{
		Matrix44T<F> mat;
		mat.setScale(scale);
		return mat;
	};

	template<class T>
	inline static Matrix44T CreateTranslation(const Vec3T<T>& vec)
	{
		Matrix44T<F> mat;
		mat.setIdentity();
		mat.setTranslation(vec);
		return mat;
	};

	template<class T>
	inline static Matrix44T CreateRotationXYZ(Vec3T<T>& vec)
	{
		Matrix44T<F> mat;
		mat.setIdentity();
		mat.setRotationXYZ(vec);
		return mat;
	};

	template<class T>
	inline static Matrix44T CreateRotationQuat(T x, T y, T z, T w)
	{
		Matrix44T<F> mat;
		mat.setIdentity();
		mat(0,0)=1-2*y*y-2*z*z;
		mat(0,1)=2*x*y-2*z*w;
		mat(0,2)=2*x*z+2*y*w;
		mat(1,0)=2*x*y+2*z*w;
		mat(1,1)=1-2*x*x-2*z*z;
		mat(1,2)=2*y*z-2*x*w;
		mat(2,0)=2*x*z-2*y*w;
		mat(2,1)=2*y*z+2*x*w;
		mat(2,2)=1-2*x*x-2*y*y;
		return mat;
	};

	template<class T>
	inline static Matrix44T CreateRotationQuat(QuatT<T>& quat)
	{
		return createRotationQuat(quat.v.x,quat.v.y,quat.v.z,quat.w);
	};


	template<class T>
	inline static Matrix44T CreateRotationXYZ(Ang3T<T>& vec)
	{
		return CreateRotationXYZ(vec.toVec3());
	};

	template <class T>
	inline static Matrix44T CreateRotationAA(F angle, Vec3T<T> const& axis) 
	{
		Matrix44T<F> mat;
		mat.setIdentity();
		mat.setRotationAA(angle,axis);
		return mat;
	};

	inline static Matrix44T CreateRotationX(F angle) 
	{
		Matrix44T<F> mat;
		mat.setIdentity();
		mat.setRotationX(angle);
		return mat;
	};

	inline static Matrix44T CreateRotationY(F angle) 
	{
		Matrix44T<F> mat;
		mat.setIdentity();
		mat.setRotationX(angle);
		return mat;
	};

	inline static Matrix44T CreateRotationZ(F angle) 
	{
		Matrix44T<F> mat;
		mat.setIdentity();
		mat.setRotationX(angle);
		return mat;
	};

	inline static Matrix44T CreatePerspectiveFovLH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar) 
	{
		Matrix44T<F> mat;
		
		const f64 h = 1.f/tan(fieldOfViewRadians*0.5);
		const F w = (F)(h / aspectRatio);

		assert(zNear!=zFar); //divide by zero
		mat[0] = w;
		mat[1] = 0;
		mat[2] = 0;
		mat[3] = 0;

		mat[4] = 0;
		mat[5] = (F)h;
		mat[6] = 0;
		mat[7] = 0;

		mat[8] = 0;
		mat[9] = 0;
		mat[10] = (F)(zFar/(zFar-zNear));
		mat[11] = 1;

		mat[12] = 0;
		mat[13] = 0;
		mat[14] = (F)(-zNear*zFar/(zFar-zNear));
		mat[15] = 0;
		return mat;
	};

	inline static Matrix44T CreatePerspectiveOrthoLH(float width,float height,float znear,float zfar)
	{
		Matrix44T<F> mat;
		mat.setIdentity();

		mat(0,0)=2/width;
		mat(1,1)=2/height;
		mat(2,2)=1/(zfar-znear);
		mat(3,2)=znear/(znear-zfar);
		return mat;
	};

	inline static Matrix44T CreatePerspectiveOrthoOffCenterLH(float left,float right, float bottom, float top, float znear,float zfar)
	{
		Matrix44T<F> mat;
		mat.setIdentity();

		mat(0,0)=2/(right-left);
		mat(3,0)=(left+right)/(left-right) ;

		mat(1,1)=2/(top-bottom);
		mat(3,1)=(top+bottom)/(bottom-top);

		mat(2,2)=1/(zfar-znear);
		mat(3,2)=znear/(znear-zfar);
		return mat;
	};

	template <class T>
	inline static Matrix44T CreateLookAtLH(const Vec3T<T>& position,const Vec3T<T>& target, const Vec3T<T>& upVector) 
	{
		if(position.getDistance(target)==0)
		{
			return getIdentity();
		};

		Matrix44T<F> M;
		Vec3T<F> zaxis = target - position;
		zaxis.normalize();

		Vec3T<F> xaxis = upVector.cross(zaxis);
		xaxis.normalize();

		Vec3T<F> yaxis = zaxis.cross(xaxis);

		M[0] = (F)xaxis.x;
		M[1] = (F)yaxis.x;
		M[2] = (F)zaxis.x;
		M[3] = 0;

		M[4] = (F)xaxis.y;
		M[5] = (F)yaxis.y;
		M[6] = (F)zaxis.y;
		M[7] = 0;

		M[8] = (F)xaxis.z;
		M[9] = (F)yaxis.z;
		M[10] = (F)zaxis.z;
		M[11] = 0;

		M[12] = (F)-xaxis.dot(position);
		M[13] = (F)-yaxis.dot(position);
		M[14] = (F)-zaxis.dot(position);
		M[15] = 1;
		return M;
	};
	//////////CONSTRUCTOR///////////////////////////////
	inline Matrix44T()
	{
		m00=NULL;	m01=NULL;	m02=NULL;	m03=NULL;
		m10=NULL;	m11=NULL;	m12=NULL;	m13=NULL;
		m20=NULL;	m21=NULL;	m22=NULL;	m23=NULL;
		m30=NULL;	m31=NULL;	m32=NULL;	m33=NULL;
	};

	template<class F1> inline Matrix44T(const Matrix44T<F1>& m ) 
	{
		assert(m.IsValid());
		m00=(F)m.m00;		m01=(F)m.m01;		m02=(F)m.m02;	m03=(F)m.m03;	
		m10=(F)m.m10;		m11=(F)m.m11;		m12=(F)m.m12;	m13=(F)m.m13;
		m20=(F)m.m20;		m21=(F)m.m21;		m22=(F)m.m22;	m23=(F)m.m23;
		m30=(F)m.m30;		m31=(F)m.m31;		m32=(F)m.m32;	m33=(F)m.m33;
	}

	Matrix44T(	f32 v00, f32 v01, f32 v02, f32 v03,
				f32 v10, f32 v11, f32 v12, f32 v13,
				f32 v20, f32 v21, f32 v22, f32 v23,
				f32 v30, f32 v31, f32 v32, f32 v33) :
		m00(v00), m01(v01), m02(v02), m03(v03), 
		m10(v10), m11(v11), m12(v12), m13(v13), 
		m20(v20), m21(v21), m22(v22), m23(v23), 
		m30(v30), m31(v31), m32(v32), m33(v33) 
	{ 	
	};

	///////END CONSTRUCTOR///////////////////////////////

	///////ASSESS////////////////////////////////////////

	inline F& operator () (uint32 Row, uint32 Col) const
	{	
		assert ((Row<4) && (Col<4));	
		//F* p_data=(F*)(&m00);		
		return ((float*)this)[Row*4+Col];	
	}

	inline F& operator () (uint32 index) const
	{	
		assert (index<16);	
		F* p_data=(F*)(&m00);		
		return p_data[index];	
	}

	inline F& operator [] (uint32 index) const	
	{	
		assert (index<16);	
		F* p_data=(F*)(&m00);		
		return p_data[index];
	}

	inline F* ptr() const
	{
		return (F*)(&m00);
	};
	///////END ASSESS////////////////////////////////////

	/////////////////EQUATIONS///////////////////////////
	
	inline Matrix44T& operator = (const Matrix44T<F> &m) 
	{ 
		//assert(m.IsValid());
		m00=m.m00;	m01=m.m01;	m02=m.m02; m03=m.m03; 
		m10=m.m10;	m11=m.m11;	m12=m.m12; m13=m.m13; 
		m20=m.m20;	m21=m.m21;	m22=m.m22; m23=m.m23; 
		m30=m.m30;	m31=m.m31;	m32=m.m32; m33=m.m33; 

		return *this;
	};

	inline Matrix44T& operator = (const F& scalar) 
	{ 
		for(int i=0;i<16;i++)
		{
			if(*this(i)=scalar);
		};

		return *this;
	};

	inline bool operator == (const Matrix44T<F> &m) const
	{ 
		for(int i=0;i<16;i++)
		{
			if(((*this)(i))!=m(i)) 
				return false;
		};
		return true;
	};

	inline bool operator != (const Matrix44T<F> &m) const
	{ 
		return !(*this==m);
	};

	/////////////END EQUATIONS///////////////////////////

	/////////////////ARIFMETIC///////////////////////////

	//ADDITION
	inline Matrix44T operator + (const Matrix44T<F> &m) 
	{
		Matrix44T<F> temp=*this;
		temp.m00+=m.m00;	temp.m01+=m.m01;	temp.m02+=m.m02;	temp.m03+=m.m03;
		temp.m10+=m.m10;	temp.m11+=m.m11;	temp.m12+=m.m12;	temp.m13+=m.m13;
		temp.m20+=m.m20;	temp.m21+=m.m21;	temp.m22+=m.m22;	temp.m23+=m.m23;
		temp.m30+=m.m30;	temp.m31+=m.m31;	temp.m32+=m.m32;	temp.m33+=m.m33;

		return temp;
	};

	inline Matrix44T& operator += (const Matrix44T<F> &m) 
	{
		*this.m00+=m.m00;	*this.m01+=m.m01;	*this.m02+=m.m02;	*this.m03+=m.m03;
		*this.m10+=m.m10;	*this.m11+=m.m11;	*this.m12+=m.m12;	*this.m13+=m.m13;
		*this.m20+=m.m20;	*this.m21+=m.m21;	*this.m22+=m.m22;	*this.m23+=m.m23;
		*this.m30+=m.m30;	*this.m31+=m.m31;	*this.m32+=m.m32;	*this.m33+=m.m33;

		return *this;
	};

	//SUBSTRACTION
	inline Matrix44T operator - (const Matrix44T<F> &m) 
	{
		Matrix44T<F> temp=*this;
		temp.m00-=m.m00;	temp.m01-=m.m01;	temp.m02-=m.m02;	temp.m03-=m.m03;
		temp.m10-=m.m10;	temp.m11-=m.m11;	temp.m12-=m.m12;	temp.m13-=m.m13;
		temp.m20-=m.m20;	temp.m21-=m.m21;	temp.m22-=m.m22;	temp.m23-=m.m23;
		temp.m30-=m.m30;	temp.m31-=m.m31;	temp.m32-=m.m32;	temp.m33-=m.m33;

		return temp;
	};

	inline Matrix44T& operator -= (const Matrix44T<F> &m) 
	{
		*this.m00-=m.m00;	*this.m01-=m.m01;	*this.m02-=m.m02;	*this.m03-=m.m03;
		*this.m10-=m.m10;	*this.m11-=m.m11;	*this.m12-=m.m12;	*this.m13-=m.m13;
		*this.m20-=m.m20;	*this.m21-=m.m21;	*this.m22-=m.m22;	*this.m23-=m.m23;
		*this.m30-=m.m30;	*this.m31-=m.m31;	*this.m32-=m.m32;	*this.m33-=m.m33;

		return *this;
	};

	//MULTIPLY
	inline Matrix44T operator * (Matrix44T<F> &m) 
	{
		return multiply(*this,m);
	};

	inline Matrix44T& operator *= (Matrix44T<F> &m) 
	{
		*this=multiply(*this,m);
		return *this;
	};

	/*inline static Matrix44T multiply(Matrix44T<F> &m1,Matrix44T<F> &m2)
	{
		Matrix44T M;
		M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		M[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		M[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		M[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		return M;
	};*/

	inline static Matrix44T multiply(const Matrix44T<F>& l, const Matrix44T<F>& r)
	{
		Matrix44T<F> res;
		res.m00 = l.m00*r.m00 + l.m01*r.m10 + l.m02*r.m20 + l.m03*r.m30;
		res.m10 = l.m10*r.m00 + l.m11*r.m10 + l.m12*r.m20 + l.m13*r.m30;
		res.m20 = l.m20*r.m00 + l.m21*r.m10 + l.m22*r.m20 + l.m23*r.m30;
		res.m30 = l.m30*r.m00 + l.m31*r.m10 + l.m32*r.m20 + l.m33*r.m30;

		res.m01 = l.m00*r.m01 + l.m01*r.m11 + l.m02*r.m21 + l.m03*r.m31;
		res.m11 = l.m10*r.m01 + l.m11*r.m11 + l.m12*r.m21 + l.m13*r.m31;
		res.m21 = l.m20*r.m01 + l.m21*r.m11 + l.m22*r.m21 + l.m23*r.m31;
		res.m31 = l.m30*r.m01 + l.m31*r.m11 + l.m32*r.m21 + l.m33*r.m31;

		res.m02 = l.m00*r.m02 + l.m01*r.m12 + l.m02*r.m22 + l.m03*r.m32;
		res.m12 = l.m10*r.m02 + l.m11*r.m12 + l.m12*r.m22 + l.m13*r.m32;
		res.m22 = l.m20*r.m02 + l.m21*r.m12 + l.m22*r.m22 + l.m23*r.m32;
		res.m32 = l.m30*r.m02 + l.m31*r.m12 + l.m32*r.m22 + l.m33*r.m32;

		res.m03 = l.m00*r.m03 + l.m01*r.m13 + l.m02*r.m23 + l.m03*r.m33;
		res.m13 = l.m10*r.m03 + l.m11*r.m13 + l.m12*r.m23 + l.m13*r.m33;
		res.m23 = l.m20*r.m03 + l.m21*r.m13 + l.m22*r.m23 + l.m23*r.m33;
		res.m33 = l.m30*r.m03 + l.m31*r.m13 + l.m32*r.m23 + l.m33*r.m33;
		return res;
	}

	/////////////END ARIFMETIC///////////////////////////

	/////////////////GRAPHICS//////////////////////////////

	template <class T>
	inline void setTranslation(const Vec3T<T>& vec)
	{
		m30=vec.x;
		m31=vec.y;
		m32=vec.z;
	};

	inline Vec3T<F> getTranslation()
	{
		return Vec3T<F>(m30,m31,m32);
	};

	template <class T>
	inline void setScale(const Vec3T<T>& vec)
	{
		m00=vec.x;
		m11=vec.y;
		m22=vec.z;
		m33=1;
	};

	inline Vec3T<F> getScale()
	{
		return Vec3T<F>(m00,m11,m22);
	};

	template <class T>
	inline void setRotationXYZ( const Vec3T<T>& rotation )
	{
		const f64 cr = cos( rotation.x );
		const f64 sr = sin( rotation.x );
		const f64 cp = cos( rotation.y );
		const f64 sp = sin( rotation.y );
		const f64 cy = cos( rotation.z );
		const f64 sy = sin( rotation.z );

		m00 = (T)( cp*cy );
		m01 = (T)( cp*sy );
		m02 = (T)( -sp );

		const f64 srsp = sr*sp;
		const f64 crsp = cr*sp;

		m10 = (T)( srsp*cy-cr*sy );
		m11 = (T)( srsp*sy+cr*cy );
		m12 = (T)( sr*cp );

		m20 = (T)( crsp*cy+sr*sy );
		m21 = (T)( crsp*sy-sr*cy );
		m22 = (T)( cr*cp );
	};

	template <class T>
	inline void setRotationAA(F angle, Vec3T<T> const& axis) {	
		f64 s,c; matkit::_sincos(angle,&s,&c);	f64	mc=1.0-c;	
		
		f64 mcx=mc*axis.x;	f64 mcy=mc*axis.y;	f64 mcz=mc*axis.z; 
		f64 tcx=axis.x*s;	f64 tcy=axis.y*s;	f64 tcz=axis.z*s;

		m00=F(mcx*axis.x+c);	m01=F(mcx*axis.y-tcz);	m02=F(mcx*axis.z+tcy);	
		m10=F(mcy*axis.x+tcz);	m11=F(mcy*axis.y+c);	m12=F(mcy*axis.z-tcx);	
		m20=F(mcz*axis.x-tcy);	m21=F(mcz*axis.y+tcx);	m22=F(mcz*axis.z+c);					
	};

	void setRotationX(const f32 rad )	{
		F s,c; matkit::_sincosf(rad,&s,&c);
		m00=1.0f;		m01=0.0f;		m02=	0.0f;		
		m10=0.0f;		m11=c;			m12=-s;
		m20=0.0f;		m21=s;			m22= c;
	}

	void setRotationY(const f32 rad ) {
		F s,c; matkit::_sincosf(rad,&s,&c);
		m00	=	c;		m01	=	0;		m02	=	s;
		m10	=	0;		m11	=	1;		m12	=	0;			
		m20	=-s;		m21	=	0;		m22	= c;	
	}

	void setRotationZ( const f32 rad ) {
		F s,c; matkit::_sincosf(rad,&s,&c);
		m00	=	c;			m01	=-s;			m02	=	0.0f;	
		m10	=	s;			m11	=	c;			m12	=	0.0f;	
		m20	=	0.0f;		m21	=	0.0f;		m22	= 1.0f;
	}
	/////////////END GRAPHICS//////////////////////////////

	/////////////////OTHERS//////////////////////////////

	inline bool isIdentity() 
	{
		if((m00!=1) || (m11!=1) || (m22!=1) || (m33!=1)) return false;

		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				if(i==j) continue;
				if(*this(i,j)!=0) return false;
			};
		};
		return true;
	};

	inline void setIdentity()
	{
		*this=getIdentity();
	};

	inline static Matrix44T getIdentity()
	{
		return Matrix44T<F>(1,0,0,0,	0,1,0,0,	0,0,1,0,	0,0,0,1);
	};

	inline bool isOrthogonal()
	{
		F dp=ptr()[0] * ptr()[4 ] + ptr()[1] * ptr()[5 ] + ptr()[2 ] * ptr()[6 ] + ptr()[3 ] * ptr()[7 ];
		if (dp!=0)
			return false;
		dp = ptr()[0] * ptr()[8 ] + ptr()[1] * ptr()[9 ] + ptr()[2 ] * ptr()[10] + ptr()[3 ] * ptr()[11];
		if (dp!=0)
			return false;
		dp = ptr()[0] * ptr()[12] + ptr()[1] * ptr()[13] + ptr()[2 ] * ptr()[14] + ptr()[3 ] * ptr()[15];
		if (dp!=0)
			return false;
		dp = ptr()[4] * ptr()[8 ] + ptr()[5] * ptr()[9 ] + ptr()[6 ] * ptr()[10] + ptr()[7 ] * ptr()[11];
		if (dp!=0)
			return false;
		dp = ptr()[4] * ptr()[12] + ptr()[5] * ptr()[13] + ptr()[6 ] * ptr()[14] + ptr()[7 ] * ptr()[15];
		if (dp!=0)
			return false;
		dp = ptr()[8] * ptr()[12] + ptr()[9] * ptr()[13] + ptr()[10] * ptr()[14] + ptr()[11] * ptr()[15];
		return (dp==0);
	};


	inline Vec4T<F> getColumnV4(int colNum)
	{
		_ASSERT((colNum>=0) && (colNum<4));
		return Vec4(ptr()[0*4+colNum],ptr()[1*4+colNum],ptr()[2*4+colNum],ptr()[3*4+colNum]);
	};

	inline Vec3T<F> getColumnV3(int colNum)
	{
		_ASSERT((colNum>=0) && (colNum<4));
		return Vec3(ptr()[0*4+colNum],ptr()[1*4+colNum],ptr()[2*4+colNum]);
	};

	inline Vec4T<F> getRowV4(int rowNum)
	{
		_ASSERT((rowNum>=0) && (rowNum<4));
		return Vec4(ptr()[rowNum*4],ptr()[rowNum*4+1],ptr()[rowNum*4+2],ptr()[rowNum*4+3]);
	};

	inline Vec3T<F> getRowV3(int rowNum)
	{
		_ASSERT((rowNum>=0) && (rowNum<4));
		return Vec3(ptr()[rowNum*4],ptr()[rowNum*4+1],ptr()[rowNum*4+2]);
	};

	inline Vec4T<F> getColumn0() const	{ return Vec4T<F> (m00,m10,m20,m30);	}
	inline Vec4T<F> getColumn1() const	{ return Vec4T<F> (m01,m11,m21,m31);	}
	inline Vec4T<F> getColumn2() const	{ return Vec4T<F> (m02,m12,m22,m32);	}
	inline Vec4T<F> getColumn3() const	{ return Vec4T<F> (m03,m13,m23,m33);	}

	int isOrthonormalRH(F threshold=0.001) const { 
		int a=GetColumnV3(0).IsEquivalent(GetColumnV3(1)%GetColumnV3(2),threshold);
		int b=GetColumnV3(1).IsEquivalent(GetColumnV3(2)%GetColumnV3(0),threshold);
		int c=GetColumnV3(2).IsEquivalent(GetColumnV3(0)%GetColumnV3(1),threshold);
		return a&b&c;
	}
	bool equals( const Matrix44T<F>& m, F e=VEC_EPSILON) const {
		return  ( 
			(fabs(m00-m.m00)<=e) && (fabs(m01-m.m01)<=e) && (fabs(m02-m.m02)<=e) && (fabs(m03-m.m03)<=e) && 
			(fabs(m10-m.m10)<=e) && (fabs(m11-m.m11)<=e) && (fabs(m12-m.m12)<=e) && (fabs(m13-m.m13)<=e) && 
			(fabs(m20-m.m20)<=e) && (fabs(m21-m.m21)<=e) && (fabs(m22-m.m22)<=e) && (fabs(m23-m.m23)<=e) 
			);	
	}

	//inline bool invert( void ) {
	//	F	tmp[12];
	//	Matrix44T<F>	m=*this;

	//	// Calculate pairs for first 8 elements (cofactors)
	//	tmp[0] = m.m22 * m.m33;
	//	tmp[1] = m.m32 * m.m23;
	//	tmp[2] = m.m12 * m.m33;
	//	tmp[3] = m.m32 * m.m13;
	//	tmp[4] = m.m12 * m.m23;
	//	tmp[5] = m.m22 * m.m13;
	//	tmp[6] = m.m02 * m.m33;
	//	tmp[7] = m.m32 * m.m03;
	//	tmp[8] = m.m02 * m.m23;
	//	tmp[9] = m.m22 * m.m03;
	//	tmp[10]= m.m02 * m.m13;
	//	tmp[11]= m.m12 * m.m03;

	//	// Calculate first 8 elements (cofactors)
	//	m00 = tmp[0]*m.m11 + tmp[3]*m.m21 + tmp[ 4]*m.m31;
	//	m00-= tmp[1]*m.m11 + tmp[2]*m.m21 + tmp[ 5]*m.m31;
	//	m01 = tmp[1]*m.m01 + tmp[6]*m.m21 + tmp[ 9]*m.m31;
	//	m01-= tmp[0]*m.m01 + tmp[7]*m.m21 + tmp[ 8]*m.m31;
	//	m02 = tmp[2]*m.m01 + tmp[7]*m.m11 + tmp[10]*m.m31;
	//	m02-= tmp[3]*m.m01 + tmp[6]*m.m11 + tmp[11]*m.m31;
	//	m03 = tmp[5]*m.m01 + tmp[8]*m.m11 + tmp[11]*m.m21;
	//	m03-= tmp[4]*m.m01 + tmp[9]*m.m11 + tmp[10]*m.m21;
	//	m10 = tmp[1]*m.m10 + tmp[2]*m.m20 + tmp[ 5]*m.m30;
	//	m10-= tmp[0]*m.m10 + tmp[3]*m.m20 + tmp[ 4]*m.m30;
	//	m11 = tmp[0]*m.m00 + tmp[7]*m.m20 + tmp[ 8]*m.m30;
	//	m11-= tmp[1]*m.m00 + tmp[6]*m.m20 + tmp[ 9]*m.m30;
	//	m12 = tmp[3]*m.m00 + tmp[6]*m.m10 + tmp[11]*m.m30;
	//	m12-= tmp[2]*m.m00 + tmp[7]*m.m10 + tmp[10]*m.m30;
	//	m13 = tmp[4]*m.m00 + tmp[9]*m.m10 + tmp[10]*m.m20;
	//	m13-= tmp[5]*m.m00 + tmp[8]*m.m10 + tmp[11]*m.m20;

	//	// Calculate pairs for second 8 elements (cofactors)
	//	tmp[ 0] = m.m20*m.m31;
	//	tmp[ 1] = m.m30*m.m21;
	//	tmp[ 2] = m.m10*m.m31;
	//	tmp[ 3] = m.m30*m.m11;
	//	tmp[ 4] = m.m10*m.m21;
	//	tmp[ 5] = m.m20*m.m11;
	//	tmp[ 6] = m.m00*m.m31;
	//	tmp[ 7] = m.m30*m.m01;
	//	tmp[ 8] = m.m00*m.m21;
	//	tmp[ 9] = m.m20*m.m01;
	//	tmp[10] = m.m00*m.m11;
	//	tmp[11] = m.m10*m.m01;

	//	// Calculate second 8 elements (cofactors)
	//	m20 = tmp[ 0]*m.m13 + tmp[ 3]*m.m23 + tmp[ 4]*m.m33;
	//	m20-= tmp[ 1]*m.m13 + tmp[ 2]*m.m23 + tmp[ 5]*m.m33;
	//	m21 = tmp[ 1]*m.m03 + tmp[ 6]*m.m23 + tmp[ 9]*m.m33;
	//	m21-= tmp[ 0]*m.m03 + tmp[ 7]*m.m23 + tmp[ 8]*m.m33;
	//	m22 = tmp[ 2]*m.m03 + tmp[ 7]*m.m13 + tmp[10]*m.m33;
	//	m22-= tmp[ 3]*m.m03 + tmp[ 6]*m.m13 + tmp[11]*m.m33;
	//	m23 = tmp[ 5]*m.m03 + tmp[ 8]*m.m13 + tmp[11]*m.m23;
	//	m23-= tmp[ 4]*m.m03 + tmp[ 9]*m.m13 + tmp[10]*m.m23;
	//	m30 = tmp[ 2]*m.m22 + tmp[ 5]*m.m32 + tmp[ 1]*m.m12;
	//	m30-= tmp[ 4]*m.m32 + tmp[ 0]*m.m12 + tmp[ 3]*m.m22;
	//	m31 = tmp[ 8]*m.m32 + tmp[ 0]*m.m02 + tmp[ 7]*m.m22;
	//	m31-= tmp[ 6]*m.m22 + tmp[ 9]*m.m32 + tmp[ 1]*m.m02;
	//	m32 = tmp[ 6]*m.m12 + tmp[11]*m.m32 + tmp[ 3]*m.m02;
	//	m32-= tmp[10]*m.m32 + tmp[ 2]*m.m02 + tmp[ 7]*m.m12;
	//	m33 = tmp[10]*m.m22 + tmp[ 4]*m.m02 + tmp[ 9]*m.m12;
	//	m33-= tmp[ 8]*m.m12 + tmp[11]*m.m22 + tmp[ 5]*m.m02;

	//	// Calculate determinant
	//	F det=(m.m00*m00+m.m10*m01+m.m20*m02+m.m30*m03);
	//	if (matkit::_fabsf(det)<0.0001f) return false;

	//	// Divide the cofactor-matrix by the determinant
	//	F idet=(F)1.0/det;
	//	m00*=idet; m01*=idet; m02*=idet; m03*=idet;
	//	m10*=idet; m11*=idet; m12*=idet; m13*=idet;
	//	m20*=idet; m21*=idet; m22*=idet; m23*=idet;
	//	m30*=idet; m31*=idet; m32*=idet; m33*=idet;
	//	return true;
	//}

	inline bool invert()
	{
		Matrix44T<F> &m = *this;
		f32 d = (m00 * m11 - m01 * m10) * (m22 * m33 - m23 * m32) -
			(m00 * m12 - m02 * m10) * (m21 * m33 - m23 * m31) +
			(m00 * m13 - m03 * m10) * (m21 * m32 - m22 * m31) +
			(m01 * m12 - m02 * m11) * (m20 * m33 - m23 * m30) -
			(m01 * m13 - m03 * m11) * (m20 * m32 - m22 * m30) +
			(m02 * m13 - m03 * m12) * (m20 * m31 - m21 * m30);

		if(d==0) return false;

		d=1.f/d;

		Matrix44T<F> out;

		out(0, 0) = d * (m11 * (m22 * m33 - m23 * m32) +
				m12 * (m23 * m31 - m21 * m33) +
				m13 * (m21 * m32 - m22 * m31));
		out(0, 1) = d * (m21 * (m02 * m33 - m03 * m32) +
				m22 * (m03 * m31 - m01 * m33) +
				m23 * (m01 * m32 - m02 * m31));
		out(0, 2) = d * (m31 * (m02 * m13 - m03 * m12) +
				m32 * (m03 * m11 - m01 * m13) +
				m33 * (m01 * m12 - m02 * m11));
		out(0, 3) = d * (m01 * (m13 * m22 - m12 * m23) +
				m02 * (m11 * m23 - m13 * m21) +
				m03 * (m12 * m21 - m11 * m22));
		out(1, 0) = d * (m12 * (m20 * m33 - m23 * m30) +
				m13 * (m22 * m30 - m20 * m32) +
				m10 * (m23 * m32 - m22 * m33));
		out(1, 1) = d * (m22 * (m00 * m33 - m03 * m30) +
				m23 * (m02 * m30 - m00 * m32) +
				m20 * (m03 * m32 - m02 * m33));
		out(1, 2) = d * (m32 * (m00 * m13 - m03 * m10) +
				m33 * (m02 * m10 - m00 * m12) +
				m30 * (m03 * m12 - m02 * m13));
		out(1, 3) = d * (m02 * (m13 * m20 - m10 * m23) +
				m03 * (m10 * m22 - m12 * m20) +
				m00 * (m12 * m23 - m13 * m22));
		out(2, 0) = d * (m13 * (m20 * m31 - m21 * m30) +
				m10 * (m21 * m33 - m23 * m31) +
				m11 * (m23 * m30 - m20 * m33));
		out(2, 1) = d * (m23 * (m00 * m31 - m01 * m30) +
				m20 * (m01 * m33 - m03 * m31) +
				m21 * (m03 * m30 - m00 * m33));
		out(2, 2) = d * (m33 * (m00 * m11 - m01 * m10) +
				m30 * (m01 * m13 - m03 * m11) +
				m31 * (m03 * m10 - m00 * m13));
		out(2, 3) = d * (m03 * (m11 * m20 - m10 * m21) +
				m00 * (m13 * m21 - m11 * m23) +
				m01 * (m10 * m23 - m13 * m20));
		out(3, 0) = d * (m10 * (m22 * m31 - m21 * m32) +
				m11 * (m20 * m32 - m22 * m30) +
				m12 * (m21 * m30 - m20 * m31));
		out(3, 1) = d * (m20 * (m02 * m31 - m01 * m32) +
				m21 * (m00 * m32 - m02 * m30) +
				m22 * (m01 * m30 - m00 * m31));
		out(3, 2) = d * (m30 * (m02 * m11 - m01 * m12) +
				m31 * (m00 * m12 - m02 * m10) +
				m32 * (m01 * m10 - m00 * m11));
		out(3, 3) = d * (m00 * (m11 * m22 - m12 * m21) +
				m01 * (m12 * m20 - m10 * m22) +
				m02 * (m10 * m21 - m11 * m20));
		*this=out;
		return true;
	};

	inline Matrix44T<F> getInverted()
	{
		Matrix44T<F> out=*this;
		out.invert();
		return out;
	};

	inline void transpose()
	{
		Matrix44T<F> o;
		Matrix44T<F>& M=*this;

		o[ 0] = M[ 0];
		o[ 1] = M[ 4];
		o[ 2] = M[ 8];
		o[ 3] = M[12];

		o[ 4] = M[ 1];
		o[ 5] = M[ 5];
		o[ 6] = M[ 9];
		o[ 7] = M[13];

		o[ 8] = M[ 2];
		o[ 9] = M[ 6];
		o[10] = M[10];
		o[11] = M[14];

		o[12] = M[ 3];
		o[13] = M[ 7];
		o[14] = M[11];
		o[15] = M[15];
		*this=o;
	};

	inline Matrix44T<F> getTransposed()
	{
		Matrix44T<F> out=*this;
		out.transpose();
		return out;
	};

	template <class T>
	inline Vec3T<T> transformVect( Vec3T<T>& vect) const
	{
		f32 vector[3];

		vector[0] = vect.x*m00 + vect.y*m10 + vect.z*m20 + m30;
		vector[1] = vect.x*m01 + vect.y*m11 + vect.z*m21 + m31;
		vector[2] = vect.x*m02 + vect.y*m12 + vect.z*m22 + m32;

		vect.x = vector[0];
		vect.y = vector[1];
		vect.z = vector[2];
		return vect;
	};

	template <class T>
	inline Vec4T<T> transformVect( Vec4T<T>& vect) const
	{
		f32 vector[4];

		vector[0] = vect.x*m00 + vect.y*m10 + vect.z*m20 + m30;
		vector[1] = vect.x*m01 + vect.y*m11 + vect.z*m21 + m31;
		vector[2] = vect.x*m02 + vect.y*m12 + vect.z*m22 + m32;
		vector[3] = vect.x*m03 + vect.y*m13 + vect.z*m23 + m33;

		vect.x = vector[0];
		vect.y = vector[1];
		vect.z = vector[2];
		vect.w = vector[3];
		return vect;
	}

	inline void transformBBox(AABB& box)
	{
		Matrix44T<F> &m = *this;
		f32 Amin[3] = {box.min.x, box.min.y, box.min.z};
		f32 Amax[3] = {box.max.x, box.max.y, box.max.z};

		f32 Bmin[3];
		f32 Bmax[3];

		Bmin[0] = Bmax[0] = m[12];
		Bmin[1] = Bmax[1] = m[13];
		Bmin[2] = Bmax[2] = m[14];

		

		for (uint32 i = 0; i < 3; ++i)
		{
			for (uint32 j = 0; j < 3; ++j)
			{
				f32 a = m(j,i) * Amin[j];
				f32 b = m(j,i) * Amax[j];

				if (a < b)
				{
					Bmin[i] += a;
					Bmax[i] += b;
				}
				else
				{
					Bmin[i] += b;
					Bmax[i] += a;
				}
			}
		}

		box.min.x = Bmin[0];
		box.min.y = Bmin[1];
		box.min.z = Bmin[2];

		box.max.x = Bmax[0];
		box.max.y = Bmax[1];
		box.max.z = Bmax[2];
	}

	inline void transformBBox2(AABB& bbox) 
	{

		if (bbox.isReset())
			bbox.reset();
		else
		{
			Matrix44 m_size;
			m_size.m00=fabs(m00); m_size.m01=fabs(m01);	m_size.m02=fabs(m02); 
			m_size.m10=fabs(m10); m_size.m11=fabs(m11);	m_size.m12=fabs(m12);
			m_size.m20=fabs(m20); m_size.m21=fabs(m21);	m_size.m22=fabs(m22);

			Vec3 sz		=	m_size*((bbox.max-bbox.min)*0.5f);
			Vec3 pos	= (*this)*((bbox.max+bbox.min)*0.5f);
			bbox.min = pos-sz;	bbox.max = pos+sz;
		}
	}
	/////////////END OTHERS//////////////////////////////
};

typedef Matrix44T<f32> Matrix44;
typedef Matrix44T<f64> Matrix44r;

//post-multiply
template<class F1, class F2>
inline Vec4T<F1> operator*(const Matrix44T<F2> &m, const Vec4T<F1> &v) 
{
	return Vec4T<F1>(v.x*m.m00 + v.y*m.m01 + v.z*m.m02 + v.w*m.m03,
                      v.x*m.m10 + v.y*m.m11 + v.z*m.m12 + v.w*m.m13,
                      v.x*m.m20 + v.y*m.m21 + v.z*m.m22 + v.w*m.m23,
                      v.x*m.m30 + v.y*m.m31 + v.z*m.m32 + v.w*m.m33);
}

//pre-multiply
template<class F1, class F2>
inline Vec4T<F1> operator*(const Vec4T<F1> &v, const Matrix44T<F2> &m) 
{
	return Vec4T<F1>(v.x*m.m00 + v.y*m.m10 + v.z*m.m20 + v.w*m.m30,
                      v.x*m.m01 + v.y*m.m11 + v.z*m.m21 + v.w*m.m31,
                      v.x*m.m02 + v.y*m.m12 + v.z*m.m22 + v.w*m.m32,
                      v.x*m.m03 + v.y*m.m13 + v.z*m.m23 + v.w*m.m33);
}

//post-multiply
template<class F1, class F2>
inline Vec3T<F1> operator*(const Matrix44T<F2> &m, const Vec3T<F1> &v) 
{
	return Vec3T<F1>(v.x*m.m00 + v.y*m.m01 + v.z*m.m02,
                      v.x*m.m10 + v.y*m.m11 + v.z*m.m12,
                      v.x*m.m20 + v.y*m.m21 + v.z*m.m22);
}

//pre-multiply
template<class F1, class F2>
inline Vec3T<F1> operator*(const Vec3T<F1> &v, const Matrix44T<F2> &m) 
{
	return Vec3T<F1>(v.x*m.m00 + v.y*m.m10 + v.z*m.m20,
                      v.x*m.m01 + v.y*m.m11 + v.z*m.m21,
                      v.x*m.m02 + v.y*m.m12 + v.z*m.m22);
}