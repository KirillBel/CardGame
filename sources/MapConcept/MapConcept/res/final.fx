float2 screenSize : gDevViewportSize;

texture g_DownsampleTexture; 

sampler DownsampleMap = 
sampler_state
{
    Texture = < g_DownsampleTexture >;
	AddressU = CLAMP;
    AddressV = CLAMP;
    MagFilter = POINT;
    MinFilter = POINT;
    Mipfilter = POINT;
};

struct VS_INPUT 
{
	float4 position	: POSITION0;
    float2 tex	: TEXCOORD0;
};

struct VS_OUTPUT 
{
	float4 position : SV_Position;
	float2 tex		: TEXCOORD1;
};

VS_OUTPUT VShader( VS_INPUT input )
{
	VS_OUTPUT output=(VS_OUTPUT)0;
	output.position =input.position;
	
	float2 halfPixel=float2(0.5/screenSize.x,0.5/screenSize.y);
	output.tex=input.tex-halfPixel;
	return( output );
}


float4 PShader( VS_OUTPUT input ) : COLOR0
{   
	
}

technique Main
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 VShader();
      PixelShader = compile ps_3_0 PShader();
   }

}