#pragma once
class GCamera : public GSceneNode
{
public:
	GCamera(GScene* pScene);
	GCamera(GScene* pScene,float Width_, float Height_, float Fov=(75.0f*(FL_PI/180.0f)), float Near=1, float Far=10000, float Aspect=4.f/3.f, Vec3 UpVec=Vec3(0,1,0));
	~GCamera(void);

	Matrix44& getViewMatrix() {return viewMatrix;};
	Matrix44& getProjectionMatrix() {return projectionMatrix;};

	void setFov(float val) {fFov=val;};
	void setNear(float val) {fNear=val;};
	void setFar(float val) {fFar=val;};
	void setAspectRatio(float val) {fAspectRatio=val;};
	void setUpVector(Vec3 up) {vUp=up;};
	void setDistance(float Dist) {distance=Dist;};

	float getWidth() {return fWidth;};
	float getHeight() {return fHeight;};
	float getFov() {return fFov;};
	float getNear() {return fNear;};
	float getFar() {return fFar;};
	float getAspectRatio() {return fAspectRatio;};
	Vec3 getUpVector() {return vUp;};
	Ang3 getRotation() {return Vec3(0,0,0);};////////////////
	Vec3 getTarget() {return vTarget;};
	Vec3 getLookPos() {return vLookPos;};
	float getDistance() {return distance;};

	GFrustum& getFrustum() {return frustum;};
	void updateFrustum();
	void recalcFrustum();
	void updateCamera();
	void updateNode() {updateCamera();};

	void setRotationAroundTarget(bool bVal) {bRotationAroundTarget=bVal;};

	void setLookAt(Vec3 target, float distance, Vec2 yaw_pitch);


	///////
	AABB getBoundingBox() {return frustum.getBoundingBox();};
	void drawNode();

	void setDrawFrustum(bool bVal) {bDrawFrustum=bVal;};
	bool isDrawFrustumEnabled() {return bDrawFrustum;};

private:
	void recalcViewMatrix();
	void recalcProjectionMatrix();

	bool bDrawFrustum;

	Matrix44 viewMatrix;
	Matrix44 projectionMatrix;

	float fWidth;
	float fHeight;
	float fFov;
	float fNear;
	float fFar;
	float fAspectRatio;
	bool bRotationAroundTarget;
	Vec3 vUp;
	float distance;
	Vec3 vTarget;
	Vec3 vLookPos;
	GFrustum frustum;
};

