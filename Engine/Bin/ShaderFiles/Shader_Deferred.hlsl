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
vector  g_vLightEmissive = vector(1.f, 1.f, 1.f, 1.f);

vector  g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector  g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

Texture2D	g_NormalTarget;
Texture2D	g_DiffuseTarget;
Texture2D	g_DepthTarget;
Texture2D	g_BlueTarget;

Texture2D	g_ShadeTarget;
Texture2D	g_SpecularTarget;
Texture2D	g_EmissiveTarget;

Texture2D	g_GlowTarget;
Texture2D	g_BlurHTarget;
Texture2D	g_BlurHVTarget;

Texture2D	g_DistortionTarget;

Texture2D	g_SceneTarget;

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

struct VS_OUT_BLUR
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord0 : TEXCOORD0;
    float2 vTexcoord1 : TEXCOORD1;
    float2 vTexcoord2 : TEXCOORD2;
    float2 vTexcoord3 : TEXCOORD3;
    float2 vTexcoord4 : TEXCOORD4;
    float2 vTexcoord5 : TEXCOORD5;
    float2 vTexcoord6 : TEXCOORD6;
    float2 vTexcoord7 : TEXCOORD7;
    float2 vTexcoord8 : TEXCOORD8;
    float2 vTexcoord9 : TEXCOORD9;
    float2 vTexcoord10 : TEXCOORD10;
    float2 vTexcoord11 : TEXCOORD11;
    float2 vTexcoord12 : TEXCOORD12;
    float2 vTexcoord13 : TEXCOORD13;
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

VS_OUT_BLUR VS_MAIN_BLURH( /* 정점 */VS_IN In)
{
    VS_OUT_BLUR Out = (VS_OUT_BLUR) 0;
	
    float texelSize;
    
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

    Out.vTexcoord0 = In.vTexcoord;

    texelSize = 1.0f / 1440.f;
    
    Out.vTexcoord1 = In.vTexcoord + float2(texelSize * -6.0f, 0.0f);
    Out.vTexcoord2 = In.vTexcoord + float2(texelSize * -5.0f, 0.0f);
    Out.vTexcoord3 = In.vTexcoord + float2(texelSize * -4.0f, 0.0f);
    Out.vTexcoord4 = In.vTexcoord + float2(texelSize * -3.0f, 0.0f);
    Out.vTexcoord5 = In.vTexcoord + float2(texelSize * -2.0f, 0.0f);
    Out.vTexcoord6 = In.vTexcoord + float2(texelSize * -1.0f, 0.0f);
    Out.vTexcoord7 = In.vTexcoord;
    Out.vTexcoord8 = In.vTexcoord + float2(texelSize * 1.0f, 0.0f);
    Out.vTexcoord9 = In.vTexcoord + float2(texelSize * 2.0f, 0.0f);
    Out.vTexcoord10 = In.vTexcoord + float2(texelSize * 3.0f, 0.0f);
    Out.vTexcoord11 = In.vTexcoord + float2(texelSize * 4.0f, 0.0f);
    Out.vTexcoord12 = In.vTexcoord + float2(texelSize * 5.0f, 0.0f);
    Out.vTexcoord13 = In.vTexcoord + float2(texelSize * 6.0f, 0.0f);
    
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

    Out.vTexcoord0 = In.vTexcoord;

    texelSize = 1.0f / 810.f;
    
    Out.vTexcoord1 = In.vTexcoord + float2(0.0f, texelSize * -6.0f);
    Out.vTexcoord2 = In.vTexcoord + float2(0.0f, texelSize * -5.0f);
    Out.vTexcoord3 = In.vTexcoord + float2(0.0f, texelSize * -4.0f);
    Out.vTexcoord4 = In.vTexcoord + float2(0.0f, texelSize * -3.0f);
    Out.vTexcoord5 = In.vTexcoord + float2(0.0f, texelSize * -2.0f);
    Out.vTexcoord6 = In.vTexcoord + float2(0.0f, texelSize * -1.0f);
    Out.vTexcoord7 = In.vTexcoord;
    Out.vTexcoord8 = In.vTexcoord + float2(0.0f, texelSize * 1.0f);
    Out.vTexcoord9 = In.vTexcoord + float2(0.0f, texelSize * 2.0f);
    Out.vTexcoord10 = In.vTexcoord + float2(0.0f, texelSize * 3.0f);
    Out.vTexcoord11 = In.vTexcoord + float2(0.0f, texelSize * 4.0f);
    Out.vTexcoord12 = In.vTexcoord + float2(0.0f, texelSize * 5.0f);
    Out.vTexcoord13 = In.vTexcoord + float2(0.0f, texelSize * 6.0f);
    
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
    if (currentDepth > 1.0)
        return 1.0;
    
	// PCF
    float shadow = 0.0;
    float2 texelSize = float2(1.f / 1440.f, 1.f / 810.f);
    texelSize /= 8.f;
    
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = g_ShadowDepthTarget.Sample(PointSampler, projCoords.xy + float2(x, y) * texelSize).r;
            shadow += fragPosLightSpace.w > pcfDepth * 2000.f ? 0.5f : 1.0f;
        }
    }
    shadow /= 9.0;
    return shadow;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
    float fBias = 0.0001f;
    PS_OUT Out = (PS_OUT) 0;

    vector vDiffuse = g_DiffuseTarget.Sample(PointSampler, In.vTexcoord);
    if (vDiffuse.a == 0.f)
        discard;
    vector vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord);
    //vector		vSpecular = g_SpecularTarget.Sample(LinearSampler, In.vTexcoord);
    
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
    //
    //vector		vSpecular = g_SpecularTarget.Sample(LinearSampler, In.vTexcoord);
    
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
    //
    //vector		vSpecular = g_SpecularTarget.Sample(LinearSampler, In.vTexcoord);
    
    Out.vColor = vDiffuse * vShade/* + vSpecular*/;

    return Out;
}

