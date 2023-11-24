#include "Client_Shader_Defines.hlsl"
#include "Shader_Lighting_Functions.hlsl"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float   g_fFrameTime;

vector g_vCamPosition;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{	
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	/* mul : 모든(곱하기가 가능한) 행렬의 곱하기를 수행한다. */
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

VS_OUT VS_BUBBLE_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	/* mul : 모든(곱하기가 가능한) 행렬의 곱하기를 수행한다. */
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    In.vPosition *= (30.f * g_fFrameTime);
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};
    
struct PS_WAVE_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_BUBBLE_MAIN(PS_IN In)
{
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexcoord);
	
    PS_OUT Out = (PS_OUT) 0;
	
    float4 vRedColor = float4(1.f, 0.2f, 0.25f, 1.f);
    
    vector vLook = In.vWorldPos - g_vCamPosition;
    float3 E = normalize(-vLook);
    
    float value = saturate(dot(E, float3(In.vNormal.xyz)));
    float fEmissive = 1.0f - value;
    
    fEmissive = smoothstep(0.0f, 1.0f, fEmissive);
    fEmissive = pow(fEmissive, 2 + 10.f * g_fFrameTime);
    
    Out.vColor = vRedColor * fEmissive;
    
    return Out;
}

sampler Sampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    //AddressV = wrap;
};

PS_WAVE_OUT PS_WAVERING_MAIN(PS_IN In)
{
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexcoord);
	
    PS_WAVE_OUT Out = (PS_WAVE_OUT) 0;
    
    In.vTexcoord.x *= 5.f;
    In.vTexcoord.y += (3.f * g_fFrameTime - 0.7f);
    float3 vColor = g_DiffuseTexture.Sample(Sampler, In.vTexcoord);
    
    if (vColor.r == 0.f)
        discard;
    
    vColor = 1.f - vColor;
    //vColor = pow(vColor, 4.f);
    //vColor.bg = 1.f - (0.8f * vColor.r);
    vColor.r = 1.f;
    vColor.b += 0.1f;
    //vColor.b *= 1.2f;
    
    clamp(vColor.b, 0.f, 1.f);
    
    Out.vColor = float4(vColor, (0.55f - 2.f * g_fFrameTime) * vColor.g);
    
    return Out;
}

technique11 DefaultTechnique
{
	pass Bubble
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        //SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_BUBBLE_MAIN();
        GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BUBBLE_MAIN();
        ComputeShader = NULL;
    }

    pass WaveRing
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_WAVERING_MAIN();
        ComputeShader = NULL;
    }
}




