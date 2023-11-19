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
    
    // ������ ������ �ؽ��ĸ� ���� �ٸ� �� �ؽ��� ��ǥ�� ����Ͽ� �� ���� �ٸ� ũ���� ����� ����ϴ�.
    vNoise1 = g_NoiseTexture.Sample(LinearSampler, In.vNoiseUV1);
    vNoise2 = g_NoiseTexture.Sample(LinearSampler, In.vNoiseUV2);
    vNoise3 = g_NoiseTexture.Sample(LinearSampler, In.vNoiseUV3);

    // ������ ���� ������ (0, 1)���� (-1, +1)�� �ǵ��� �մϴ�.
    vNoise1 = (vNoise1 - 0.5f) * 2.0f;
    vNoise2 = (vNoise2 - 0.5f) * 2.0f;
    vNoise3 = (vNoise3 - 0.5f) * 2.0f;
    
     // �������� x�� y���� �� ���� �ٸ� �ְ� x�� y��ǥ�� ��߸��ϴ�.
    vNoise1.xy = vNoise1.xy * g_vDistortion1.xy;
    vNoise2.xy = vNoise2.xy * g_vDistortion2.xy;
    vNoise3.xy = vNoise3.xy * g_vDistortion3.xy;

    // �ְ�� �� ������ ������ �ϳ��� ������� �Լ��մϴ�.
    vFinalNoise = vNoise1 + vNoise2 + vNoise3;
    
     // �Է����� ���� �ؽ����� Y��ǥ�� �ְ� ũ��� ���̾ ������ ������ŵ�ϴ�.
    // �� ������ �ؽ����� �������� ������ �������� �� ���ʿ��� �����̴� ȿ���� �������ϴ�.
    fPerturb = ((1.0f - In.vTexcoord.y) * g_fDistortionScale) + g_fDistortionBias;

    // �Ҳ� ���� �ؽ��ĸ� ���ø��ϴµ� ���� �ְ� �� ������ �ؽ��� ��ǥ�� ����ϴ�.
    vNoiseCoords.xy = (vFinalNoise.xy * fPerturb) + In.vTexcoord.xy;
    
     // �ְ�ǰ� ������ �ؽ��� ��ǥ�� �̿��Ͽ� �Ҳ� �ؽ��Ŀ��� ������ ���ø��մϴ�.
    // wrap�� ����ϴ� ������Ʈ ��� clamp�� ����ϴ� ������Ʈ�� ����Ͽ� �Ҳ� �ؽ��İ� ���εǴ� ���� �����մϴ�.
    vFireColor = g_FireTexture.Sample(Sampler, vNoiseCoords.xy);

    // �ְ�ǰ� ������ �ؽ��� ��ǥ�� �̿��Ͽ� ���� �ؽ��Ŀ��� ���İ��� ���ø��մϴ�.
    // �Ҳ��� ������ �����ϴ� �� ���� ���Դϴ�.
    // wrap�� ����ϴ� ������Ʈ ��� clamp�� ����ϴ� ������Ʈ�� ����Ͽ� �Ҳ� �ؽ��İ� ���εǴ� ���� �����մϴ�.
    vAlphaColor = g_AlphaTexture.Sample(Sampler, vNoiseCoords.xy);
    
    // �ְ� �� ������ ���� �ؽ��� ���� ���ĺ����� ����մϴ�.
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




