#include "Client_Shader_Defines.hlsl"
#include "Shader_Lighting_Functions.hlsl"

#define MAX_INSTANCE 500

/* 상수테이블. */
Texture2D		g_Texture;

vector g_vCamPosition;

/* For ParticleSystem */
float3 g_vEmitPosition;
float3 g_vEmitDirection;

float g_fGameTime;
float g_fTimeStep;

Texture1D g_RandomTexture;

// HELPER FUNCTIONS
float3 RandUnitVec3(float fOffset)
{
	// Use game time plus offset to sample random texture.
    float u = (g_fGameTime + fOffset);

	// coordinates in [-1,1]
    float3 v = g_RandomTexture.SampleLevel(LinearSampler, u, 0).xyz;

	// project onto unit sphere
    return normalize(v);
}

float3 RandVec3(float fOffset)
{
	// Use game time plus offset to sample random texture.
    float u = (g_fGameTime + fOffset);
    
	// coordinates in [-1,1]
    float3 v = g_RandomTexture.SampleLevel(LinearSampler, u, 0).xyz;

    return v;
}

// STREAM-OUT TECH
#define PT_EMITTER 0
#define PT_PARTICLE 1

struct GS_PARTICLE
{	
    float3  vPosition   : POSITION;
    float3  vVelocity   : VELOCITY;
    float2  vSize       : SIZE;
    float   fAge        : AGE;
    uint    iType       : TYPE;
};

GS_PARTICLE VS_MAIN(GS_PARTICLE In)
{
    return In;
}

[maxvertexcount(6)]
void GS_MAIN(point GS_PARTICLE In[1], inout PointStream<GS_PARTICLE> OutStream)
{
    In[0].fAge += g_fTimeStep;

    if (In[0].iType == PT_EMITTER)
    {
		// time to emit a new particle?
        if (In[0].fAge > 0.002f)
        {
            for (int i = 0; i < 5; ++i)
            {
				// Spread rain drops out above the camera.
                float3 vRandom = 35.0f * RandVec3((float) i / 5.0f);
                vRandom.y = 20.0f;

                GS_PARTICLE p;
                p.vPosition = g_vEmitPosition.xyz + vRandom;
                p.vVelocity = float3(0.0f, 0.0f, 0.0f);
                p.vSize = float2(1.0f, 1.0f);
                p.fAge = 0.0f;
                p.iType = PT_PARTICLE;

                OutStream.Append(p);
            }

			// reset the time to emit
            In[0].fAge = 0.0f;
        }

		// always keep emitters
        OutStream.Append(In[0]);
    }
    else
    {
		// Specify conditions to keep particle; this may vary from system to system.
        if (In[0].fAge <= 2.0f)
            OutStream.Append(In[0]);
    }
}

technique11 StreamOutTech
{
    pass P0
    {
		SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		/* 여러 셰이더에 대해서 각각 어떤 버젼으로 빌드하고 어떤 함수를 호출하여 해당 셰이더가 구동되는지를 설정한다. */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = ConstructGSWithSO(CompileShader(gs_5_0, GS_MAIN()), "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = NULL;
        ComputeShader = NULL;
    }
}




