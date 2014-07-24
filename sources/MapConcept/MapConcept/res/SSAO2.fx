float2 screenSize : gDevViewportSize;
matrix WVP : gDevWVP;

static const float strength = 0.125;
static const float2 offset  = float2( 1366.0 / 4.0, 768.0 / 4.0 );
static const float falloff  = 0.0000002;
static const float rad = 0.006;

#define NUM_SAMPLES	 10
static const float invSamples = 1.0 / (float)NUM_SAMPLES;

// AO sampling directions 
static const float3 AO_SAMPLES[ NUM_SAMPLES ] = 
{
    float3(-0.010735935,  0.0164701800,  0.0062425877),
    float3(-0.065333690,  0.3647007000, -0.1374632100),
    float3(-0.653923500, -0.0167263880, -0.5300095700),
    float3( 0.409582850,  0.0052428036, -0.5591124000),
    float3(-0.146536600,  0.0989926700,  0.1557167900),
    float3(-0.441221120, -0.5458797000,  0.0491253200),
    float3( 0.037555660, -0.1096134500, -0.3304027300),
    float3( 0.019100213,  0.2965278300,  0.0662376660),
    float3( 0.876532300,  0.0112360040,  0.2826596200),
    float3( 0.292644350, -0.4079423800,  0.1596416700)
};

// Textures.
//

texture randNormalTex;
texture NormalTex;
texture DepthTex;

sampler2D randNormalSampler = sampler_state {
    Texture = <randNormalTex>;
    FILTER = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
}; 

sampler2D NormSampler = sampler_state {
    Texture = <NormalTex>;
    FILTER = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
}; 

sampler2D DepthSampler = sampler_state {
    Texture = <DepthTex>;
    FILTER = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
}; 

// Vertex Shader.
//
struct VSInput
{
    float4 Position    :POSITION;
    float2 Texture0    :TEXCOORD0;
};

struct VSOutput
{
    float4 Position    :SV_POSITION;
    float2 Texture0    :TEXCOORD0;
};

VSOutput MyVS( VSInput input )
{
    VSOutput output;

	float2 halfPixel=float2(0.5/screenSize.x,0.5/screenSize.y);
	
    output.Position = input.Position;
    output.Texture0 = input.Texture0;//-halfPixel;

    return output;
}

float4 MyPS( VSOutput input ):SV_Target
{

    float3 fres = normalize( tex2D( randNormalSampler, input.Texture0 * offset ).xyz * 2.0 - 1.0 );
    //float4 normal_depth = tex0.Sample( defss0, input.Texture0 );

    float3 normal = tex2D( NormSampler, input.Texture0).xyz;
	//normal=normalize(mul(float4(normal,1),WVP));
    float  depth  = tex2D( DepthSampler, input.Texture0);

    float3 ep = float3( input.Texture0, depth );
    float bl = 0.0;
    float radD = rad / depth;

    float3 ray;
    float4 occFrag;
    float  depthDiff;

    for( int i = 0; i < NUM_SAMPLES; ++i )
    {
        ray = radD * reflect( AO_SAMPLES[i], fres );
        occFrag.xyz = tex2D( NormSampler, ep.xy + sign(dot(ray, normal)) * ray.xy).xyz;
		occFrag.w=tex2D( DepthSampler, ep.xy + sign(dot(ray, normal)) * ray.xy);

        depthDiff = depth - occFrag.a;

        bl += step( falloff, depthDiff ) * (1.0 - dot( occFrag.xyz, normal )) * 
              (1.0 - smoothstep( falloff, strength, depthDiff ));
    }

    float ao = 1.0 - bl * invSamples;

	//return float4(depth,depth,depth,1);
	//return float4(bl,bl,bl,1);
	//return float4(normal,1);
    return float4( ao, ao, ao, 1.0 );
}

technique Main {
    
	pass SSAO  {      
	    VertexShader = compile vs_3_0 MyVS();
	    //ZEnable = false;
		//ZWriteEnable = false;
		//AlphaBlendEnable = false;
		//CullMode = None;
        PixelShader = compile ps_3_0 MyPS();
    }
}
