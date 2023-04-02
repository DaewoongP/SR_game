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
	m_pTransform->m_vInfo[INFO_POS] = _vec3(15.f, 10.f, 10.f);
	return S_OK;
}
_int CPlayer02::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}
void CPlayer02::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPlayer02::Render_GameObject(void)
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

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Player_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Rigidbody", pComponent });

	m_pRigid->m_bUseGrivaty = false;	

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 1.f,2.f,0.2f });
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
	_vec3		vDir;
	_vec3		vRight;
	m_pTransform->Get_Info(INFO_LOOK, &vDir);
	m_pTransform->Get_Info(INFO_RIGHT, &vRight);



	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS)
		m_pRigid->m_Velocity.x = -m_fSpeed;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS)
		m_pRigid->m_Velocity.x = m_fSpeed;

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYUP)
		m_pRigid->m_Velocity.x = -m_fSpeed*0.f;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYUP)
		m_pRigid->m_Velocity.x = m_fSpeed*0.f;

	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYPRESS)
		m_pRigid->m_Velocity.y = +m_fSpeed;

	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYPRESS)
		m_pRigid->m_Velocity.y =-m_fSpeed;

	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYUP)
		m_pRigid->m_Velocity.y = +m_fSpeed*0.f;

	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYUP)
		m_pRigid->m_Velocity.y =-m_fSpeed*0.f;
}