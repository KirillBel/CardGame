#include "stdinclude.h"
#include "WorldMap.h"


WorldMap::WorldMap(void)
{
}


WorldMap::~WorldMap(void)
{
}

bool WorldMap::load(GTexture* pHM)
{
	int pitch;
	float hex_size=5;
	UINT* bytes=(UINT*)pHM->Lock2D(0,pitch,LOCK_READONLY);
	for(int i=0;i<pHM->getWidth();i++)
	{
		for(int j=0;j<pHM->getHeight();j++)
		{
			ColorB color(bytes[i+j*pHM->getWidth()]);
			float height=float(color.r+color.g+color.b)/3;
			height/=255.0;

			int q = ((1.0/3.0) * sqrt(3.0) * float(i) - (1.0/3.0) * float(j)) / hex_size;
			int r = ((2.0/3.0) * float(j)) / hex_size;

			auto it=hexmap.get(Vec2i(q,r));
			if(it==hexmap.end())
				it=hexmap.insert(HexData(),Vec2i(q,r));

			(*it).average+=height;
			(*it).count++;

		};
	};
	pHM->Unlock(0);
	return true;
};

void WorldMap::generate_geometry()
{
	pGeometry=new GGeometryBufferElem();

	
	int tris_count_per_bar=3*6;
	int vertex_per_bar=tris_count_per_bar*3;
	int vertex_count=hexmap.size()*vertex_per_bar;
	int tris_count=hexmap.size()*tris_count_per_bar;
	
	
	pGeometry->createVB(0,MapVertex::GetVD(),vertex_count,BUFTYP_STATIC,true);

	GVertexBuffer* pVB=pGeometry->getVB(0);
	MapVertex* pVertices=(MapVertex*)pVB->Lock(0,vertex_count*sizeof(MapVertex),0);
	pGeometry->setNumPrimitives(tris_count);
	pGeometry->setPrimitiveType(ePrimitiveType::PRIM_TRIANGLELIST);

	int cnt=0;
	for(int i=0;i<hexmap.size();i++)
	{
		Vec2i coord=hexmap.coord(i);
		Vec2 off_vec1(1,0);
		Vec2 off_vec2(1,0);
		off_vec1.rotate(DEG2RAD(30));
		off_vec2.rotate(DEG2RAD(-30));
		float radius=80;
		float inner_radius=sqrt(3.0)*radius/2.0;

		Vec2 offset=(off_vec1*coord.x+off_vec2*coord.y)*inner_radius*2;//(coord.x*170,coord.y*170);
		float height=hexmap[i].height ? hexmap[i].height : hexmap[i].average/hexmap[i].count;
		height*=1300;
		

		for(int a=0;a<360;a+=60)
		{
			Vec2 rot1(radius,0);
			Vec2 rot2(radius,0);
			Vec2 n(1,0);
			Vec2 n1(1,0);
			Vec2 n2(1,0);
			
			rot1.rotate(DEG2RAD(a));
			rot2.rotate(DEG2RAD(a+60));
			n.rotate(DEG2RAD(a+30));
			n1.rotate(DEG2RAD(a));
			n2.rotate(DEG2RAD(a+60));
			Vec3 norm(n.x,0,n.y);
			float tex_x=float(a)/360.f;
			float tex_xpp=float(a+60)/360.f;
			float len=radius*6;
			float rand_offset=matkit::random32f();
			float tex_y=0+rand_offset;
			float tex_ypp=height/len+rand_offset;

			pVertices[cnt]=MapVertex(Vec3(offset.x,height,offset.y),Vec2(0.5,0.5),Vec3(0,1,0));
			pVertices[cnt+1]=MapVertex(Vec3(rot2.x+offset.x,height,rot2.y+offset.y),n1/2+0.5,Vec3(0,1,0));
			pVertices[cnt+2]=MapVertex(Vec3(rot1.x+offset.x,height,rot1.y+offset.y),n2/2+0.5,Vec3(0,1,0));
			

			pVertices[cnt+3]=MapVertex(Vec3(rot1.x+offset.x,height,rot1.y+offset.y),Vec2(tex_x,tex_y),norm);
			pVertices[cnt+4]=MapVertex(Vec3(rot2.x+offset.x,height,rot2.y+offset.y),Vec2(tex_xpp,tex_y),norm);
			pVertices[cnt+5]=MapVertex(Vec3(rot1.x+offset.x,0,rot1.y+offset.y),Vec2(tex_x,tex_ypp),norm);

			pVertices[cnt+6]=MapVertex(Vec3(rot2.x+offset.x,height,rot2.y+offset.y),Vec2(tex_xpp,tex_y),norm);
			pVertices[cnt+7]=MapVertex(Vec3(rot2.x+offset.x,0,rot2.y+offset.y),Vec2(tex_xpp,tex_ypp),norm);
			pVertices[cnt+8]=MapVertex(Vec3(rot1.x+offset.x,0,rot1.y+offset.y),Vec2(tex_x,tex_ypp),norm);
			cnt+=9;
		};
	};
	pVB->Unlock();

	pScreenShader=GShader::LoadShader("res/deferred_lighting.fx");
	pSSAOShader=GShader::LoadShader("res/SSAO2.fx");
	
	GTexture* pHexDiffuse=GTexture::LoadTexture2D("res/wall2.jpg",8);
	GTexture* pHexNormal=GTexture::LoadTexture2D("res/wall_n.tga",8);
	
	sShaderResources res;
	res.params.setValue("texDiffuse",pHexDiffuse);
	res.params.setValue("texNormal",pHexNormal);
	res.bUpdateCacheBlock=true;
	pScreenShader->applyShaderResources(&res);
	


	pScreenColor=GTexture::CreateColorTarget("$screen_color",1366,768);
	pScreenNormal=GTexture::CreateColorTarget("$screen_normal",1366,768,TEXFMT_A8R8G8B8);
	pScreenDepth=GTexture::CreateColorTarget("$screen_depth",1366,768,TEXFMT_R32F);
	pScreenSSAO=GTexture::CreateColorTarget("$screen_ssao",1366,768);
	pTempTex=GTexture::CreateColorTarget("$screen_temp",1366,768);


	pSSAOrnm=GTexture::LoadTexture2D("res/noise.png",1);

	pGaussShader=GShader::LoadShader("res/gauss.fx");
	gaussApp.SetShader(pGaussShader);
	
	pFinalShader=GShader::LoadShader("res/light_deffered_calc.fx");
	sShaderResources res1;
	res1.params.setValue("g_DiffuseTexture",pScreenColor);
	res1.params.setValue("g_NormalTexture",pScreenNormal);
	res1.params.setValue("g_DepthTexture",pScreenDepth);
	res1.params.setValue("g_SSAOTexture",pScreenSSAO);
	res1.bUpdateCacheBlock=true;
	pFinalShader->applyShaderResources(&res1);
};

