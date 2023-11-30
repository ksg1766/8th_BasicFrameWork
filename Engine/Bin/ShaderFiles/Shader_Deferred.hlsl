#include "Engine_Shader_Defines.hlsl"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix	g_ProjMatrixInv;
matrix	g_ViewMatrixInv;

// Shadow Map
matrix  g_LightViewMatrix;
matrix  g_LightProjMatrix;
Texture2D g_ShadowDepthTarget;
float g_fBias;

vector	g_vCamPosition;
vector	g_vLightDir;
vector	g_vLightPos;
vector	g_fLightRange;

vector	g_vLightDiffuse;
vector	g_vLightAmbient;
vector	g_vLightSpecular;

vector  g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector  g_vMtrlSpecular = vector(0.7f, 0.7f, 0.7f, 1.f);

Texture2D g_PriorityTarget;

Texture2D	g_NormalTarget;
Texture2D	g_DiffuseTarget;
Texture2D	g_DepthTarget;
Texture2D	g_BlueTarget;

Texture2D	g_ShadeTarget;

Texture2D	g_BlurHVTarget;

Texture2D	g_DistortionTarget;

Texture2D	g_SceneTarget;
Texture2D   g_BlendTarget;

// Temp : LightShafting
Texture2D   g_GodRayTarget;

// Water
Texture2D   g_ReflectionTarget;
Texture2D   g_RefractionTarget;

Texture2D   g_WaterTarget;
Texture2D   g_WaterDepthTarget;

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
    
struct PS_REFRACTION_OUT
{
    float4 vColor : SV_TARGET0;
};

struct PS_REFLECTION_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	return Out;
}

struct PS_OUT_LIGHT
{
	float4	vShade : SV_TARGET0;
    float4 vSpecular : SV_TARGET1;
    //float4 vLightProj : SV_TARGET2;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

    //Out.vLightProj.xy = In.vTexcoord.xy;
    
	vector		vNormalDesc = g_NormalTarget.Sample(PointSampler, In.vTexcoord);
    vector		vDepthDesc = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
    float		fViewZ = vDepthDesc.y * 2000.f;

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient));
    
	vector vReflect = reflect(normalize(g_vLightDir), vNormal);
    
	vector vWorldPos;
    
	/* 투영스페이스 상의 위치를 구한다. */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;

    ////////////////
    
    //if ((saturate(In.vTexcoord.x) == In.vTexcoord.x) && (saturate(In.vTexcoord.y) == In.vTexcoord))
    //{
    //    vDepthDesc

    //}
    
    ////////////////
        
	/* 뷰스페이스 상의 위치를 구한다. */
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vector vLook = vWorldPos - g_vCamPosition;

    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 20.f);
		
	return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

    vector vNormalDesc = g_NormalTarget.Sample(PointSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * 2000.f;

    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vWorldPos;

	/* 투영스페이스 상의 위치를 구한다. */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;

	/* 뷰스페이스 상의 위치를 구한다. */
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vector vLightDir = vWorldPos - g_vLightPos;
    float fDistance = length(vLightDir);

    float fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);

    Out.vShade = fAtt * g_vLightDiffuse * (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient));

    vector vReflect = reflect(normalize(vLightDir), vNormal);

    vector vLook = vWorldPos - g_vCamPosition;

    Out.vSpecular = fAtt * (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 20.f);

    return Out;
}

sampler PointClampSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = clamp;
    AddressV = clamp;
};

float PCF_ShadowCalculation(float4 fragPosLightSpace/*, float3 lightDir*/)
{
	// perform perspective divide
    float3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// Transform to [0,1] range
    projCoords.x = projCoords.x * 0.5f + 0.5f;
    projCoords.y = projCoords.y * -0.5f + 0.5f;
    float currentDepth = projCoords.z;
    if (currentDepth > 1.0f)
        return 1.0f;
    
	// PCF
    float shadow = 0.0f;
    float2 texelSize = float2(1.f / 1440.f, 1.f / 810.f);
    texelSize /= 3.f;
    
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = g_ShadowDepthTarget.Sample(PointSampler, projCoords.xy + float2(x, y) * texelSize).r;
            shadow += fragPosLightSpace.w > pcfDepth * 2000.f ? 0.5f : 1.0f;
        }
    }
    //shadow *= 0.04f;
    shadow /= 9.f;
    return shadow;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
    float fBias = 0.0001f;
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTarget.Sample(PointSampler, In.vTexcoord);
    if (vDiffuse.a == 0.f)
    {
        vector vPriority = g_PriorityTarget.Sample(LinearSampler, In.vTexcoord);
        Out.vColor = vPriority;
        return Out;
    }
    vector vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord);
    
    vector vBlue = g_BlueTarget.Sample(PointSampler, In.vTexcoord);
    vector vDepth = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
    
    if (vBlue.b > vDepth.y)
    {
        Out.vColor = vector(0.f, 1.f, 1.f, 1.f);
        return Out;
    }
    
    vector vWorldPos;
    
    /* 투영스페이스 상의 위치를 구한다. */        //vShadowDepth = input.lightViewPosition.w / 2000.f
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f; // In.vTexcoord.x = vWorldPos.x / 2.f + 0.5f; // vWorldPos.x = vPosition.x / vPosition.w
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.f;

	/* 뷰스페이스 상의 위치를 구한다. */
    float fViewZ = (vDepth.y - fBias) * 2000.f;
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	/* 광원기준의 뷰랑 투영행렬을 곱한다. */
    vector vPosition = mul(vWorldPos, g_LightViewMatrix);
    vPosition = mul(vPosition, g_LightProjMatrix);
    //
    float shadow = PCF_ShadowCalculation(vPosition);
    
    Out.vColor = (vDiffuse * vShade) * float4(shadow, shadow, shadow, 1.f) /* + vSpecular*/;
    
    return Out;
}
 
