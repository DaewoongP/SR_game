#include "stdafx.h"
#include "Boss2Parts.h"

CBoss2Parts::CBoss2Parts(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	m_dwTimer = 0;
	m_UseAnim = false;
}

CBoss2Parts::~CBoss2Parts()
{
}

HRESULT CBoss2Parts::Ready_GameObject(_vec3 & vPos)
{
	return S_OK;
}

_int CBoss2Parts::Update_GameObject(const _float & fTimeDelta)
{
	m_dwTimer -= fTimeDelta;

	__super::Update_GameObject(fTimeDelta);

	//애니메이션 사용하도록 세팅된 친구라면? 애니메이션 재생
	if (m_UseAnim)
		m_pTextureCom->m_bUseFrameAnimation = true;
	else
		m_CurrentTexture = m_OriginTexture;

	//타이머가 0이라면 이거 수행하지마.
	if (m_dwTimer < 0)
		return 0;
	//
	SetTexture_Blink((((int)(m_dwTimer * 10) / 4) % 2 == 0));

	return 0;
}

_int CBoss2Parts::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Parts::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Parts::LateUpdate_GameObject(void)
{
}

void CBoss2Parts::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CBoss2Parts::Render_Too(void)
{
}

void CBoss2Parts::Render_Top(void)
{
}

HRESULT CBoss2Parts::Add_Component(void)
{
	return S_OK;
}

CBoss2Parts * CBoss2Parts::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Parts*		pInstance = new CBoss2Parts(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Parts::Free(void)
{
	__super::Free();
}
