#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_bJumpalbe(false)
{

}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = _vec3(10.f, 7.f, 10.f);
	return S_OK;
}
_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	_matrix viewMatrix;
	_vec3 myPos, cameraPos, up;

	up = { 0.f,1.f,0.f };

	//카메라 임시생성
	myPos = m_pTransform->m_vInfo[INFO_POS];
	cameraPos = { myPos.x ,myPos.y,-20.f };
	D3DXMatrixLookAtLH(&viewMatrix, &cameraPos, &myPos, &up);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &viewMatrix);

	_matrix projMatrix;
	D3DXMatrixPerspectiveFovLH(&projMatrix, D3DXToRadian(60.f), (float)WINCX / WINCY, 1.f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &projMatrix);

	Key_Input(0.01f);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	
	return 0;
}
void CPlayer::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPlayer::Render_GameObject(void)
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

void CPlayer::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CPlayer::OnCollisionStay(const Collision * collision)
{
	if (collision->_dir == DIR_DOWN)
		m_bJumpalbe = true;
	__super::OnCollisionStay(collision);
}

void CPlayer::OnCollisionExit(const Collision * collision)
{
	m_bJumpalbe = false;
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Player_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 1.f,2.f,0.2f });
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
	_vec3		vDir;
	_vec3		vRight;
	m_pTransform->Get_Info(INFO_LOOK, &vDir);
	m_pTransform->Get_Info(INFO_RIGHT, &vRight);

	if (Engine::Get_DIKeyState(DIK_LEFT) & 0x80)
		m_pRigid->m_Velocity.x = -m_fSpeed;

	if (Engine::Get_DIKeyState(DIK_RIGHT) & 0x80)
		m_pRigid->m_Velocity.x = m_fSpeed;

	if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80 && m_bJumpalbe)
	{
		m_pRigid->AddForce(_vec3(0, 1, 0), 25.f, IMPULSE, fTimeDelta);
	}
}