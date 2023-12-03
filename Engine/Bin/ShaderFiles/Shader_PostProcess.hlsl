#include "Engine_Shader_Defines.hlsl"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D	g_DistortionTarget;

float4      g_CamPosition;
float2      g_ScreenSunPosition;

Texture2D	g_SceneTarget;
Texture2D	g_GodRayTarget;
Texture2D	g_SunOccluderTarget;
float4      g_LightShaftValue = float4(1.f, 0.9733f, 1.f, 2.f);
// x = Density; y = Decay(각 샘플의 분산되는 영향도); z = Weight(샘플에 대한 전체 강도); w = Exposure(포스트 프로세스 전체 강도);

Texture2D   g_GlowTarget;
Texture2D   g_BlurHTarget;
Texture2D   g_BlurHVTarget;
Texture2D   g_SpecularTarget;
Texture2D   g_EmissiveTarget;

float4      g_vLightEmissive = float4(1.f, 1.f, 1.f, 1.f);

Texture2D	g_Texture;

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

VS_OUT VS_MAIN(/* 정점 */VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

struct VS_OUT_BLUR
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord[14] : TEXCOORD;
};

VS_OUT_BLUR VS_MAIN_BLURH( /* 정점 */VS_IN In)
{
    VS_OUT_BLUR Out = (VS_OUT_BLUR) 0;
	
    float texelSize;
    
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

    Out.vTexcoord[0] = In.vTexcoord;

    texelSize = 1.0f / 1440.f;
    
    for (int i = -6; i < 7; ++i)    
        Out.vTexcoord[i + 7] = In.vTexcoord + float2(texelSize * (float)i, 0.0f);
    
    return Out;
}

VS_OUT_BLUR VS_MAIN_BLURV( /* 정점 */VS_IN In)
{
    VS_OUT_BLUR Out = (VS_OUT_BLUR) 0;
	
    float texelSize;
    
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

    Out.vTexcoord[0] = In.vTexcoord;

    texelSize = 1.0f / 810.f;
    
    for (int i = -6; i < 7; ++i)    
        Out.vTexcoord[i + 7] = In.vTexcoord + float2(0.0f, texelSize * (float)i);
    
    return Out;
}

PS_OUT PS_MAIN_LIGHTSHAFT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vMaskColor = g_SunOccluderTarget.Sample(PointSampler, In.vTexcoord);
    
    #define SAMPLINGCOUNT 192.f
    
    float2 vTexCoord = In.vTexcoord;
    
    float2 vDeltaTexCoord = float2(In.vTexcoord - g_ScreenSunPosition);
    
    vDeltaTexCoord *= ((1.0f / SAMPLINGCOUNT) * g_LightShaftValue.x);
    
    float fIlluminationDecay = 1.0f;
    
    for (int i = 0; i < SAMPLINGCOUNT; ++i)
    {
        vTexCoord -= vDeltaTexCoord;
        float4 vNewPixel = g_SunOccluderTarget.Sample(PointSampler, vTexCoord);
        vNewPixel *= fIlluminationDecay * g_LightShaftValue.z;
        
        vMaskColor += vNewPixel;

        fIlluminationDecay *= g_LightShaftValue.y;
    }
    
    Out.vColor = saturate(vMaskColor * g_LightShaftValue.w);
        
    return Out;
}

struct PS_IN_BLUR
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord[14] : TEXCOORD;
};

PS_OUT PS_MAIN_BLURH(PS_IN_BLUR In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float normalization;
    float4 color;
    
    float fWeight[7] = { 1.0f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
    
    normalization = (fWeight[0] + 2.0f * (fWeight[1] + fWeight[2] + fWeight[3] + fWeight[4] + fWeight[5] + fWeight[6]));
    
    for (int i = 0; i < 7; ++i)
        fWeight[i] = fWeight[i] / normalization;
    
    color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int j = -6; j < 7; ++j)
    {
        color += (
        g_GlowTarget.Sample(LinearSampler, In.vTexcoord[7 + j]) +
        g_SpecularTarget.Sample(LinearSampler, In.vTexcoord[7 + j]) +
        g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord[7 + j]) * g_vLightEmissive +
        g_GodRayTarget.Sample(LinearSampler, In.vTexcoord[7 + j])
        ) * fWeight[abs(j)];
    }
    
    color.a = 1.0f;

    Out.vColor = color;
    
    return Out;
}

PS_OUT PS_MAIN_BLURV(PS_IN_BLUR In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float normalization;
    float4 color;
    
    float fWeight[7] = { 1.0f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
    
    normalization = (fWeight[0] + 2.0f * (fWeight[1] + fWeight[2] + fWeight[3] + fWeight[4] + fWeight[5] + fWeight[6]));
    
    for (int i = 0; i < 7; ++i)
        fWeight[i] = fWeight[i] / normalization;
    
    //weight0 = 1.0f;
    //weight1 = 0.9f;
    //weight2 = 0.55f;
    //weight3 = 0.18f;
    //weight4 = 0.1f;
    
    color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int j = -6; j < 7; ++j)
        color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord[7 + j]) * fWeight[abs(j)];
    
    color.a = 1.0f;
    
    Out.vColor = color;
    
    return Out;
}

PS_OUT PS_MAIN_POSTPROCESS(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float fDistortion = g_DistortionTarget.Sample(LinearSampler, In.vTexcoord).x;

    if (fDistortion > 0.f)
    {
        fDistortion -= 0.5f;
        fDistortion *= 2.f;
        fDistortion *= 0.05f;
    }
    
    float4 vScene = g_SceneTarget.Sample(LinearSampler, saturate(In.vTexcoord + float2(fDistortion, fDistortion)));
    if (vScene.a == 0.f)
        discard;
    
    //float4 vBlur = g_BlurHVTarget.Sample(LinearSampler, In.vTexcoord);
    //float4 vBlend = g_BlendTarget.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vScene /* + vBlur*/ /* + vBlend*/;
    

        return Out;
}

technique11 DefaultTechnique
{
   pass PostProcess
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POSTPROCESS();
        ComputeShader = NULL;
    }

	pass LightShafting
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHTSHAFT();
        ComputeShader = NULL;
    }

    pass BlurHorizontal
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
	
        VertexShader = compile vs_5_0 VS_MAIN_BLURH();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLURH();
        ComputeShader = NULL;
    }

    pass BlurVertical
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
	
        VertexShader = compile vs_5_0 VS_MAIN_BLURV();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLURV();
        ComputeShader = NULL;
    }
}