PS_REFRACTION_OUT PS_MAIN_REFRACTION(PS_IN In)
{
    PS_REFRACTION_OUT Out = (PS_REFRACTION_OUT) 0;

    vector vDiffuse = g_DiffuseTarget.Sample(PointSampler, In.vTexcoord);
    if (vDiffuse.a == 0.f)
        discard;
    vector vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vDiffuse * vShade/* + vSpecular*/;

    return Out;
}

PS_REFLECTION_OUT PS_MAIN_REFLECTION(PS_IN In)
{
    PS_REFLECTION_OUT Out = (PS_REFLECTION_OUT) 0;

    vector vDiffuse = g_DiffuseTarget.Sample(PointSampler, In.vTexcoord);
    if (vDiffuse.a == 0.f)
        discard;
    vector vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vDiffuse * vShade/* + vSpecular*/;

    return Out;
}

// 이거 안씀 //패스 5번도 다른걸로 채우든지 해야함.
PS_OUT PS_MAIN_SCENE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vScene = g_SceneTarget.Sample(PointSampler, In.vTexcoord);
    vector vBlur = g_BlurHVTarget.Sample(LinearSampler, In.vTexcoord);
    vector vGodRay = g_GodRayTarget.Sample(LinearSampler, In.vTexcoord);
  
    Out.vColor = vScene + vBlur + vGodRay;

    return Out;
}

PS_OUT PS_MAIN_NONBLENDFINAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //float4 vDistortion = g_DistortionTarget.Sample(LinearSampler, In.vTexcoord);
    //float2 vDistort = vDistortion.xy;
    float4 vScene = g_SceneTarget.Sample(LinearSampler, In.vTexcoord);
    if (vScene.a == 0.f)
        discard;
    
    float4 vBlur = g_BlurHVTarget.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vScene + vBlur;
    
    return Out;
}

PS_OUT PS_MAIN_BLENDFINAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //float4 vDistortion = g_DistortionTarget.Sample(LinearSampler, In.vTexcoord);
    //float2 vDistort = vDistortion.xy;
    float4 vNonBlend = g_SceneTarget.Sample(PointSampler, In.vTexcoord);
    
    float4 vBlend = g_BlendTarget.Sample(PointSampler, In.vTexcoord);
    
    Out.vColor = vNonBlend + vBlend;
    
    return Out;
}

technique11 DefaultTechnique
{
	pass Target_Debug   // 0
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
        ComputeShader = NULL;
    }

    pass Light_Directional // 1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
        ComputeShader = NULL;
    }

    pass Light_Point // 2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
        ComputeShader = NULL;
    }

    pass Deferred // 3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
        ComputeShader = NULL;
    }

    //pass Scene  // 4
    //{
    //    SetRasterizerState(RS_Default);
    //    SetDepthStencilState(DSS_None, 0);
    //    SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

    //    VertexShader = compile vs_5_0 VS_MAIN();
    //    GeometryShader = NULL;
    //    HullShader = NULL;
    //    DomainShader = NULL;
    //    PixelShader = compile ps_5_0 PS_MAIN_SCENE();
    //    ComputeShader = NULL;
    //}

    pass NonBlendFinal  // 4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONBLENDFINAL();
        ComputeShader = NULL;
    }

    pass ShadowMap // 5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SCENE();
        ComputeShader = NULL;
    }

    pass Refraction // 6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_REFRACTION();
        ComputeShader = NULL;
    }

    pass Reflection // 7
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_REFLECTION();
        ComputeShader = NULL;
    }

    pass BlendFinal // 8
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        //SetRasterizerState(RS_Default);
        //SetDepthStencilState(DSS_None, 0);
        //SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLENDFINAL();
        ComputeShader = NULL;
    }
}




