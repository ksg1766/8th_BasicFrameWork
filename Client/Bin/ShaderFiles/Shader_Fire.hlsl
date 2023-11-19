#include "Client_Shader_Defines.hlsl"
#include "Shader_Lighting_Functions.hlsl"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float   g_fFrameTime;
float3 g_vScrollSpeeds = float3(1.3f, 2.1f, 2.3f);
float3 g_vScales = float3(1.0f, 2.0f, 3.0f);

Texture2D g_FireTexture;
Texture2D g_NoiseTexture;
Texture2D g_AlphaTexture;

float2 g_vDistortion1 = float2(0.1f, 0.2f);
float2 g_vDistortion2 = float2(0.1f, 0.3f);
float2 g_vDistortion3 = float2(0.1f, 0.1f);
float g_fDistortionScale = 0.8f;
float g_fDistortionBias = 0.5f;

sampler Sampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{	
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vNoiseUV1 : TEXCOORD1;
    float2 vNoiseUV2 : TEXCOORD2;
    float2 vNoiseUV3 : TEXCOORD3;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vTexcoord = In.vTexcoord;
    
    Out.vNoiseUV1 = (In.vTexcoord * g_vScales.x);
    Out.vNoiseUV1.y = Out.vNoiseUV1.y + (g_fFrameTime * g_vScrollSpeeds.x);
    
    Out.vNoiseUV2 = (In.vTexcoord * g_vScales.y);
    Out.vNoiseUV2.y = Out.vNoiseUV2.y + (g_fFrameTime * g_vScrollSpeeds.y);
    
    Out.vNoiseUV3 = (In.vTexcoord * g_vScales.z);
    Out.vNoiseUV3.y = Out.vNoiseUV3.y + (g_fFrameTime * g_vScrollSpeeds.z);
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vNoiseUV1 : TEXCOORD1;
    float2 vNoiseUV2 : TEXCOORD2;
    float2 vNoiseUV3 : TEXCOORD3;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float4 vNoise1;
    float4 vNoise2;
    float4 vNoise3;
    float4 vFinalNoise;
    float  fPerturb;
    float2 vNoiseCoords;
    float4 vFireColor;
    float4 vAlphaColor;
    
    // 동일한 노이즈 텍스쳐를 서로 다른 세 텍스쳐 좌표를 사용하여 세 개의 다른 크기의 노이즈를 얻습니다.
    vNoise1 = g_NoiseTexture.Sample(LinearSampler, In.vNoiseUV1);
    vNoise2 = g_NoiseTexture.Sample(LinearSampler, In.vNoiseUV2);
    vNoise3 = g_NoiseTexture.Sample(LinearSampler, In.vNoiseUV3);

    // 노이즈 값의 범위를 (0, 1)에서 (-1, +1)이 되도록 합니다.
    vNoise1 = (vNoise1 - 0.5f) * 2.0f;
    vNoise2 = (vNoise2 - 0.5f) * 2.0f;
    vNoise3 = (vNoise3 - 0.5f) * 2.0f;
    
     // 노이즈의 x와 y값을 세 개의 다른 왜곡 x및 y좌표로 흩뜨립니다.
    vNoise1.xy = vNoise1.xy * g_vDistortion1.xy;
    vNoise2.xy = vNoise2.xy * g_vDistortion2.xy;
    vNoise3.xy = vNoise3.xy * g_vDistortion3.xy;

    // 왜곡된 세 노이즈 값들을 하나의 노이즈로 함성합니다.
    vFinalNoise = vNoise1 + vNoise2 + vNoise3;
    
     // 입력으로 들어온 텍스쳐의 Y좌표를 왜곡 크기와 바이어스 값으로 교란시킵니다.
    // 이 교란은 텍스쳐의 위쪽으로 갈수록 강해져서 맨 위쪽에는 깜박이는 효과를 만들어냅니다.
    fPerturb = ((1.0f - In.vTexcoord.y) * g_fDistortionScale) + g_fDistortionBias;

    // 불꽃 색상 텍스쳐를 샘플링하는데 사용될 왜곡 및 교란된 텍스쳐 좌표를 만듭니다.
    vNoiseCoords.xy = (vFinalNoise.xy * fPerturb) + In.vTexcoord.xy;
    
     // 왜곡되고 교란된 텍스쳐 좌표를 이용하여 불꽃 텍스쳐에서 색상을 샘플링합니다.
    // wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
    vFireColor = g_FireTexture.Sample(Sampler, vNoiseCoords.xy);

    // 왜곡되고 교란된 텍스쳐 좌표를 이용하여 알파 텍스쳐에서 알파값을 샘플링합니다.
    // 불꽃의 투명도를 지정하는 데 사용될 것입니다.
    // wrap를 사용하는 스테이트 대신 clamp를 사용하는 스테이트를 사용하여 불꽃 텍스쳐가 래핑되는 것을 방지합니다.
    vAlphaColor = g_AlphaTexture.Sample(Sampler, vNoiseCoords.xy);
    
    // 왜곡 및 교란된 알파 텍스쳐 값을 알파블렌딩에 사용합니다.
    vFireColor.a = vAlphaColor;
    
    Out.vColor = vFireColor;
    
    return Out;
}

technique11 DefaultTechnique
{
	pass Fire
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
        ComputeShader = NULL;
    }
}




