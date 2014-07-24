#pragma once
class SceneNode
{
public:
	SceneNode();
	virtual ~SceneNode();

	void add(SceneNode* node);
	void remove(SceneNode* node);
	void removeFromParent();
	int find(SceneNode* node);

	virtual AABB getBoundingBox() {return AABB();};
	AABB getTransformedBoundingBox();
	AABB getAbsChildsBB();
	
	void updateAbsoluteTransformWithChilds();
	void updateAbsoluteTransform();
	Transform& getAbsoluteTransform();
	Transform& getTransform();

	void setScale(Vec3 val) {relScale=val;};
	Vec3& getScale() {return relScale;};

	UINT getChildCount() {return childs.size();};
	SceneNode* getChild(UINT index);
	
protected:
	SceneNode* parent;
	std::vector<SceneNode*> childs;

	Transform absTransform;
	Transform relTransform;
	Vec3 relScale;

	uint32 nodeID;

	bool bFlyNodeRotation;
};