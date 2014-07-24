#pragma once
class GScene;
class GSceneNode : public SceneNode
{
public:
	GSceneNode(GScene* pScene,IRenderNode* pNode)
	{
		this->pNode=pNode;
		bDrawBBox=false;
		bEnableDraw=true;
	};

	virtual ~GSceneNode() {};

	///////////DRAW//////////////////
	void draw();
	void update();

	virtual void drawNode();
	virtual void updateNode() {};
	virtual AABB getBoundingBox() {return pNode->GetBBox();};

	void setRenderNode(IRenderNode* pNode) {this->pNode=pNode;};
	IRenderNode* getRenderNode() {return pNode;};

	void setDrawBoundingBox(bool bVal) {bDrawBBox=bVal;};
	bool isDrawBoundingBoxEnabled() {return bDrawBBox;};
	void setEnableDraw(bool bVal) {bEnableDraw=bVal;};
	bool isDrawEnabled() {return bEnableDraw;};

protected:
	bool bDrawBBox;
	bool bEnableDraw;
	IRenderNode* pNode;
	GRenderObject renderObject;
	GScene* pScene;
};

