#include "stdafx.h"
#include "Produce1BackCode.h"


CProduce1BackCode::CProduce1BackCode(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CProduceObject(pGraphicDev)
{
}

CProduce1BackCode::~CProduce1BackCode()
{
}

HRESULT CProduce1BackCode::Ready_GameObject(void)
{
	return S_OK;
}

_int CProduce1BackCode::Update_GameObject(const _float & fTimeDelta)
{
	return 0;
}

void CProduce1BackCode::Render_GameObject(void)
{
	
}

CProduce1BackCode * CProduce1BackCode::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProduce1BackCode*		pInstance = new CProduce1BackCode(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CProduce1BackCode::Add_Component()
{
	return E_NOTIMPL;
}

void CProduce1BackCode::Free()
{
	CGameObject::Free();
}
