#pragma once

template <class F> 
class QuatT
{
public:
	inline QuatT() {w=0;}

	inline QuatT<F>( F X, F Y, F Z, F W,  bool bNorm = false ) 
		: w(W),v(X,Y,Z)	
	{
		if (bNorm) 
			normalize();
		//assert(IsValid()); 
	};

	QuatT(const QuatT<F>& q): v(q.v), w(q.w) {}

	inline QuatT<F>( const Vec3T<F> &axis, F angle ) : w(angle),v(axis) {};

	inline QuatT<F>(const Matrix44T<F>& m)
	{
		(*this)=m;
	};

	inline QuatT<F>( const Ang3T<F>& angles)
	{
		setRotationXYZ(angles);
	};

	//////////////////////////////////

	inline static QuatT<F> CreateAngleAxis(f32 angle, const Vec3T<F>& axis)
	{
		const f32 fHalfAngle = 0.5f*angle;
		const f32 fSin = sinf(fHalfAngle);
		QuatT<F> q;
		q.w = cosf(fHalfAngle);
		q.v.x = fSin*axis.x;
		q.v.y = fSin*axis.y;
		q.v.z = fSin*axis.z;
		return q;
	};

	inline static QuatT<F> CreateRotationXYZ(const Ang3T<F>& angles)
	{
		QuatT<F> q;
		q.setRotationXYZ(angles);
		return q;
	}

	inline static QuatT<F> CreateIdentity()
	{
		QuatT<F> q;
		q.w = 1.f;
		q.v.set(0,0,0);
		return q;
	}

	inline static QuatT<F> CreateLookAt(Vec3T<F> forwardVector)
	{
		float dot = Vec3T<F>(0,0,1).dot(forwardVector);

		if (abs(dot - (-1.0f)) < 0.000001f)
		{
			return QuatT<F>(0, 1, 0, 3.1415926535897932f);
		}
		if (abs(dot - (1.0f)) < 0.000001f)
		{
			return QuatT<F>::CreateIdentity();
		}

		float rotAngle = (float)acos(dot);
		Vec3T<F> rotAxis = Vec3T<F>(0,0,1).cross(forwardVector).getNormalized();
		return QuatT<F>::CreateAngleAxis(rotAngle,rotAxis);
	};

	inline static QuatT<F> CreateLookAt(Vec3T<F> sourcePoint, Vec3T<F> destPoint)
	{
		Vec3T<F> forwardVector = (destPoint - sourcePoint).getNormalized();

		return CreateLookAt(forwardVector);
	}

	///////////OPERATORS//////////////

	bool operator==(const QuatT<F>& other) {return equals(other);};
	bool operator!=(const QuatT<F>& other) {return !(this->operator==(other));};

	inline QuatT<F>& operator=(const QuatT<F>& other) {v=other.v; w=other.w; return *this;};

	inline QuatT<F>& operator=(const Matrix44T<F>& m)
	{
		const f32 diag = m(0,0) + m(1,1) + m(2,2) + 1;

		if( diag > 0.0f )
		{
			const f32 scale = sqrtf(diag) * 2.0f; // get scale from diagonal

			// TODO: speed this up
			v.x = ( m(2,1) - m(1,2)) / scale;
			v.y = ( m(0,2) - m(2,0)) / scale;
			v.z = ( m(1,0) - m(0,1)) / scale;
			w = 0.25f * scale;
		}
		else
		{
			if ( m(0,0) > m(1,1) && m(0,0) > m(2,2))
			{
				// 1st element of diag is greatest value
				// find scale according to 1st element, and double it
				const f32 scale = sqrtf( 1.0f + m(0,0) - m(1,1) - m(2,2)) * 2.0f;

				// TODO: speed this up
				v.x = 0.25f * scale;
				v.y = (m(0,1) + m(1,0)) / scale;
				v.z = (m(2,0) + m(0,2)) / scale;
				w = (m(2,1) - m(1,2)) / scale;
			}
			else if ( m(1,1) > m(2,2))
			{
				// 2nd element of diag is greatest value
				// find scale according to 2nd element, and double it
				const f32 scale = sqrtf( 1.0f + m(1,1) - m(0,0) - m(2,2)) * 2.0f;

				// TODO: speed this up
				v.x = (m(0,1) + m(1,0) ) / scale;
				v.y = 0.25f * scale;
				v.z = (m(1,2) + m(2,1) ) / scale;
				w = (m(0,2) - m(2,0) ) / scale;
			}
			else
			{
				// 3rd element of diag is greatest value
				// find scale according to 3rd element, and double it
				const f32 scale = sqrtf( 1.0f + m(2,2) - m(0,0) - m(1,1)) * 2.0f;

				// TODO: speed this up
				v.x = (m(0,2) + m(2,0)) / scale;
				v.y = (m(1,2) + m(2,1)) / scale;
				v.z = 0.25f * scale;
				w = (m(1,0) - m(0,1)) / scale;
			}
		}

		return normalize();
	}

