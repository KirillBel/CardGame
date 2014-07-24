#pragma once

typedef std::vector<GSceneNode*> SceneArray;
typedef std::list<GSceneNode*> DrawArray;

class GScene
{
public:
	GScene(void);
	~GScene(void);

	//GMesh* createMesh();
	//GMesh* createMesh(const char * path);
	GSkybox* createSkybox(IMaterial* pMat);

	GSceneNode* createNode(IRenderNode* pNode);
	void addNode(GSceneNode* pNode);
	//GSceneNode* createLight(sLightDecl& lightDecl);
	GCamera* createCamera();
	GCamera* createCamera(float Width, float Height,float Fov=(75.0f*(FL_PI/180.0f)), float Near=1, float Far=10000, float Aspect=4.f/3.f, Vec3 UpVec=Vec3(0,1,0));

	//GTerrain* createTerrain(GTexture* heightmap,GTexture* color, float size, float height, const char* terrainDrawShader,const char* normalGenShader, int nodeWidth=128);

	void setActiveCamera(GCamera* pCamera)
	{
		pActiveCamera=pCamera;
	};
	GCamera* getActiveCamera() {return pActiveCamera;};

	void draw();

	void release();

protected:
	void prepare_objects();
private:
	SceneArray preparedObjects;
	SceneArray objects;
	SceneArray lights;
	DrawArray drawArray;

	GCamera* pActiveCamera;
};