struct PS_IN_BLUR
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord0 : TEXCOORD0;
    float2 vTexcoord1 : TEXCOORD1;
    float2 vTexcoord2 : TEXCOORD2;
    float2 vTexcoord3 : TEXCOORD3;
    float2 vTexcoord4 : TEXCOORD4;
    float2 vTexcoord5 : TEXCOORD5;
    float2 vTexcoord6 : TEXCOORD6;
    float2 vTexcoord7 : TEXCOORD7;
    float2 vTexcoord8 : TEXCOORD8;
    float2 vTexcoord9 : TEXCOORD9;
    float2 vTexcoord10 : TEXCOORD10;
    float2 vTexcoord11 : TEXCOORD11;
    float2 vTexcoord12 : TEXCOORD12;
    float2 vTexcoord13 : TEXCOORD13;
};

struct PS_OUT_BLURH
{
    float4 vColor : SV_TARGET0;
};

PS_OUT_BLURH PS_MAIN_BLURH(PS_IN_BLUR In)
{
    PS_OUT_BLURH Out = (PS_OUT_BLURH) 0;

    float weight0, weight1, weight2, weight3, weight4, weight5, weight6;
    float normalization;
    float4 color;
    
    weight0 = 1.0f;
    weight1 = 0.9231f;
    weight2 = 0.7261f;
    weight3 = 0.4868f;
    weight4 = 0.278f;
    weight5 = 0.1353f;
    weight6 = 0.0561f;
    //weight0 = 1.0f;
    //weight1 = 0.9f;
    //weight2 = 0.55f;
    //weight3 = 0.18f;
    //weight4 = 0.1f;
    
    normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4/* + weight5 + weight6*/));
    
    weight0 = weight0 / normalization;
    weight1 = weight1 / normalization;
    weight2 = weight2 / normalization;
    weight3 = weight3 / normalization;
    weight4 = weight4 / normalization;
    weight5 = weight5 / normalization;
    weight6 = weight6 / normalization;
    
    color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord1) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord1) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord1)) * weight6;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord2) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord2) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord2)) * weight5;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord3) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord3) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord3)) * weight4;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord4) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord4) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord4)) * weight3;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord5) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord5) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord5)) * weight2;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord6) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord6) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord6)) * weight1;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord7) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord7) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord7)) * weight0;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord8) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord8) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord8)) * weight1;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord9) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord9) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord9)) * weight2;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord10) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord10) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord10)) * weight3;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord11) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord11) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord11)) * weight4;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord12) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord12) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord12)) * weight5;
    color += (g_GlowTarget.Sample(LinearSampler, In.vTexcoord13) + g_SpecularTarget.Sample(LinearSampler, In.vTexcoord13) + g_vLightEmissive * g_EmissiveTarget.Sample(LinearSampler, In.vTexcoord13)) * weight6;
    
    color.a = 1.0f;
    
    Out.vColor = color;
    
    return Out;
}

