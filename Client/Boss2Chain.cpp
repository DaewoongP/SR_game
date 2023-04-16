#include "stdafx.h"
#include "Boss2Chain.h"

CBoss2Chain::CBoss2Chain(LPDIRECT3DDEVICE9 pGraphicDev):
	CBoss2Parts(pGraphicDev)
{
}

CBoss2Chain::~CBoss2Chain()
{
}

HRESULT CBoss2Chain::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = _vec3(1.0f, 1.0f, 1.0f);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_OriginTexture = 0;
	m_WhiteTexture = 1;
	m_CurrentTexture = m_OriginTexture;
	return S_OK;
}

_int CBoss2Chain::Update_GameObject(const _float & fTimeDelta)
{
	SetChain();
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CBoss2Chain::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Chain::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Chain::LateUpdate_GameObject(void)
{
}

void CBoss2Chain::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_CurrentTexture);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Chain::Render_Too(void)
{
}

void CBoss2Chain::Render_Top(void)
{
}

HRESULT CBoss2Chain::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Chain", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	return S_OK;
}

void CBoss2Chain::SetChain()
{
	//x������ = ����(����Ʈ 01(foot) - ����Ʈ02(����Ʈ))*0.5f << �ؽ��� ũ�Ⱑ 2,2�� 0.5 ����
	m_pTransform->m_vScale.x =
		D3DXVec3Length(&_vec3(m_pTrans_Joint01->m_vInfo[INFO_POS] - m_pTrans_Joint02->m_vInfo[INFO_POS])) * 0.5f;
	//lerp (����Ʈ 01, ����Ʈ 02 , 0.5f) << ����Ʈ 1 2�� ����
	m_pTransform->m_vInfo[INFO_POS] = Lerp(m_pTrans_Joint01->m_vInfo[INFO_POS], m_pTrans_Joint02->m_vInfo[INFO_POS],0.45f);

	//dir = ����Ʈ���� �Ÿ�
	_vec3 dir = m_pTrans_Joint01->m_vInfo[INFO_POS] - m_pTrans_Joint02->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&dir, &dir);
	_vec3 right;
	D3DXVec3Normalize(&right, &m_pTransform->m_vInfo[INFO_RIGHT]);
	//ȸ���� ���� ����
	_float dot = D3DXVec3Dot(&dir, &right);
	
	_float radian = acosf(dot);
	//�� ����
	if (m_pTrans_Joint01->m_vInfo[INFO_POS].y < m_pTrans_Joint02->m_vInfo[INFO_POS].y)
		radian = 2 * D3DX_PI- radian;

	m_pTransform->m_vAngle.z = radian;
}

CBoss2Chain * CBoss2Chain::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Chain*		pInstance = new CBoss2Chain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Chain::Free(void)
{
	__super::Free();
}
