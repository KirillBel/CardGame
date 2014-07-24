texture g_DownsampleTexture; 

float2 screenSize : gDevViewportSize;
float range=1.1;

float PixelKernel[13] =
{
    -6,
    -5,
    -4,
    -3,
    -2,
    -1,
     0,
     1,
     2,
     3,
     4,
     5,
     6,
};

static const float BlurWeights[13] = 
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

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
	//float4 position : POSITION0;
	float4 position : SV_Position;
	float4 screenPosition		: TEXCOORD0;
	float2 tex		: TEXCOORD1;
};

VS_OUTPUT VShader( VS_INPUT input )
{
	VS_OUTPUT output=(VS_OUTPUT)0;
	output.position =input.position;
	output.screenPosition = output.position;
	
	float2 halfPixel=float2(0.5/screenSize.x,0.5/screenSize.y);
	output.tex=input.tex-halfPixel;
	return( output );
}


float4 VPass( VS_OUTPUT input ) : COLOR0
{   
	float2 halfPixel=float2(0.5/screenSize.x,0.5/screenSize.y);
    float4 color = 0;
    float2 samp = input.tex;
	float4 probe;
	
    for (int i = 0; i < 13; i++) {
        samp = input.tex + PixelKernel[i] * halfPixel * range * float2(1,0);
		probe=tex2D(DownsampleMap, samp.xy);
		color+=probe * BlurWeights[i];
    };

    return color;
}

float4 HPass( VS_OUTPUT input ) : COLOR0
{   
	float2 halfPixel=float2(0.5/screenSize.x,0.5/screenSize.y);
    float4 color = 0;
    float2 samp = input.tex;
	float4 probe;
	
    for (int i = 0; i < 13; i++) {
        samp = input.tex + PixelKernel[i] * halfPixel * range * float2(0,1);
		probe=tex2D(DownsampleMap, samp.xy);
		color+=probe * BlurWeights[i];
    };

    return color;
}

technique Main
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 VShader();
      PixelShader = compile ps_3_0 VPass();
   }
   
   pass Pass_1
   {
      VertexShader = compile vs_3_0 VShader();
      PixelShader = compile ps_3_0 HPass();
   }

}