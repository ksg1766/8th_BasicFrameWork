#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND		hWnd;
		WINMODE		eWinMode;
		_uint		iWinSizeX, iWinSizeY;

	}GRAPHIC_DESC;

	/*typedef struct tagMaterialDesc
	{
		_char		strName[MAX_PATH];

		Vec4		vAmbient;
		Vec4		vDiffuse;
		Vec4		vSpecular;
		Vec4		vEmissive;
		class CTexture*	pTextures[AI_TEXTURE_TYPE_MAX];
	}MATERIALDESC;*/

	// VTF
	// Animation
	typedef struct KeyframeDesc
	{
		int32 animIndex = -1;
		uint32 currFrame = 0;
		uint32 nextFrame = 0;
		float ratio = 0.f;
		float sumTime = 0.f;
		float speed = 1.f;
		Vec2 padding;
	}KEYFRAMEDESC;

	typedef struct TweenDesc
	{
		TweenDesc()
		{
			curr.animIndex = 0;
			next.animIndex = -1;
		}

		void ClearNextAnim()
		{
			next.animIndex = -1;
			next.currFrame = 0;
			next.nextFrame = 0;
			next.sumTime = 0;
			tweenSumTime = 0;
			tweenRatio = 0;
		}

		float tweenDuration = 0.1f;
		float tweenRatio = 0.f;
		float tweenSumTime = 0.f;
		float padding = 0.f;
		KeyframeDesc curr;
		KeyframeDesc next;
	}TWEENDESC;

	struct InstancingData
	{
		Matrix matWorld;
	};

	#define MAX_INSTANCE 100
	struct InstancedTweenDesc
	{
		TweenDesc tweens[MAX_INSTANCE];
	};
	//

	typedef struct tagMeshMaterial
	{
		class CTexture* pTextures[AI_TEXTURE_TYPE_MAX];
	}MESH_MATERIAL;

	typedef struct tagKeyFrame
	{
		float		fTime;

		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vPosition;

	}KEYFRAME;

	typedef struct tagFaceIndices16
	{
		_ushort		_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		_ulong		_0, _1, _2;
	}FACEINDICES32;


	typedef struct ENGINE_DLL tagVertex_Position
	{
		XMFLOAT3		vPosition;

		static const _uint				iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const _uint				iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const _uint				iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Cube_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexture;

		static const _uint				iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];

	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Cube_Texcoord_Normal
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTexture;

		static const _uint				iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];

	}VTXCUBENOM;

	typedef struct ENGINE_DLL tagVertex_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const _uint				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXMESH;

	//typedef struct ENGINE_DLL tagVertex_Anim_Mesh
	//{
	//	XMFLOAT3		vPosition;
	//	XMFLOAT3		vNormal;
	//	XMFLOAT2		vTexture;
	//	XMFLOAT3		vTangent;
	//	XMUINT4			vBlendIndex; /* �� ������ ������ �ִ� ���� �ε��� �װ�. */
	//	XMFLOAT4		vBlendWeight; /* ���⸣ �ְ� �ִ� �� ������ ���� ���� */

	//	static const _uint iNumElements = 6;
	//	static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	//}VTXANIMMESH;

	typedef struct ENGINE_DLL tagVertex_Anim_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexture;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex; /* �� ������ ������ �ִ� ���� �ε��� �װ�. */
		XMFLOAT4		vBlendWeight; /* ���⸣ �ְ� �ִ� �� ������ ���� ���� */
		// Instance
		_int			iInstanceID;
		XMFLOAT4X4		matWorld;

		static const _uint iNumElements = 10;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXANIMMESH;

	typedef struct tagCollision
	{
		class CRigidBody* pOther;
		// Vec3		vImpulse;
		 Vec3		vResultVelocity;
		//_float fTimeDelta;
	}COLLISION_DESC;
}


#endif // Engine_Struct_h__
