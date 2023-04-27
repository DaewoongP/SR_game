#include "stdafx.h"
#include "ProduceObject.h"

CProduceObject::CProduceObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CProduceObject::~CProduceObject()
{
}

void CProduceObject::Free(void)
{
	CGameObject::Free();
}
