#include "stdafx.h"
#include "Boss3EyePupil.h"
#include "Export_Function.h"

CBoss3EyePupil::CBoss3EyePupil(LPDIRECT3DDEVICE9 pGraphicDev)\
	:CGameObject(pGraphicDev)
{
	m_pBoss3 = nullptr;
}

CBoss3EyePupil::~CBoss3EyePupil()
{
}

HRESULT CBoss3EyePupil::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ vPos.x, vPos.y, vPos.z };
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(45.f));
	m_pTransform->m_bIsStatic = true;

	m_iIndex = iIndex;

	m_pBoss3 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3");

	return S_OK;
}

_int CBoss3EyePupil::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);


	//  1로 넣는 값이 z 변화, 2로 넣는 값이 y 변화, 3로 넣는 값이 x 변화,
	if (!lstrcmp(m_pTag, L"BossLeftPupil"))
		m_pTransform->Set_ParentTransform(m_pBoss3, -4.4f, 1.f, +2.5f);

	else if (!lstrcmp(m_pTag, L"BossRightPupil"))
		m_pTransform->Set_ParentTransform(m_pBoss3, -4.4f, 1.f, -2.5f);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBoss3EyePupil::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss3EyePupil::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iIndex);
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CBoss3EyePupil::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss3_Eye", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Boss3_Eye", pComponent });

	return S_OK;
}

CBoss3EyePupil * CBoss3EyePupil::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int iIndex)
{
	CBoss3EyePupil* pInstance = new CBoss3EyePupil(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3EyePupil::Free(void)
{
	__super::Free();
}