	QuatT<F> operator+(const QuatT<F>& other) {return QuatT<F>(v+other.v,w+other.w);};
	QuatT<F>& operator+=(const QuatT<F>& other) {w+=other.w; v+=other.v; return *this;};

	QuatT<F> operator*(const QuatT<F>& other) const
	{
		QuatT<F> tmp;

		tmp.w = (other.w * w) - (other.v.x * v.x) - (other.v.y * v.y) - (other.v.z * v.z);
		tmp.v.x = (other.w * v.x) + (other.v.x * w) + (other.v.y * v.z) - (other.v.z * v.y);
		tmp.v.y = (other.w * v.y) + (other.v.y * w) + (other.v.z * v.x) - (other.v.x * v.z);
		tmp.v.z = (other.w * v.z) + (other.v.z * w) + (other.v.x * v.y) - (other.v.y * v.x);

		return tmp;
	};

	QuatT<F>& operator*=(const QuatT<F>& other) const {return (*this = other * (*this));};

	QuatT<F> operator*(F s) const {return quaternion(s*v.x, s*v.y, s*v.z, s*w);};
	QuatT<F>& operator*=(F s) const {v*=s;	w*=s; return *this;};
	
	Vec3T<F> transform(const Vec3T<F>& other) const
	{
		Vec3T<F> uv, uuv;
		Vec3T<F> qvec(v);
		uv = qvec.cross(other);
		uuv = qvec.cross(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return other + uv + uuv;
	};

	Vec3T<F> operator*(const Vec3T<F>& other) const
	{
		return transform(other);
	};

	inline F dot(const QuatT<F>& other) {return (v.x * other.v.x) + (v.y * other.v.y) + (v.z * other.v.z) + (w * other.w);};

	inline QuatT<F>& set(f32 x, f32 y, f32 z, f32 w) {v.Set(x,y,z); this->w=w;};
	inline QuatT<F>& set(const QuatT<F>& quat) {v=quat.v; w=quat.w;};
	inline QuatT<F>& set(const Matrix44T<F>& m) {(*this)=m; return *this;};

	inline bool equals(const QuatT<F>& other,
				const F tolerance = ROUNDING_ERROR_f32 ) const
	{
		return matkit::equals_tpl(v.x,other.v.x,tolerance) && matkit::equals_tpl(v.y,other.v.y,tolerance) 
				&& matkit::equals_tpl(v.z,other.v.z,tolerance) && matkit::equals_tpl(w,other.w,tolerance);
	};

	inline QuatT<F>& normalize()
	{
		F d = matkit::isqrt(w*w + v.x*v.x+v.y*v.y+v.z*v.z);
		w*=d;	v.x*=d; v.y*=d; v.z*=d;
		return *this;
	};

	inline QuatT<F>& getNormalized() const
	{
		QuatT<F> t=*this; 
		t.normalize(); 
		return t;	
	}

	QuatT<F>& makeInverse()
	{
		v.x = -v.x; v.y = -v.y; v.z = -v.z;
		return *this;
	};

	QuatT<F> getInverted() const
	{
		QuatT<F> q=*this;
		q.v.x = -v.x; q.v.y = -v.y; q.v.z = -v.z;
		return q;
	};

	QuatT<F>& makeIdentity()
	{
		*this=CreateIdentity();
		return *this;
	}

	QuatT<F>& slerp( QuatT<F> q1, QuatT<F> q2, f32 interpolate )
	{
		F angle = q1.dot(q2);

		if (angle < 0.0f)
		{
			q1 *= -1.0f;
			angle *= -1.0f;
		}

		f32 scale;
		f32 invscale;

		if ((angle + 1.0f) > 0.05f)
		{
			if ((1.0f - angle) >= 0.05f) // spherical interpolation
			{
				const f32 theta = acosf(angle);
				const f32 invsintheta = reciprocal(sinf(theta));
				scale = sinf(theta * (1.0f-time)) * invsintheta;
				invscale = sinf(theta * time) * invsintheta;
			}
			else // linear interploation
			{
				scale = 1.0f - time;
				invscale = time;
			}
		}
		else
		{
			q2.set(-q1.v.y, q1.v.x, -q1.w, q1.v.z);
			scale = sinf(PI * (0.5f - time));
			invscale = sinf(PI * time);
		}

		return (*this = (q1*scale) + (q2*invscale));
	};

	inline void toAngleAxis(f32 &angle, Vec3T<F> &axis) const
	{
		const f32 scale = v.GetLength();

		if (matkit::iszero(scale) || w > 1.0f || w < -1.0f)
		{
			angle = 0.0f;
			axis.x = 0.0f;
			axis.y = 1.0f;
			axis.z = 0.0f;
		}
		else
		{
			const f32 invscale = matkit::reciprocal(scale);
			angle = 2.0f * acosf(w);
			axis.x = v.x * invscale;
			axis.y = v.y * invscale;
			axis.z = v.z * invscale;
		}
	}

	inline void rotateAA(F angle,Vec3& axis)
	{
		*this = (*this)*CreateAngleAxis(angle,axis);
	};

	inline void rotateYPR(F yaw, F pitch, F roll)
	{
		QuatT<F> q=CreateAngleAxis(-yaw,getUpVector())*
						CreateAngleAxis(-pitch,getRightVector())*CreateAngleAxis(-roll,getForwardVector());
		*this = (*this)*q;
	};

	inline void rotateYaw(F val)
	{
		*this = (*this)*CreateAngleAxis(-val,getUpVector());
	};

	inline void rotatePitch(F val)
	{
		*this = (*this)*CreateAngleAxis(-val,getRightVector());
	};

	inline void rotateRoll(F val)
	{
		*this = (*this)*CreateAngleAxis(-val,getForwardVector());
	};

	inline Ang3T<F> getRotationXYZ() const
	{
		Ang3T<F> euler;
		const f64 sqw = w*w;
		const f64 sqx = v.x*v.x;
		const f64 sqy = v.y*v.y;
		const f64 sqz = v.z*v.z;

		// heading = rotation about z-axis
		euler.z = (f32) (atan2(2.0 * (v.x*v.y +v.z*w),(sqx - sqy - sqz + sqw)));

		// bank = rotation about x-axis
		euler.x = (f32) (atan2(2.0 * (v.y*v.z +v.x*w),(-sqx - sqy + sqz + sqw)));

		// attitude = rotation about y-axis
		euler.y = asinf( matkit::clamp(-2.0f * (v.x*v.z - v.y*w), -1.0f, 1.0f) );
		return euler;
	}

	inline QuatT<F>& setRotationXYZ(const Ang3T<F>& a)
	{
		f64 angle;

		angle = a.x * 0.5;
		const f64 sr = sin(angle);
		const f64 cr = cos(angle);

		angle = a.y * 0.5;
		const f64 sp = sin(angle);
		const f64 cp = cos(angle);

		angle = a.z * 0.5;
		const f64 sy = sin(angle);
		const f64 cy = cos(angle);

		const f64 cpcy = cp * cy;
		const f64 spcy = sp * cy;
		const f64 cpsy = cp * sy;
		const f64 spsy = sp * sy;

		v.x = (f32)(sr * cpcy - cr * spsy);
		v.y = (f32)(cr * spcy + sr * cpsy);
		v.z = (f32)(cr * cpsy - sr * spcy);
		w = (f32)(cr * cpcy + sr * spsy);

		return normalize();
	};

	inline QuatT<F>& rotationFromTo(const Vec3T<F>& from, const Vec3T<F>& to)
	{
		// Based on Stan Melax's article in Game Programming Gems
		// Copy, since cannot modify local
		Vec3T<F> v0 = from;
		Vec3T<F> v1 = to;
		v0.normalize();
		v1.normalize();

		const f32 d = v0.dot(v1);
		if (d >= 1.0f) // If dot == 1, vectors are the same
		{
			return makeIdentity();
		}
		else if (d <= -1.0f) // exactly opposite
		{
			Vec3T<F> axis(1.0f, 0.f, 0.f);
			axis = axis.cross(v);
			if (axis.getLength()==0)
			{
				axis.set(0.f,1.f,0.f);
				axis.cross(v);
			}
			*this=CreateAngleAxis(FL_PI, axis);
			return *this;
		}

		const f32 s = sqrtf( (1+d)*2 ); // optimize inv_sqrt
		const f32 invs = 1.f / s;
		const Vec3T<F> c = v0.cross(v1)*invs;
		v=c;
		w = s * 0.5f;

		return *this;
	};

	inline F getAngle(const QuatT<F>& q) const
	{
		return matkit::_acosf((float)dot(q)) * F(2);
	};

	inline F getLengthSquared() const
	{
		return v.x*v.x + v.y*v.y + v.z*v.z + w*w;
	};

	inline F getLength() const
	{
		return sqrt(getLengthSquared());
	};

	inline bool isSane() const
	{
	      const F unitTolerance = F(1e-2);
	      return isFinite() && fabs((float)getLength()-1)<unitTolerance;
	};

	bool isFinite() const
	{
		return matkit::isFinite(v.x) 
			&& matkit::isFinite(v.y) 
			&& matkit::isFinite(v.z)
			&& matkit::isFinite(w);
	}

	/** brief computes rotation of x-axis */
	Vec3 getBasisVector0()	const
	{	
		const F x2 = v.x*F(2);
		const F w2 = w*F(2);
		return Vec3(	(w * w2) - F(1) + v.x*x2,
						(v.z * w2)        + v.y*x2,
						(-v.y * w2)       + v.z*x2);
	}
	
	/** brief computes rotation of y-axis */
	Vec3 getBasisVector1()	const 
	{	
		const F y2 = v.y*F(2);
		const F w2 = w*F(2);
		return Vec3(	(-v.z * w2)       + v.x*y2,
						(w * w2) - 1.0f + v.y*y2,
						(v.x * w2)        + v.z*y2);
	}


	/** brief computes rotation of z-axis */
	Vec3 getBasisVector2() const	
	{	
		const F z2 = v.z*F(2);
		const F w2 = w*F(2);
		return Vec3(	(v.y * w2)        + v.x*z2,
						(-v.x * w2)       + v.y*z2,
						(w * w2) - 1.0f + v.z*z2);
	}

	Vec3 getForwardVector() const
	{
		return Vec3( 2 * (v.x * v.z + w * v.y), 
						2 * (v.y * v.z - w * v.x),
						1 - 2 * (v.x * v.x + v.y * v.y));
	}
 
	Vec3 getUpVector() const
	{
		return Vec3( 2 * (v.x * v.y - w * v.z), 
						1 - 2 * (v.x * v.x + v.z * v.z),
						2 * (v.y * v.z + w * v.x));
	}
 
	Vec3 getRightVector() const
	{
		return Vec3( 1 - 2 * (v.y * v.y + v.z * v.z),
						2 * (v.x * v.y + w * v.z),
						2 * (v.x * v.z - w * v.y));
	}

	Matrix44T<F> toMatrix()
	{
		return Matrix44T<F>::createRotationQuat(*this);
	};

public:
	Vec3T<F> v;	F w;
};

typedef QuatT<f32>	Quat;