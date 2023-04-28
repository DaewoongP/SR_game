#pragma once

#include "Base.h"
#include "Timer.h"
#include "Include.h"

BEGIN(Engine)

class  CTimerMgr : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr();

public:
	_float		Get_Timer(const _tchar* pTimerTag);
	void		Set_Timer(const _tchar* pTimerTag);

public:
	HRESULT		Ready_Timer(const _tchar* pTimerTag);
	
private:
	CTimer*		Find_Timer(const _tchar* pTimerTag);
	

private:
	unordered_map<const _tchar*, CTimer*>		m_umapTimers;
	
public:
	virtual	void Free(void)override;
};

// Add_Timer(핸들, 클래스포인터, 호출할함수, 실행간격, 시행횟수(0=무한))
// Stop_Timer(핸들)

END
