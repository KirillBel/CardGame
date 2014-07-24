#include "stdinclude.h"

GFramework framework;
DWORD prevCameraProc=GetTickCount();
GCamera* pCamera;
GScene scene;

bool mouseFunc(int x, int y, int dx, int dy);
bool buttonFunc(unsigned char keyCode, bool bVal);
void keyProcess();


bool bPause=false;
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	HTMLLogger* pLogger=new HTMLLogger();
	Logger::DebugTraceLogger* pTraceLogger=new Logger::DebugTraceLogger();
	pLogger->Init("LOG.html","Global log");
	Logger::AttachOverallListener(pLogger);
	Logger::AttachOverallListener(pTraceLogger);

	framework.CreateFramework(1366,768,NULL,true,"GEngine",0);
	framework.GetInput().addMouseMoveCallback(&mouseFunc);
	framework.GetInput().addButtonCallback(&buttonFunc);

	Get3dEngine()->Create(1366,768,framework.GetWindow(),true);

	Get3dDevice()->setProjectionMatrix(Matrix44::CreatePerspectiveFovLH((75.0f*(FL_PI/180.0f)),1366/768.f,1,1000));
	Get3dDevice()->getDefaultRenderStates().CullMode=CULL_DISABLE;

	pCamera=scene.createCamera();
	pCamera->getTransform().p=Vec3(0,0,-70);
	scene.setActiveCamera(pCamera);

	GTexture* pHM=GTexture::LoadTexture2D("res/hm3.png");
	WorldMap wmap;
	wmap.load(pHM);
	wmap.generate_geometry();

	/*bytes=(UINT*)pHM->Lock2D(0,pitch,0);
	for(int i=0;i<pHM->getWidth();i++)
	{
		for(int j=0;j<pHM->getHeight();j++)
		{
			ColorB color(bytes[i+j*pHM->getWidth()]);

			int q = ((1.0/3.0) * sqrt(3.0) * float(i) - (1.0/3.0) * float(j)) / hex_size;
			int r = ((2.0/3.0) * float(j)) / hex_size;

			auto it=hexmap.get(Vec2i(q,r));
			if(it==hexmap.end())
			{
				_ASSERT(false);
			};

			if((*it).height==0)
				(*it).height=(*it).average/(*it).count;
			color.r=color.g=color.b=(*it).height*255;
			color.a=255;
			bytes[i+j*pHM->getWidth()]=color.pack_argb8888();
		};
	};
	pHM->Unlock(0);

	pHM->Save("res/temp.png");*/

	while(framework.Frame())
	{
		if(bPause)
		{
			keyProcess();
			Sleep(30);
			continue;
		};

		Get3dDevice()->setClearColor(ColorF(0.5,0.5,0.5,1));
		Get3dDevice()->beginFrame(Render::CLEAR_STD);
		
		scene.draw();
		wmap.draw();

		Get3dRenderer()->GetDynamicGeomBuffer().Flush();

		Get3dDevice()->drawText(strkit::format("FPS: %.2f, %.1f ms/frame, DIPs: %u",
			Get3dDevice()->getFPS(),
			1000.0/Get3dDevice()->getFPS(),
			Get3dDevice()->getDIPCount()).c_str(),Vec2i(10,10),ColorF(1,0,0,1));
		Get3dDevice()->drawText(strkit::format("POS: %.2f, %.2f, %.2f",pCamera->getTransform().p.x,pCamera->getTransform().p.y,pCamera->getTransform().p.z).c_str(),
			Vec2i(10,30),ColorF(1,0,0,1));

		std::string load_path=Get3dEngine()->GetLoader()->GetCurLoadPath();
		if(!load_path.empty())
			Get3dDevice()->drawText(strkit::format("Loading: %s",load_path.c_str()).c_str(),Vec2i(10.f,Get3dDevice()->getFrameSize().y-40.f),ColorF(0,1,0,1));
		Get3dDevice()->endFrame();
		keyProcess();

		Get3dRenderer()->ClearBuffers();
	};
	

	Get3dEngine()->Release();
	framework.Release();

	//Profiler::Instance().LogCallStack();

	Logger::DetachListener(pLogger);
	Logger::DetachListener(pTraceLogger);
	pLogger->Release();
	delete pLogger;
	delete pTraceLogger;
	return 0;
};



bool mouseFunc(int x, int y, int dx, int dy)
{
	if(framework.GetInput().GetMouseKeyState(DIM_RBUTTON))
	{
		//pCamera->getTransform().q.rotateYaw(-float(dx)/100.f);
		pCamera->getTransform().q.rotateAA(float(dx)/100.f, Vec3(0,1,0));
		pCamera->getTransform().q.rotatePitch(float(dy)/100.f);
	};
	return true;
};

bool buttonFunc(unsigned char keyCode, bool bVal)
{
	if((keyCode==DIK_H) && (bVal))
	{
		bPause=!bPause;
	};

	if((keyCode==DIK_B) && (bVal))
	{
		eFillMode mode=Get3dDevice()->getDefaultRenderStates().FillMode;
		Get3dDevice()->getDefaultRenderStates().FillMode= (mode==FILL_SOLID) ? FILL_WIREFRAME : FILL_SOLID;
	};

	if((keyCode==DIK_P) && (bVal))
	{
		GResource::ReloadClass(GResource::GetRCNameHash("GShader"));
	}

	if((keyCode==DIK_O) && (bVal))
	{
		//Profiler::Instance().LogCallStack();
	};

	if((keyCode==DIK_L) && (bVal))
	{
		Get3dRenderer()->EnableSorting(!Get3dRenderer()->isSortingEnabled());
	}
	return true;
};

void keyProcess()
{
	DWORD timeLeft=GetTickCount()-prevCameraProc;

	float MovSense=200;
	if(framework.GetInput().GetKeyState(DIK_LSHIFT))
	{
		MovSense=MovSense*5;
	}

	float time=float(timeLeft)/1000.0f;
	MovSense*=time;
	float rot_sense=1.f*time;

	if(framework.GetInput().GetKeyState(DIK_A))
	{
		pCamera->getTransform().strafe(-MovSense);
	}
	if(framework.GetInput().GetKeyState(DIK_D))
	{
		pCamera->getTransform().strafe(MovSense);
	}
	if(framework.GetInput().GetKeyState(DIK_W))
	{
		pCamera->getTransform().walk(MovSense);
	}
	if(framework.GetInput().GetKeyState(DIK_S))
	{
		pCamera->getTransform().walk(-MovSense);
	}
	if(framework.GetInput().GetKeyState(DIK_Q))
	{
		pCamera->getTransform().q.rotateRoll(-rot_sense);
	}
	if(framework.GetInput().GetKeyState(DIK_E))
	{
		pCamera->getTransform().q.rotateRoll(rot_sense);
	}

	prevCameraProc=GetTickCount();
};