#include "stdafx.h"
#include "TopdeeJoint.h"

CTopdeeJoint::CTopdeeJoint(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CTopdeeJoint::~CTopdeeJoint()
{
}

HRESULT CTopdeeJoint::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	return S_OK;
}

_int CTopdeeJoint::Update_GameObject(const _float & fTimeDelta)
{
//	_matrix		matWorld, matBill, matView;
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	//matWorld = *m_pTransform->Get_WorldMatrixPointer();
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//D3DXMatrixIdentity(&matBill);

	//matBill._22 = matView._22;
	//matBill._23 = matView._23;
	//matBill._32 = matView._32;
	//matBill._33 = matView._33;

	//D3DXMatrixInverse(&matBill, 0, &matBill);
	//matWorld = matBill * matWorld;
	//m_pTransform->m_matWorld = matWorld;

	return 0;
}

CTopdeeJoint * CTopdeeJoint::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CTopdeeJoint*		pInstance = new CTopdeeJoint(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTopdeeJoint::Add_Component(void)
{
	return S_OK;
}

void CTopdeeJoint::Free(void)
{
	__super::Free();
}
