#include "Engine_Shader_Defines.hlsl"

matrix g_WorldMatrix;
matrix		g_ViewMatrix, g_ProjMatrix;

//Texture2D	g_SceneTexture;
Texture2D	g_DistortionTexture;

float       g_fWaveLength = 50.f;
float       g_fWavePos = 100.f;
float       g_fWaveSize = 50.f;
float       g_fWaveCurvature = 10.f;
float       g_fWaveIntensity = 2.f;
float       g_fWaveU = 720.f;
float       g_fWaveV = 405.f;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{	
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(/* Á¤Á¡ */VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

////Use as you will.
//void mainImage(inout float4 vColor, in float2 vTexcoord)
//{
//    //Sawtooth function to pulse from centre.
//    float offset = (iTime - floor(iTime)) / iTime;
//    float CurrentTime = (iTime) * (offset);
    
//    float3 WaveParams = float3(10.0, 0.8, 0.1);
    
//    float ratio = iResolution.y / iResolution.x;
    
//    //Use this if you want to place the centre with the mouse instead
//	//vec2 WaveCentre = vec2( iMouse.xy / iResolution.xy );
       
//    float2 WaveCentre = float2(0.5, 0.5);
//    WaveCentre.y *= ratio;
   
//    float2 texCoord = vTexcoord.xy / iResolution.xy;
//    texCoord.y *= ratio;
//    float Dist = distance(texCoord, WaveCentre);
    
//    vec4 Color = texture(iChannel0, texCoord);
    
////Only distort the pixels within the parameter distance from the centre
//    if ((Dist <= ((CurrentTime) + (WaveParams.z))) &&
//	(Dist >= ((CurrentTime) - (WaveParams.z))))
//    {
//        //The pixel offset distance based on the input parameters
//        float Diff = (Dist - CurrentTime);
//        float ScaleDiff = (1.0 - pow(abs(Diff * WaveParams.x), WaveParams.y));
//        float DiffTime = (Diff * ScaleDiff);
        
//        //The direction of the distortion
//        vec2 DiffTexCoord = normalize(texCoord - WaveCentre);
        
//        //Perform the distortion and reduce the effect over time
//        texCoord += ((DiffTexCoord * DiffTime) / (CurrentTime * Dist * 40.0));
//        Color = texture(iChannel0, texCoord);
        
//        //Blow out the color and reduce the effect over time
//        Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.0);
//    }
    
//    fragColor = Color;
//}

PS_OUT PS_SHOCKWAVE_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
    
    vector vSourColor = g_DistortionTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vSourColor.r < 0.01f)
        discard;
    
    Out.vColor = vSourColor;
    
    
    /////////////////////////////////////////
    //float2 waveCenter;
    //waveCenter.x = g_fWaveU;
    //waveCenter.y = g_fWaveV;
	
    //float dist = distance(In.vTexcoord, waveCenter);
    //float waveDecreaseFactor = 1 - dist / g_fWaveLength;
    //if (dist <= g_fWavePos + g_fWaveSize * waveDecreaseFactor && dist >= g_fWavePos - g_fWaveSize * waveDecreaseFactor)
    //{
    //    float ecart = (dist - g_fWavePos);
    //    float powEcart = 1.0 - pow(abs(ecart * g_fWaveCurvature), g_fWaveIntensity);
    //    float ecartTime = ecart * powEcart;
    //    float2 diff = normalize(In.vTexcoord - waveCenter);
    //    float2 newTexCoord = 2.f * In.vTexcoord + (diff * ecartTime);
	
    //    Out.vColor = g_SceneTexture.Sample(LinearSampler, newTexCoord);

    //}
    //else
    //{
    //    Out.vColor = g_SceneTexture.Sample(LinearSampler, In.vTexcoord);
    //}
 
	return Out;
}

technique11 DefaultTechnique
{
	pass Shockwave
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SHOCKWAVE_MAIN();
        ComputeShader = NULL;
    }
}




