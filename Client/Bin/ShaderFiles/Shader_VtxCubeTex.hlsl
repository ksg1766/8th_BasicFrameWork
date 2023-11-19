#include "Client_Shader_Defines.hlsl"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
TextureCube	g_DiffuseTexture;

sampler DefaultSampler = sampler_state {

    Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler PointSampler = sampler_state {

    Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_IN
{
	float3		vPosition : POSITION;	
    float3		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;	
    float3		vTexcoord : TEXCOORD0;
};

struct VS_BIG_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vTexcoord : TEXCOORD0;
    
    float4 vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
	
	return Out;
}

VS_BIG_OUT VS_BIG_MAIN(VS_IN In)
{
    VS_BIG_OUT Out = (VS_BIG_OUT) 0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;
	
	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
    float3	vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

struct PS_BIG_IN
{
    float4 vPosition : SV_POSITION;
    float3 vTexcoord : TEXCOORD0;
    
    float4 vProjPos : TEXCOORD1;
};

struct PS_BIG_OUT
{
    float4 vColor : SV_TARGET0;
    //float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
	//Out.vColor = (vector)0.f;

	return Out;
}

PS_BIG_OUT PS_BIG_MAIN(PS_BIG_IN In)
{
    PS_BIG_OUT Out = (PS_BIG_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    //Out.vNormal = ;
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 2000.0f, 0.f, 0.f);
	//Out.vColor = (vector)0.f;

    return Out;
}

BlendState DefaultBlendState
{
	BlendEnable[0] = true;
	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;	
};

technique11 DefaultTechnique
{
	pass SkyBox
	{
		SetRasterizerState(RS_Skybox);
		SetDepthStencilState(DSS_Skybox, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

	/*	SetBlendState(DefaultBlendState);
		SetRasterizerState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
        ComputeShader = NULL;
    }

    pass SkyBoxBig
    {
        SetRasterizerState(RS_SkyboxBig);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_BIG_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BIG_MAIN();
        ComputeShader = NULL;
    }

    pass Cube
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);

        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
	/*	SetBlendState(DefaultBlendState);
		SetRasterizerState();
		*/

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
        ComputeShader = NULL;
    }
}