void WorldMap::draw()
{
	Get3dDevice()->setWorldMatrix(Matrix44::getIdentity());
	Get3dDevice()->setCullMode(CULL_BACK);

	Get3dDevice()->pushRenderTarget(0,pScreenColor);
	Get3dDevice()->pushRenderTarget(1,pScreenNormal);
	Get3dDevice()->pushRenderTarget(2,pScreenDepth);
	Get3dDevice()->clear(CLEAR_COLOR);

	UINT nPasses;
	pScreenShader->applyShaderSemantics();
	pScreenShader->begin(&nPasses);
	pScreenShader->beginPass(0);

	pGeometry->elPreDraw(0);
	pGeometry->elDraw(0,0);
	pGeometry->elPostDraw(0);

	pScreenShader->endPass();
	pScreenShader->end();

	Get3dDevice()->popRenderTarget(0);
	Get3dDevice()->popRenderTarget(1);
	Get3dDevice()->popRenderTarget(2);

	Get3dDevice()->pushRenderTarget(0,pScreenSSAO);
	Get3dDevice()->clear(CLEAR_COLOR);

	sShaderResources res;
	res.params.setValue("randNormalTex",pSSAOrnm);
	res.params.setValue("NormalTex",pScreenNormal);
	res.params.setValue("DepthTex",pScreenDepth);
	res.bUpdateCacheBlock=true;
	pSSAOShader->applyShaderResources(&res);

	pSSAOShader->begin(&nPasses);
	pSSAOShader->beginPass(0);
	Get3dDevice()->drawScreenQuad();
	pSSAOShader->endPass();
	pSSAOShader->end();

	Get3dDevice()->popRenderTarget(0);

	
	
	pGaussShader->begin(&nPasses);
	
	Get3dDevice()->pushRenderTarget(0,pTempTex);
	gaussApp.GetShaderParams(true).setValue("g_DownsampleTexture",pScreenSSAO);
	gaussApp.apply();
	pGaussShader->beginPass(0);
	Get3dDevice()->drawScreenQuad();
	pGaussShader->endPass();
	Get3dDevice()->popRenderTarget(0);

	Get3dDevice()->pushRenderTarget(0,pScreenSSAO);
	gaussApp.GetShaderParams(true).setValue("g_DownsampleTexture",pTempTex);
	gaussApp.apply();
	pGaussShader->beginPass(1);
	Get3dDevice()->drawScreenQuad();
	pGaussShader->endPass();
	Get3dDevice()->popRenderTarget(0);

	pGaussShader->end();

	
	pFinalShader->begin(&nPasses);
	pFinalShader->beginPass(0);
	Get3dDevice()->drawScreenQuad();
	pFinalShader->endPass();
	pFinalShader->end();

	/*Get3dDevice()->setTexture(0,pScreenSSAO);
	Get3dDevice()->drawScreenQuad();*/
};