struct PS_OUT_BLURV
{
    float4 vColor : SV_TARGET0;
};

PS_OUT_BLURV PS_MAIN_BLURV(PS_IN_BLUR In)
{
    PS_OUT_BLURV Out = (PS_OUT_BLURV) 0;

    float weight0, weight1, weight2, weight3, weight4, weight5, weight6;
    float normalization;
    float4 color;
    
    weight0 = 1.0f;
    weight1 = 0.9231f;
    weight2 = 0.7261f;
    weight3 = 0.4868f;
    weight4 = 0.278f;
    weight5 = 0.1353f;
    weight6 = 0.0561f;
    //weight0 = 1.0f;
    //weight1 = 0.9f;
    //weight2 = 0.55f;
    //weight3 = 0.18f;
    //weight4 = 0.1f;
    
    normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4 + weight5 + weight6));
    
    weight0 = weight0 / normalization;
    weight1 = weight1 / normalization;
    weight2 = weight2 / normalization;
    weight3 = weight3 / normalization;
    weight4 = weight4 / normalization;
    weight5 = weight5 / normalization;
    weight6 = weight6 / normalization;
    
    color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord1) * weight6;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord2) * weight5;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord3) * weight4;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord4) * weight3;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord5) * weight2;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord6) * weight1;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord7) * weight0;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord8) * weight1;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord9) * weight2;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord10) * weight3;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord11) * weight4;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord12) * weight5;
    color += g_BlurHTarget.Sample(LinearSampler, In.vTexcoord13) * weight6;
    
    color.a = 1.0f;
    
    Out.vColor = color;
    
    return Out;
}

PS_OUT PS_MAIN_SCENE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vScene = g_SceneTarget.Sample(PointSampler, In.vTexcoord);
    if (vScene.a == 0.f)
        discard;
    vector vBlur = g_BlurHVTarget.Sample(LinearSampler, In.vTexcoord);

    Out.vColor = vScene + vBlur;

    return Out;
}

/////////

struct VS_IN_SHADOW
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vNormal : NORMAL;
};

struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vNormal : NORMAL;
    float4 vLightViewPosition : TEXCOORD1;
    float3 vLightPos : TEXCOORD2;
};

struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vNormal : NORMAL;
    float4 vLightViewPosition : TEXCOORD1;
    float3 vLightPos : TEXCOORD2;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN_SHADOW In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;
    
    float4 vWorldPos;
    
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

    Out.vTexcoord = In.vTexcoord;
    
    matrix matWVLight, matWVPLight;
    
    matWVLight = mul(g_WorldMatrix, g_ViewMatrix);
    matWVPLight = mul(matWV, g_ProjMatrix);
    
    Out.vLightViewPosition = mul(float4(In.vPosition, 1.f), matWVPLight);

    // 픽셀 셰이더의 텍스쳐 좌표를 저장합니다.
    Out.vTexcoord = In.vTexcoord;
    
    // 노말 벡터를 월드 행렬에만 곱합니다.
    Out.vNormal = mul(In.vNormal, (float3x3) g_WorldMatrix);
    
    // 노말 벡터를 정규화합니다.
    Out.vNormal = normalize(Out.vNormal);
    
    vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    // 조명의 위치에서의 정점의 위치를 조명의 위치와 정점의 월드 위치를 이용하여 계산합니다.
    Out.vLightPos = g_vLightPos.xyz - vWorldPos.xyz;

    // 조명 위치 벡터를 정규화합니다.
    Out.vLightPos = normalize(Out.vLightPos);

    return Out;
}

technique11 DefaultTechnique
{
	pass Target_Debug
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

	pass Light_Directional
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

	pass Light_Point
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

	pass Deferred
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

    pass Horizontal_Blur
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

    pass Vertical_Blur // 5
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

    pass Scene  // 6
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

    pass ShadowMap // 7
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

    pass Refraction // 8
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

    pass Reflection // 9
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
}




