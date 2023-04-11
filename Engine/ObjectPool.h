#pragma once
#include "Include.h"
#include "GameObject.h"
template<typename T>
class CObjectPool
{
private:
	explicit CObjectPool() {}
	~CObjectPool() {}
public:
	CGameObject*	ReuseObject(_vec3& vPos)
	{

	}
	void			CollectObject(CGameObject* pObj)
	{

	}

	static CObjectPool*		m_pInstance = nullptr;
public:
	static CObjectPool* GetInstance()
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CObjectPool;
		}
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};

