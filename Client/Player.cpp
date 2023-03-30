#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = _vec3(10.f, 7.f, 0.f);
	//m_pTransform->m_vAngle.x += D3DXToRadian(180.f);

	_matrix projMatrix;
	D3DXMatrixPerspectiveFovLH(&projMatrix, D3DXToRadian(60.f), (float)WINCX/WINCY, 1.f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &projMatrix);

	return S_OK;
}
_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	// m_planeVec
	__super::Update_GameObject(fTimeDelta);

	_matrix viewMatrix;
	_vec3 myPos, cameraPos, up;

	up = { 0.f,1.f,0.f };

	//m_pTransform->m_vInfo[INFO_POS].y -= 1.f * fTimeDelta;

	myPos = m_pTransform->m_vInfo[INFO_POS];
	cameraPos = { myPos.x ,myPos.y + 7.f,myPos.z - 7.f };
	D3DXMatrixLookAtLH(&viewMatrix, &cameraPos, &myPos, &up);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &viewMatrix);


	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}
void CPlayer::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();

	CTerrainTex* terrainTex = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"TerrainTex", ID_STATIC));
	NULL_CHECK_MSG(terrainTex, L"터레인 텍스 널..");

	const vector<D3DXPLANE>& PlaneVec = terrainTex->m_PlaneVec;

	for (int i = 0; i < PlaneVec.size(); i++)
	{
		_vec3 playerFootPos = m_pTransform->m_vInfo[INFO_POS] - _vec3(0.f, 0.7f, 0.f);
		//_vec3 playerFootPos = m_pTransform->m_vInfo[INFO_POS];

		float fDot = D3DXPlaneDotCoord(&PlaneVec[i], &playerFootPos);
		bool bIsIn = terrainTex->IsInPlane(m_pTransform->m_vInfo[INFO_POS], i);
		
		if (fDot <  0.f && bIsIn)
		{
			m_pTransform->m_vInfo[INFO_POS].y -= fDot ;
			int i = 0;
			// collision detected
			// take appropriate action (e.g. adjust player's position or velocity)
		}
	}
	
	// 충돌 처리 부분.
}

void CPlayer::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);*/

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__super::Render_GameObject();
}

void CPlayer::OnTriggerStay(const CCollider * other)
{
	static int i = 0;
	cout << "충돌 테스트 플레이어" << ++i <<endl;
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex",this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });
		
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Player_Texture",this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Player_Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Player_Collider", pComponent });
	m_pCollider->Set_BoundingBox({3.f,3.f,3.f});
	return S_OK;
}



CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*		pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free(void)
{
	__super::Free();
}

void CPlayer::Key_Input(const _float & fTimeDelta)
{
	_vec3		vUp;
	_vec3		vRight;
	m_pTransform->Get_Info(INFO_UP, &vUp);
	m_pTransform->Get_Info(INFO_RIGHT, &vRight);

	if (GetAsyncKeyState(VK_UP))	m_pTransform->Move_Pos(&vUp, fTimeDelta, m_fSpeed);
	if (GetAsyncKeyState(VK_DOWN))	m_pTransform->Move_Pos(&vUp, fTimeDelta, -m_fSpeed);
	if (GetAsyncKeyState(VK_LEFT))	m_pTransform->Move_Pos(&vRight, fTimeDelta, -m_fSpeed);
	if (GetAsyncKeyState(VK_RIGHT))	m_pTransform->Move_Pos(&vRight, fTimeDelta, m_fSpeed);
	
}