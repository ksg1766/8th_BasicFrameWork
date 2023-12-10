#include "stdafx.h"
#include "..\Public\MainApp.h"
#include <filesystem>

#include "GameInstance.h"
#include "DissolveManager.h"
#include "Level_Loading.h"
#include "FlyingCameraController.h"
#include "DebugTerrainGrid.h"

CMainApp::CMainApp()	
	: m_pGameInstance(CGameInstance::GetInstance())
{	
	// D3D11_SAMPLER_DESC

	Safe_AddRef(m_pGameInstance);

	/*
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENBALE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	*/

	/* 
	_float4		vSrcColor(���� ����ۿ� �׸������ϴ� �ȼ��� ��), vDestColor(�̹� ����ۿ� �׷����ִ� ��);

	_float4		vResult =
		(vSrcColor * vSrcColor.a) + (vDestColor * (1.f - vSrcColor.a));
		*/
}


HRESULT CMainApp::Initialize()
{
	/* 1. �� ������ �ʱ�ȭ�� �����Ҳ���. */
	/* 1-1. �׷�����ġ�� �ʱ�ȭ�Ѵ�. */
	/* 1-2. ������ġ�� �ʱ�ȭ�Ѵ�. */
	/* 1-3. �Է���ġ�� �ʱ�ȭ�Ѵ�. */
	GRAPHIC_DESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.eWinMode = GRAPHIC_DESC::WINMODE_WIN;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext, g_hInstance)))
		return E_FAIL;

	if (FAILED(Reserve_Client_Managers()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Scripts()))
		return E_FAIL;

	/* 1-4. ���ӳ����� ����� ����(��)�� �����Ѵ�.   */
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	/* 1-4-1. ���ӳ����� ����� ���� �ڿ�(�ؽ���, ��, ��ü) ���� �غ��Ѵ�.  */

	return S_OK;
}

