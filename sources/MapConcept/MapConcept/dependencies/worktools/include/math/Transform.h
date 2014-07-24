#pragma once

class Transform
{
public:
	Transform(): q(0, 0, 0, 1), p(),s(1,1,1)
	{
	}

	Transform(const Vec3& position): q(0, 0, 0, 1), p(position),s(1,1,1)
	{
	}

	Transform(const Quat& orientation): q(orientation), p(0, 0, 0),s(1,1,1)
	{
		_ASSERT(q.isSane());
	};

	Transform(const Vec3& p0, const Quat& q0): q(q0), p(p0) ,s(1,1,1)
	{
		_ASSERT(q.isSane());
	}

	/*Transform getInverse() const
	{
		_ASSERT(isFinite());
		return Transform(q.rotateInv(-p),q.getInverted());
	}*/


	Vec3 transform(const Vec3& input) const
	{
		_ASSERT(isFinite());
		return q.transform(input) + p;
	}

	/*Vec3 transformInv(const Vec3& input) const
	{
		_ASSERT(isFinite());
		return q.rotateInv(input-p);
	}*/

	Vec3 rotate(const Vec3& input) const
	{
		_ASSERT(isFinite());
		return q.transform(input);
	}

	/*Vec3 rotateInv(const Vec3& input) const
	{
		_ASSERT(isFinite());
		return q.rotateInv(input);
	}*/

	Transform transform(const Transform& src) const
	{
		_ASSERT(src.isSane());
		_ASSERT(isSane());
		// src = [srct, srcr] -> [r*srct + t, r*srcr]
		return Transform(q.transform(src.p) + p, q*src.q);
	};

	Transform transformInv(const Transform& src) const
	{
		_ASSERT(src.isSane());
		_ASSERT(isFinite());
		// src = [srct, srcr] -> [r^-1*(srct-t), r^-1*srcr]
		Quat qinv = q.getInverted();
		return Transform(qinv.transform(src.p - p), qinv*src.q);
	}

	Matrix44 getMatrix() const 
	{
		Matrix44 mat=Matrix44::CreateRotationQuat(q.v.x,q.v.y,q.v.z,q.w);
		//mat*=Matrix44::createScale(s);
		mat.setTranslation(p);
		mat=Matrix44::CreateScale(s)*mat;
		
		return mat;

		//return Matrix44::createTranslation(p)*Matrix44::createRotationQuat(q.v.x,q.v.y,q.v.z,q.w)*Matrix44::createScale(s);
		//return Matrix44::createScale(s)*Matrix44::createRotationQuat(q.v.x,q.v.y,q.v.z,q.w)*Matrix44::createTranslation(p);
	};

	inline void transformBBox(AABB& box)
	{
		getMatrix().transformBBox(box);
	};

	bool isSane() const
	{
	      return isFinite() && q.isSane();
	}

	bool isFinite() const 
	{ 
		return matkit::isFinite(p.x) && matkit::isFinite(p.y) && matkit::isFinite(p.z) && q.isFinite(); 
	};

	void setRotation(Matrix44& mat) 
	{
		q.set(mat);
	};

	void setTransform(Matrix44& mat) 
	{
		p=mat.getTranslation();
		q.set(mat);
		s=mat.getScale();
	};

	void strafe(float val){p+=q.getRightVector()*val;};
	void fly(float val){p+=q.getUpVector()*val;};
	void walk(float val){p+=q.getForwardVector()*val;};
	void setPosition(Vec3 val) {p=val;};
	void move(Vec3 val) {p+=val;};
public:
	Vec3 s;
	Vec3 p;
	Quat q;
};