matrix WVP : gDevWVP;
matrix ViewProj : gDevViewProj;
float2 screenSize : gDevViewportSize;

#define DOSMALL
#define DOLARGE

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
    FILTER = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
}; 

sampler2D DepthSampler = sampler_state {
    Texture = <DepthTex>;
    FILTER = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
}; 

float2 nearFar : gDevCameraNearFar;
float2 worldToScreenScale=float2(1,1);
float2 texSize0=float2(1366,768);
float2 texSize1=float2(64,64);
float2 targetSize=float2(1366,768);

// Script-set constants.

float overallStrength=2;

float sRadius=1;
float sStrength=4;
float sDepthMin=0;
float sDepthMax=1;
float sDepthPow=1;
float sNormalTol=0;
float sNormalPow=0;

float lRadius=1;
float lStrength=4;
float lDepthMin=0;
float lDepthMax=10000;
float lDepthPow=1;
float lNormalTol=0;
float lNormalPow=0;

#ifndef QUALITY
   #define QUALITY 2
#endif


#if QUALITY == 0
   #define sSampleCount 4
   #define totalSampleCount 12
#elif QUALITY == 1
   #define sSampleCount 6
   #define totalSampleCount 24
#elif QUALITY == 2
   #define sSampleCount 8
   #define totalSampleCount 32
#endif

float getOcclusion( float depthDiff, float depthMin, float depthMax, float depthPow, 
                    float normalDiff, float dt, float normalTol, float normalPow )
{
   if ( depthDiff < 0.0 )
      return 0.0;
      
   float delta = abs( depthDiff );
   
   if ( delta < depthMin || delta > depthMax )
      return 0.0;   
      
   delta = saturate( delta / depthMax );
   
   if ( dt > 0.0 )
      normalDiff *= dt;
   else
      normalDiff = 1.0;
      
      
   normalDiff *= 1.0 - ( dt * 0.5 + 0.5 );
        
   //return ( 1.0 - tex1D( powTable, delta ).r ) * normalDiff;
   float power = pow( max( delta, 0 ), 0.1 );  
   return ( 1.0 - power ) * normalDiff;
}
///////////////////////////////////////////////////////


struct QuadVertexOutput {
    float4 Position	: POSITION;
    float2 UV		: TEXCOORD0;
};

QuadVertexOutput VShader( QuadVertexOutput input )
{
	QuadVertexOutput output=(QuadVertexOutput)0;
	
	float2 halfPixel=float2(0.5/screenSize.x,0.5/screenSize.y);
	output.Position = input.Position;//mul(input.Position,WVP);
	output.UV = input.UV-halfPixel;
	
	return output;
}

float3 vector_to_texture(float3 v) { return ((v*0.5)+float3(0.5,0.5,0.5)); }
float3 texture_to_vector(float3 t) { return ((t-float3(0.5,0.5,0.5))*2.0); }

//////////////// END PASS 0////////////////////////

//////////////// PASS 1 ////////////////////////////

float4 prepassUncondition(float2 uv_crd)
{
	float4 f;
	f.xyz=normalize( texture_to_vector(tex2D(NormSampler,uv_crd).xyz));
	f.a=tex2D(DepthSampler,uv_crd);
	return f;
};

