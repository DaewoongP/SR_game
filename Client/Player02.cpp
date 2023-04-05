#include "stdafx.h"
#include "Player02.h"

#include "Export_Function.h"

CPlayer02::CPlayer02(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPlayer02::~CPlayer02()
{
}

HRESULT CPlayer02::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = _vec3(16.f, 10.f, 11.f);

	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];

	__super::Update_GameObject(0.01f);
	return S_OK;
}
_int CPlayer02::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}
_int CPlayer02::Update_Too(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);
	RAYCAST ray(m_pTransform->m_vInfo[INFO_POS], _vec3(1,0,0),3);
	list<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(ray);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}
_int CPlayer02::Update_Top(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);
	PlayerMove(fTimeDelta);
	RAYCAST ray(m_pTransform->m_vInfo[INFO_POS], _vec3(1, 0, 0), 3);
	list<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(ray);
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}
void CPlayer02::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPlayer02::LateUpdate_Too()
{
}

void CPlayer02::LateUpdate_Top()
{
}

void CPlayer02::Render_GameObject(void)
{
	
}

void CPlayer02::Render_Too()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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

void CPlayer02::Render_Top()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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

void CPlayer02::OnCollisionEnter(const Collision * collision)
{
	
	__super::OnCollisionEnter(collision);
}

void CPlayer02::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CPlayer02::OnCollisionExit(const Collision * collision)
{
}

HRESULT CPlayer02::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	m_pTransform->m_bIsStatic = false;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Player_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 0.99f,1.99f,0.2f });
	return S_OK;
}



CPlayer02* CPlayer02::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer02*		pInstance = new CPlayer02(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer02::Free(void)
{
	__super::Free();
}

void CPlayer02::Key_Input(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS ||
		Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS ||
		Engine::Get_DIKeyState(DIK_UP) == Engine::KEYPRESS ||
		Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYPRESS)
	{
		m_bIsMoving = true;
	}

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 8;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 4; 

	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 2;

	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 1;

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYUP)
		m_byPlayerInputDir ^= 8;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYUP)
		m_byPlayerInputDir ^= 4;

	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYUP)
		m_byPlayerInputDir ^= 2;

	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYUP)
		m_byPlayerInputDir ^= 1;
}

void CPlayer02::PlayerMove(const _float& fTimeDelta)
{
	if (m_bIsMoving)
		if (IsMoveDone(fTimeDelta))
			return;
		else
		{
			m_byPlayerMoveDir = m_byPlayerInputDir;

			if (prePos == m_pTransform->m_vInfo[INFO_POS])
			{
				m_MovetoPos = prePos;
			}
			int x = 0;
			int y = 0;

			if (m_byPlayerMoveDir & 1)
				y-=2;
			if (m_byPlayerMoveDir & 2)
				y+=2;
			if (m_byPlayerMoveDir & 4)
				x+=2;
			if (m_byPlayerMoveDir & 8)
				x-=2;

			m_MovetoPos = _vec3(m_pTransform->m_vInfo[INFO_POS].x + x, m_pTransform->m_vInfo[INFO_POS].y + y, m_pTransform->m_vInfo[INFO_POS].z);
			m_bIsMoving = false;
		}
}

//실제 움직이는 코드
_bool CPlayer02::IsMoveDone(const _float& fTimeDelta)
{
	_vec3 dir;
	D3DXVec3Normalize(&dir, &_vec3(m_MovetoPos - m_pTransform->m_vInfo[INFO_POS]));
	m_pTransform->m_vInfo[INFO_POS] += dir*m_fSpeed*fTimeDelta;
	
	if (prePos == m_pTransform->m_vInfo[INFO_POS])
	{
		m_MovetoPos = prePos;
	}
	prePos = m_pTransform->m_vInfo[INFO_POS];
	//거리 이용 도달했는지 알려주는 코드
	if (D3DXVec3Length(&_vec3(m_pTransform->m_vInfo[INFO_POS] - m_MovetoPos)) < 0.2f)
	{
		m_pTransform->m_vInfo[INFO_POS] = m_MovetoPos;
		return false;
	}
	return true;
}