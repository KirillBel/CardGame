matrix worldMatrix : gDevWorld;
matrix worldI : gDevWorldI;
matrix viewMatrix : gDevView;
matrix viewIMatrix : gDevViewI;
matrix viewProj	   : gDevViewProj;
matrix worldView	   : gDevWorldView;
matrix WVP : gDevWVP;

float3 gEyePos : gDevEyePos;
float3 lightPos=float3(0,600,0);

texture texDiffuse;
texture texSpecular;
texture texNormal;

float4 valSpecular;

float fGlow=0;

sampler DiffuseMap = 
sampler_state
{
    Texture = < texDiffuse >;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = WRAP;
    AddressV = WRAP;
};

sampler SpecularMap = 
sampler_state
{
    Texture = < texSpecular >;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = WRAP;
    AddressV = WRAP;
};

sampler NormalMap = 
sampler_state
{
    Texture = < texNormal >;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = WRAP;
    AddressV = WRAP;
};

struct DEF_INPUT 
{
	float4 position	: POSITION;
    float2 tex	: TEXCOORD0;
	float3 normal : NORMAL;
};

struct DEF_OUTPUT 
{
	float4 position : Position;
    float2 tex : TEXCOORD0;
	float4 s_pos : TEXCOORD1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
	float depth : TEXCOORD2;
};

DEF_OUTPUT calcDefOutput(DEF_INPUT input)
{
	DEF_OUTPUT output=(DEF_OUTPUT)0;
	
	output.position = mul(input.position,WVP);
	output.tex = input.tex;

	output.normal=input.normal;//mul(input.normal,WVP);
	
	output.tangent=float3(0,0,0);
	output.binormal=float3(0,0,0);
	// Pass tangent space
	//output.normal   = mul(worldI,input.normal);
	//output.normal	=mul(input.normal,WVP);
	//output.normal=normalize(output.normal);
	output.tangent = float3(abs(output.normal.z+output.normal.y), 0 , output.normal.x);
	output.binormal = cross(output.normal,output.tangent);
	
	float3 posWorld = mul(input.position, worldMatrix);
	output.s_pos=output.position;

	
	//output.depth=length(gEyePos-posWorld)/1000;//(output.position.z/output.position.w);
	output.depth=output.position.w/10000;//(output.position.z/output.position.w)/1;
	//output.depth=(output.position.z/output.position.w);

	return output;
};

DEF_OUTPUT VShader( DEF_INPUT input )
{
	return calcDefOutput(input);
}

void PShader(DEF_OUTPUT input,
	out float4 ColorOut : COLOR0,
	out float4 NormalOut : COLOR1,
	out float4 DepthOut : COLOR2)
{   
	float2 TC=input.tex;
	
	ColorOut = tex2D(DiffuseMap,TC);
	if(ColorOut.a<0.5) discard;
	
	DepthOut = input.depth;
	
	float3 bumpMap=tex2D(NormalMap,TC).rgb;
	
	//if(length(bumpMap)==0) NormalOut.xyz=input.normal;
	//else
	//{
		//bumpMap = (bumpMap * 2.0f) - 1.0f;
		NormalOut.xyz=normalize(input.normal*bumpMap.z + bumpMap.x * input.tangent + bumpMap.y * input.binormal);
	//};
	NormalOut.xyz=(NormalOut.xyz+1.0)/2.0;
	
	//ColorOut.r=input.depth;
	ColorOut.a=1;
	NormalOut.a=1;
}

technique Main
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 VShader();
      PixelShader = compile ps_3_0 PShader();
   }
}