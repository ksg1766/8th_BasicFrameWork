#include "Client_Shader_Defines.hlsl"
#include "Shader_Lighting_Functions.hlsl"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float   g_fFrameTime;
float4 g_vLightningEmissive = (1.f, 0.f, 0.f, 1.f);

Texture2D g_FireTexture;

vector g_vCamPosition;

Texture2D g_Textures[2];

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vPSize : PSIZE;
};

struct VS_OUT
{	
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    vector vRight = g_WorldMatrix._11_12_13_14;
    vector vUp = g_WorldMatrix._21_22_23_24;
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vPSize = float2(In.vPSize.x * length(vRight), In.vPSize.y * length(vUp));
    
    return Out;
}

struct GS_IN
{
    float4 vPosition : POSITION;
    float2 vPSize : PSIZE;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    GS_OUT Out[4];

    vector vLook = g_vCamPosition - In[0].vPosition;

    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vPSize.x * 0.5f;
    float3 vUp = normalize(cross(vLook.xyz, vRight.xyz)) * In[0].vPSize.y * 0.5f;

    matrix matVP;

    matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = vector(In[0].vPosition.xyz + vRight + vUp, 1.f);
    Out[0].vPosition = mul(Out[0].vPosition, matVP);
    Out[0].vTexcoord = float2(0.0f, 0.f);

    Out[1].vPosition = vector(In[0].vPosition.xyz - vRight + vUp, 1.f);
    Out[1].vPosition = mul(Out[1].vPosition, matVP);
    Out[1].vTexcoord = float2(1.0f, 0.f);
    
    Out[2].vPosition = vector(In[0].vPosition.xyz - vRight - vUp, 1.f);
    Out[2].vPosition = mul(Out[2].vPosition, matVP);
    Out[2].vTexcoord = float2(1.0f, 1.0f);
    
    Out[3].vPosition = vector(In[0].vPosition.xyz + vRight - vUp, 1.f);
    Out[3].vPosition = mul(Out[3].vPosition, matVP);
    Out[3].vTexcoord = float2(0.0f, 1.0f);
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip();

    OutStream.Append(Out[0]);
    OutStream.Append(Out[2]);
    OutStream.Append(Out[3]);
    OutStream.RestartStrip();
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

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float3 vColor = g_FireTexture.Sample(LinearSampler, In.vTexcoord);
    
    if(vColor.r == 0)
        discard;
    
    vColor = vColor * (1.f - g_vLightningEmissive.rgb);
    
    Out.vColor = float4(vColor, 1.f);
    
    return Out;
}

technique11 DefaultTechnique
{
	pass Lightning
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
        ComputeShader = NULL;
    }
}