void CMainApp::Tick(const _float& fTimeDelta)
{
	/* ���ӳ��� �����ϴ� ���� ��ü���� ����. */
	/* ������ ���� */
	m_pGameInstance->Tick(fTimeDelta);
	
	// Temp
	CDissolveManager::GetInstance()->Tick_Dissolve(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	++m_iFps;

	if (m_dwTime + 1000 < GetTickCount())
	{
#ifdef _DEBUG
		swprintf_s(m_szFPS, L"FPS : %d - DEBUG", m_iFps);
#elif
		swprintf_s(m_szFPS, L"FPS : %d - RELEASE", m_iFps);
#endif
		SetWindowText(g_hWnd, m_szFPS);

		m_iFps = 0;
		m_dwTime = GetTickCount();
	}

	/* ���ӳ��� �����ϴ� ���� ��ü���� ������. */
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.9f, 0.9f, 0.9f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();
#ifdef _DEBUG
	//if(LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	//	m_pGameInstance->Render_QuadTree();
#endif
	m_pRenderer->Draw_RenderObjects();

	//m_pGameInstance->DebugRender();
	/* �ʱ�ȭ�� ��鿡 ��ü���� �׸���. */
	m_pGameInstance->Present();

	m_pGameInstance->FinalTick();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* �ΰ������� �Ҵ��ϰ� �;�����. �ΰ����� ���� �ε������� ���� �����Ͽ� �ε��۾��� �����Ҳ���. */
	/* �ε���ü���� eLevelID��� ���� ���� �Ҵ社�ư� �;��� ��������ü�� �ذ���?! �����Ҵ��ϰ�;��� ������ �ڿ��� �غ���϶�� */
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Reserve_Client_Managers()
{
	if (FAILED(CDissolveManager::GetInstance()->Reserve_Manager(m_pDevice)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), 
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VTFSocket */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTFSocket"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VTFSocket.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxTexFetchAnim */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexFetchAnim"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTexFetchAnim.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;
	
	/* For.Prototype_Component_Shader_VtxNonAnimInstancing */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnimInstancing"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNonAnimInstancing.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimInstancing */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimInstancing"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimInstancing.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Point_Instance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Point_Instance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Point_Instance.hlsl"), VTXPOINT::Elements, VTXPOINT::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Sun */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Sun"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Sun.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCubeTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::Elements, VTXCUBETEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCubeNom*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeNom"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeNom.hlsl"), VTXCUBENOM::Elements, VTXCUBENOM::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxDebug*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxDebug"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxDebug.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Shockwave*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Shockwave"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Shockwave.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Fire*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Fire"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Fire.hlsl"), VTXPOINT::Elements, VTXPOINT::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Lightning*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Lightning"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Lightning.hlsl"), VTXPOINT::Elements, VTXPOINT::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Sphere*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Sphere"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Sphere.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	
	/* For.Prototype_Component_Shader_SwordTrail*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_SwordTrail"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_SwordTrail.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_ComputeParticles*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ComputeParticles"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_ComputeParticles.hlsl"), nullptr, 0))))
		return E_FAIL;
	
	/* For.Prototype_Component_Shader_ParticleSystem_Stream*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ParticleSystem_Stream"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_ParticleSystem_Stream.hlsl"), VTXPARTICLE::Elements, VTXPARTICLE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_ParticleSystem_Draw*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ParticleSystem_Draw"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_ParticleSystem_Draw.hlsl"), VTXPARTICLE::Elements, VTXPARTICLE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_ParticleSystem */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_ParticleSystem"),
		CVIBuffer_ParticleSystem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Grid */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Grid"),
		CVIBuffer_Grid::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"),
		CVIBuffer_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_VIBuffer_Dot */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Dot"),
		CVIBuffer_Dot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model */
	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model"),
		CModel::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/

	/* For.Prototype_Component_RigidStatic */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_RigidStatic"),
		CRigidStatic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_RigidDynamic */
	if(FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_RigidDynamic"),
		CRigidDynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	// �ϴ� ���⿡ �־��
	/* For.Prototype_Component_SphereCollider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SphereCollider"),
		CSphereCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_OBBCollider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_OBBCollider"),
		COBBCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_OBBCollider */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Camera"),
		CCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_SkyBox */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sunset.dds")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Lava_A_emissive */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lava_A_emissive"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Emissive/Lava_A_emissive0.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Lava_B_emissive */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lava_B_emissive"),
		//CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Emissive/Lava_B_erh.dds")))))
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Emissive/Lava_A_emissive1.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Noise_Liquid */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Noise_Liquid"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/Noise_Liquid.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TremorCrystal_A_emissive */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TremorCrystal_A_emissive"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Emissive/TremorCrystal_A_emissive.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TremorCrystal_B_emissive */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TremorCrystal_B_emissive"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Emissive/TremorCrystal_B_emissive.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Moloch_Sword_emissive */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Moloch_Sword_emissive"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Emissive/Moloch_Sword_emissive.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Dissolve */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Dissolve/DissolvePattern%d.dds"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Mask_Swipe*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mask_Swipe"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Mask/Mask_Swipe.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Noise_RGB*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Noise_RGB"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/Noise_RGB.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_TriangleMask*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TriangleMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Mask/TriangleMask.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_EggMask*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EggMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Mask/EggMask.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Smoke_Tiled*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Smoke_Tiled"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Fire/Smoke_Tiled.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Fire_Tiled*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fire_Tiled"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Fire/Fire_Tiled.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GeyserCrack*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GeyserCrack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Decal/GeyserCrack%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Lightning_Bolts*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lightning_Bolts"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Lightning/Lightning_Bolts%d.png"), 4))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Lightning_Spearhead*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lightning_Spearhead"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Lightning/Lightning_Spearhead%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Lightning_Spark*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lightning_Spark"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Lightning/Lightning_Spark%d.png"), 4))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Bolts*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bolts"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Lightning/Bolts%d.png"), 4))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Lightning_Bolts_Large*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lightning_Bolts_Large"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Lightning/Lightning_Bolts_Large%d.png"), 4))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Fire_SubUV*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fire_SubUV"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Fire/Fire_SubUV.png")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Fire_Stylized*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fire_Stylized"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Fire/Fire_Stylized.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Static */
	{
		wstring strStaticFilePath = TEXT("../Bin/Resources/Models/Static/");
		for (const auto& entry : filesystem::directory_iterator(strStaticFilePath))
		{
			Matrix matPivot = Matrix::Identity;
			const wstring& strFileName = entry.path().stem();
			SOCKETDESC desc = SOCKETDESC();

			if (TEXT("Strife_GunL") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(10.0f)) * XMMatrixRotationX(XMConvertToRadians(280.0f)) * XMMatrixTranslation(0.17f, -0.12f, -0.11f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strStaticFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("Strife_GunR") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(172.0f)) * XMMatrixRotationX(XMConvertToRadians(98.0f)) * XMMatrixTranslation(-0.15f, 0.f, 0.095f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strStaticFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("Moloch_Sword") == strFileName)
			{
				//XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(172.0f)) * XMMatrixRotationX(XMConvertToRadians(98.0f)) * XMMatrixTranslation(-0.15f, 0.f, 0.095f));
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationY(XMConvertToRadians(80.f)) * XMMatrixRotationX(XMConvertToRadians(270.0f)) * XMMatrixRotationY(XMConvertToRadians(270.f)) * XMMatrixRotationZ(XMConvertToRadians(330.0f))
					* XMMatrixTranslation(1.6f, 0.4f, -2.5f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strStaticFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("Sun") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.0003f, 0.0003f, 0.0003f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strStaticFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("Wisp") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.003f, 0.003f, 0.003f) * XMMatrixTranslation(0.0f, -0.5f, 0.0f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strStaticFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("SphereSwirl") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.537f, 0.537f, 0.537f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strStaticFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strStaticFilePath + strFileName))))
					return E_FAIL;
			}
		}
	}

	/* For.Prototype_Component_Model_Skeletal */
	{
		wstring strSkeletalFilePath = TEXT("../Bin/Resources/Models/Skeletal/");
		for (const auto& entry : filesystem::directory_iterator(strSkeletalFilePath))
		{
			const wstring& strFileName = entry.path().stem();

			SOCKETDESC desc = SOCKETDESC();
			Matrix matPivot = Matrix::Identity;
			XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.0f)));
			if (TEXT("P_Strife") == strFileName)
			{
				desc.vecSocketBoneNames.push_back("Bone_Strife_Weapon_Dagger_L_end");
				desc.vecSocketBoneNames.push_back("Bone_Strife_Weapon_Dagger_R_end");

				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strSkeletalFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("Moloch") == strFileName)
			{
				desc.vecSocketBoneNames.push_back("Bone_Moloch_Hand_R");

				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.01f, 0.01f, 0.01f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strSkeletalFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("Dagon") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.01f, 0.01f, 0.01f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strSkeletalFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("HellHound") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.01f, 0.01f, 0.01f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strSkeletalFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("HellBrute") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strSkeletalFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("Goblin") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strSkeletalFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("DemonCaster") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixTranslation(0.f, 0.5f, 0.f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strSkeletalFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else if (TEXT("DagonWave") == strFileName)
			{
				XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.00427f, 0.00427f, 0.00427f) * XMMatrixRotationY(XMConvertToRadians(270.0f)));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strSkeletalFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
			else
			{
				//XMStoreFloat4x4(&matPivot, XMMatrixScaling(0.01f, 0.01f, 0.01f));
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_" + strFileName),
					CModel::Create(m_pDevice, m_pContext, strSkeletalFilePath + strFileName, desc, matPivot))))
					return E_FAIL;
			}
		}
	}

	Safe_AddRef(m_pRenderer);
	
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Scripts()
{
	/* For.Prototype_Component_FlyingCameraController */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_FlyingCameraController"),
		CFlyingCameraController::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_DebugTerrainGrid */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_DebugTerrainGrid"),
		CDebugTerrainGrid::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{	
	__super::Free();

	Safe_Release(m_pRenderer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pGameInstance);

	CDissolveManager::GetInstance()->Free();

	CGameInstance::Release_Engine();
}
