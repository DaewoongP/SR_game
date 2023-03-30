#include "stdafx.h"
#include "ProtoMgr.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr(void)
{
}

CProtoMgr::~CProtoMgr(void)
{
	Free();
}

HRESULT CProtoMgr::Ready_Proto(const _tchar * pProtoTag, CComponent * pComponent)
{
	CComponent*		pPrototype = Find_Proto(pProtoTag);

	if (nullptr != pPrototype || nullptr == pComponent)
		return E_FAIL;

	m_uMapProto.insert({ pProtoTag, pComponent });

	return S_OK;
}

CComponent * CProtoMgr::Clone_Proto(const _tchar * pProtoTag, CGameObject* pGameObject)
{
	CComponent*		pPrototype = Find_Proto(pProtoTag);
	NULL_CHECK_RETURN(pPrototype, nullptr);
	
	CComponent* pClone = pPrototype->Clone();
	pClone->SetOwner(pGameObject);

	return pClone;
}

CComponent * CProtoMgr::Find_Proto(const _tchar * pProtoTag)
{
	auto	iter = find_if(m_uMapProto.begin(), m_uMapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_uMapProto.end())
		return nullptr;

	return iter->second;
}

void CProtoMgr::Free(void)
{
	for_each(m_uMapProto.begin(), m_uMapProto.end(), CDeleteMap());
	m_uMapProto.clear();
}
