#pragma once
#include "Include.h"
#include "GameObject.h"
template<typename T>
class CObjectPool
{
private:
	explicit CObjectPool() 
	{
		m_ObjectList.reserve(800);
		for (size_t i = 0; i < 800; ++i)
		{
			m_ObjectList.pu
		}
	}
	~CObjectPool() {}
public:
	CGameObject*	ReuseObject(_vec3& vPos)
	{

	}
	void			CollectObject(CGameObject* pObj)
	{

	}

	list<CGameObject*>		m_ObjectList;
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