float4 SSAOMain( QuadVertexOutput IN ) : COLOR
{
	//return tex2D(NormSampler,IN.UV);
	float3 ptSphere[32] =
   {
   	float3( 0.295184, 0.077723, 0.068429 ),
   	float3( -0.271976, -0.365221, -0.838363 ),
   	float3( 0.547713, 0.467576, 0.488515 ),
   	float3( 0.662808, -0.031733, -0.584758 ),
   	float3( -0.025717, 0.218955, -0.657094 ),
   	float3( -0.310153, -0.365223, -0.370701 ),
   	float3( -0.101407, -0.006313, -0.747665 ),
   	float3( -0.769138, 0.360399, -0.086847 ),
   	float3( -0.271988, -0.275140, -0.905353 ),
   	float3( 0.096740, -0.566901, 0.700151 ),
   	float3( 0.562872, -0.735136, -0.094647 ),
   	float3( 0.379877, 0.359278, 0.190061 ),
   	float3( 0.519064, -0.023055, 0.405068 ),
   	float3( -0.301036, 0.114696, -0.088885 ),
   	float3( -0.282922, 0.598305, 0.487214 ),
   	float3( -0.181859, 0.251670, -0.679702 ),
   	float3( -0.191463, -0.635818, -0.512919 ),
   	float3( -0.293655, 0.427423, 0.078921 ),
   	float3( -0.267983, 0.680534, -0.132880 ),
   	float3( 0.139611, 0.319637, 0.477439 ),
   	float3( -0.352086, 0.311040, 0.653913 ),
   	float3( 0.321032, 0.805279, 0.487345 ),
   	float3( 0.073516, 0.820734, -0.414183 ),
   	float3( -0.155324, 0.589983, -0.411460 ),
   	float3( 0.335976, 0.170782, -0.527627 ),
   	float3( 0.463460, -0.355658, -0.167689 ),
   	float3( 0.222654, 0.596550, -0.769406 ),
   	float3( 0.922138, -0.042070, 0.147555 ),
   	float3( -0.727050, -0.329192, 0.369826 ),
   	float3( -0.090731, 0.533820, 0.463767 ),
   	float3( -0.323457, -0.876559, -0.238524 ),
   	float3( -0.663277, -0.372384, -0.342856 )
   };
   
   // Sample a random normal for reflecting the 
   // sphere vector later in our loop.   
   float4 noiseMapUV = float4( ( IN.UV * ( targetSize / texSize1 ) ).xy, 0, 0 );
   float3 reflectNormal = normalize( tex2Dlod( randNormalSampler, noiseMapUV ).xyz * 2.0 - 1.0 );   
   //return float4( reflectNormal, 1 );
   
   float4 prepass = prepassUncondition( IN.UV );
   float3 normal = prepass.xyz;
   float depth = prepass.a;
   if(depth==0)
   	return float4(1,0,0,1);
	//return prepass;
	//return float4(depth,depth,depth,1);
   
   //return float4( normal, 1 );
   //return float4(tex2D( randNormalSampler, IN.UV ).xyz,1);
   //return float4( ( depth ).xxx, 1 );
      
   // Early out if too far away.
   if ( depth > 0.99999999 )
      return float4( 0,0,0,0 );      

   // current fragment coords in screen space
   float3 ep = float3( IN.UV, depth );        
   
   float bl;
   float3 baseRay, ray, se, occNorm, projRadius;
   float normalDiff = 0;
   float depthMin, depthMax, dt, depthDiff;    
   float4 occluderFragment=0;
   int i;
   float sOcclusion = 0.0;
   float lOcclusion = 0.0;
   
   //------------------------------------------------------------
   // Small radius
   //------------------------------------------------------------   

#ifdef DOSMALL

   bl = 0.0;
   
   //return float4((depth).xxx,1);
   //return float4((depth * nearFar.y).xxx,1);
   //projRadius.xy=( float2( sRadius.rr ) / ( depth * nearFar.y ) );
   projRadius.xy =  (( float2( sRadius.rr ) / ( depth * nearFar.y ) ) * ( worldToScreenScale / texSize0 ));
   projRadius.z = sRadius / nearFar.y;
  
   
   depthMin = projRadius.z * sDepthMin;
   depthMax = projRadius.z * sDepthMax;
   
   float maxr = 1;
   //radiusDepth = clamp( radiusDepth, 0.0001, maxr.rrr );   
   //if ( radiusDepth.x < 1.0 / targetSize.x )
   //   return color;      
   //radiusDepth.xyz = 0.0009;
   float fD=0;
   for ( i = 0; i < sSampleCount; i++ )
   {
      baseRay = reflect( ptSphere[i], reflectNormal );
      
      dt = dot( baseRay.xyz, normal );
      
      baseRay *= sign( dt );
         
      ray = ( projRadius * baseRay.xzy );
      ray.y = -ray.y;      
       
      se = ep + ray;
            
	  //occluderFragment.xyz=normalize( tex2D(NormSampler,IN.UV).xyz * 2.0 - 1.0 );
	  //occluderFragment.a=tex2D(ViewSampler,IN.UV).x;
      occluderFragment = prepassUncondition( se.xy );
      
      depthDiff = se.z - occluderFragment.w; 
	  
      
      dt = dot( occluderFragment.xyz, baseRay.xyz );
      normalDiff = dot( occluderFragment.xyz, normal );   
	  fD+=normalDiff;
      
      bl += getOcclusion( depthDiff, depthMin, depthMax, sDepthPow, normalDiff, dt, sNormalTol, sNormalPow );         
   }
   
   sOcclusion = sStrength * ( bl / (float)sSampleCount );

   //return float4(depth.xxx,1);
   //return float4(fD.xxx/10,1);
   //return float4((fD/float(sSampleCount)).xxx,1);
	return float4(sOcclusion.xxx*10,1);
#endif // DOSMALL
   
   
   //------------------------------------------------------------
   // Large radius
   //------------------------------------------------------------
   
#ifdef DOLARGE
      
   bl = 0.0;

   projRadius.xy =  ( float2( lRadius.rr ) / ( depth * nearFar.y ) ) * ( worldToScreenScale / texSize0 );
   projRadius.z = lRadius / nearFar.y;
   
   depthMin = projRadius.z * lDepthMin;
   depthMax = projRadius.z * lDepthMax;
   
   //projRadius.xy = clamp( projRadius.xy, 0.0, 0.01 );
   //float maxr = 1;
   //radiusDepth = clamp( radiusDepth, 0.0001, maxr.rrr );   
   //if ( radiusDepth.x < 1.0 / targetSize.x )
   //   return color;      
   //radiusDepth.xyz = 0.0009;   
   
   for ( i = sSampleCount; i < totalSampleCount; i++ )
   {
      baseRay = reflect( ptSphere[i], reflectNormal );
      
      dt = dot( baseRay.xyz, normal );
      
      baseRay *= sign( dt );
         
      ray = ( projRadius * baseRay.xzy );
      ray.y = -ray.y;      
       
      se = ep + ray;
            
      occluderFragment = prepassUncondition( se.xy );                
      
      depthDiff = se.z - occluderFragment.a;       
      
      normalDiff = dot( occluderFragment.xyz, normal );        
      dt = dot( occluderFragment.xyz, baseRay.xyz );         
               
      bl += getOcclusion( depthDiff, depthMin, depthMax, lDepthPow, normalDiff, dt, lNormalTol, lNormalPow );        
   }
      
   lOcclusion = lStrength * ( bl / (float)( totalSampleCount - sSampleCount ) );

#endif // DOLARGE
   
   float occlusion = saturate( max( sOcclusion, lOcclusion ) * overallStrength );   
   
   // Note black is unoccluded and white is fully occluded.  This
   // seems backwards, but it makes it simple to deal with the SSAO
   // being disabled in the lighting shaders.   
   
   //return float4(tex2D(ColrSampler,IN.UV).xyz*(1.f-occlusion),1);
   return 1.f-occlusion;      
};
//////////////// END PASS 1 ////////////////////////

technique Main {
    
	pass SSAO  {        
        VertexShader = compile vs_3_0 VShader();
	    ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = None;
        PixelShader = compile ps_3_0 SSAOMain();
    }
}
