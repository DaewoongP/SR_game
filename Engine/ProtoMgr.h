#pragma once

#include "Include.h"
#include "Base.h"

BEGIN(Engine)

class  CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit CProtoMgr(void);
	virtual ~CProtoMgr(void);

public:
	HRESULT		Ready_Proto(const _tchar* pProtoTag, class CComponent* pComponent);
	class CComponent*	Clone_Proto(const _tchar * pProtoTag, class CGameObject* pGameObject);

private:
	class CComponent*	Find_Proto(const _tchar* pProtoTag);

private:
	unordered_map<const _tchar*, class CComponent*>			m_uMapProto;

private:
	virtual void Free(void);
};

END