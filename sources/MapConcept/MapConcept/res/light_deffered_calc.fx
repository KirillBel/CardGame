matrix gViewProjI : gDevViewProjI;
float3 cameraPos : gDevEyePos;
float2 screenSize : gDevViewportSize;

texture g_DiffuseTexture; 
texture g_NormalTexture; 
texture g_DepthTexture;
texture g_SSAOTexture;

sampler DiffuseMap = 
sampler_state
{
    Texture = < g_DiffuseTexture >;
	AddressU = CLAMP;
    AddressV = CLAMP;
    MagFilter = LINEAR;
    MinFilter = LINEAR;
    Mipfilter = LINEAR;
};

sampler NormalMap = 
sampler_state
{
    Texture = < g_NormalTexture >;
	AddressU = CLAMP;
    AddressV = CLAMP;
    MagFilter = LINEAR;
    MinFilter = LINEAR;
    Mipfilter = LINEAR;
};

sampler DepthMap = 
sampler_state
{
    Texture = < g_DepthTexture >;
	AddressU = CLAMP;
    AddressV = CLAMP;
    MagFilter = LINEAR;
    MinFilter = LINEAR;
    Mipfilter = LINEAR;
};

sampler SSAOMap = 
sampler_state
{
    Texture = < g_SSAOTexture >;
	AddressU = CLAMP;
    AddressV = CLAMP;
    MagFilter = LINEAR;
    MinFilter = LINEAR;
    Mipfilter = LINEAR;
};

struct VS_INPUT 
{
	float4 position	: POSITION0;
    float2 tex	: TEXCOORD0;
};

struct VS_OUTPUT 
{
	float4 position : POSITION0;
	float4 screenPosition		: TEXCOORD0;
};

VS_OUTPUT VShader( VS_INPUT input )
{
	VS_OUTPUT output=(VS_OUTPUT)0;
	output.position =input.position;
	output.screenPosition = output.position;
	return( output );
}

float3 lightPos=float3(5000,5000,-5000);

float4 PShader(VS_OUTPUT input) : COLOR0
{   
	input.screenPosition.xy /= input.screenPosition.w;
	float2 texCoord = 0.5f * (float2(input.screenPosition.x,-input.screenPosition.y) + 1);
	float2 halfPixel=float2(0.5/screenSize.x,0.5/screenSize.y);
	texCoord-=halfPixel;

	float ssao=tex2D(SSAOMap,texCoord);
	float4 Color=tex2D(DiffuseMap,texCoord);
	float4 Normal=tex2D(NormalMap,texCoord);
	Normal.xyz=2.0f*(Normal.xyz)-1.0f;
	Normal.xyz=normalize(Normal.xyz);
	
	float fGlow=Normal.a;
	float fSpecular=Color.a;
	Color.a=1;
	float Depth=tex2D(DepthMap,texCoord).x;
	
	float4 position=float4(input.screenPosition.xy,Depth,1);
	position = mul(position,gViewProjI);
	position /= position.w;
	
	// Lighting vectors
	float3 lVec;
	float3 lightVec;
	float3 viewVec;
	float3 reflectionVector;
	
	float specularLight=0;
	float lightIntensity=0;
	float3 currentLight=lightPos;

		lVec = (currentLight - position);
		lightVec = normalize(lVec);
		viewVec = normalize(position-cameraPos);
		reflectionVector = normalize(reflect(lightVec, Normal.xyz));
		
		specularLight += 0.5f*pow(max(dot(reflectionVector,viewVec),0.0),30.0);
		lightIntensity += max(0.7,dot(Normal.xyz, lightVec));
		//return float4(lightIntensity,lightIntensity,lightIntensity,1);
	
	lightIntensity=(saturate(lightIntensity*ssao));
	specularLight=saturate(specularLight);

	//return float4(Normal.xyz,1);
	float4 result=lightIntensity * Color+specularLight*fSpecular;//((lightIntensity * Color)+specularLight*fSpecular);
	//result=Color*ssao;//lerp( ssao, 1.0f, (Color.r+Color.g+Color.b)*0.333f );  
	
	//ssao=lerp( ssao, 1.0f, (result.r+result.g+result.b)*0.333f );
	//return float4(ssao,ssao,ssao,1);
	//return Normal;
	//return float4(lightIntensity,0,0,1);
	//return Color;//float4(Depth,0,0,1);
	result.a=1;
	return  result;

}

technique Main
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 VShader();
      PixelShader = compile ps_3_0 PShader();
   }

}