
/* ������̺�. */
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector			g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector			g_vLightPos = vector(0.f, 0.f, 0.f, 1.f);
float			g_fLightRange = 0.f; 
vector			g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector			g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector			g_vCamPosition;

//Texture2D		g_DiffuseTexture[2];
Texture2D		g_DiffuseTexture;
Texture2D		g_MaskTexture;

Texture2D		g_BrushTexture;

vector			g_vBrushPos = vector(35.f, 0.f, 35.f, 1.f);
float			g_fBrushRange = 15.0f;



sampler LinearSampler = sampler_state {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state {
	Filter = MIN_MAG_MIP_POINT;
	AddressU = wrap;
	AddressV = wrap;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	/* float4 : w���� �ݵ�� ���ܾ��ϴ� ������ w�� �佺���̽� ���� ����(z)�� �����ϱ� ���ؼ���. */
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};


/* ���ؽ��� ���� ��ȯ�۾��� ��ģ��.  */
/* ��ȯ�۾� : ������ ��ġ�� ����, ��, ��������� ���Ѵ�. and �ʿ��� ��ȯ�� ���ؼ� �����Ӱ� �߰��ص� ������� .*/
/* ���ؽ��� ���� ������ �����Ѵ�. */
VS_OUT VS_MAIN(/* ���� */VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	/* mul : ���(���ϱⰡ ������) ����� ���ϱ⸦ �����Ѵ�. */
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;
	Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	return Out;	
}

/* w������ ����. ������ ������ȯ. */
/* ����Ʈ�����̽�(��������ǥ)�� ��ġ�� ��ȯ�Ѵ�. */
/* �����Ͷ����� : ������ �ѷ����� �ȼ��� ������ �����Ѵ�. */
/* �ȼ������� ���������� ����Ѵ�. */

struct PS_IN
{
	/* Viewport */
	float4		vPosition : SV_POSITION;
	float4		vNormal	: NORMAL;
	float2		vTexcoord : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};

/* �޾ƿ� �ȼ��� ������ �������� �Ͽ� ȭ�鿡 �׷��� �ȼ��� �������� ���� �����ϳ�. */
struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

//float4 Compute_TerrainPixelColor(PS_IN In)
//{
//	vector	vSourDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexcoord * 30.f);
//	vector	vDestDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexcoord * 30.f);
//	vector	vMaskColor = g_MaskTexture.Sample(LinearSampler, In.vTexcoord);
//	vector	vBrushColor = vector(0.f, 0.f, 0.f, 1.f);

//	if (g_vBrushPos.x - g_fBrushRange <= In.vWorldPos.x && In.vWorldPos.x < g_vBrushPos.x + g_fBrushRange &&
//		g_vBrushPos.z - g_fBrushRange <= In.vWorldPos.z && In.vWorldPos.z < g_vBrushPos.z + g_fBrushRange)
//	{
//		float2	vUV;

//		vUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
//		vUV.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

//		vBrushColor = g_BrushTexture.Sample(LinearSampler, vUV);
//	}

//	return vDestDiffuse * vMaskColor + vSourDiffuse * (1.f - vMaskColor) + vBrushColor;
//}

/* ���޹��� �ȼ��� ������ �̿��Ͽ� �ȼ��� �������� ���� ��������. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	//vector	vMtrlDiffuse = Compute_TerrainPixelColor(In);
    //vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord * 30.f);
    vector vMtrlDiffuse = vector(1.f, 0.f, 0.f, 1.f);

	vector	vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f) +
		g_vLightAmbient * g_vMtrlAmbient;		

	vector	vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
	vector	vLook = In.vWorldPos - g_vCamPosition;

	float	fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(vShade) +
		(g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

	return Out;
}

technique11 DefaultTechnique
{
	/* */
	pass Mesh
	{
		/* ���� ���̴��� ���ؼ� ���� � �������� �����ϰ� � �Լ��� ȣ���Ͽ� �ش� ���̴��� �����Ǵ����� �����Ѵ�. */
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}	
}




