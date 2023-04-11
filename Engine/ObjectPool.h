#pragma once
#include "Include.h"
#include "GameObject.h"
template<typename T>
class CObjectPool
{
private:
	explicit CObjectPool() 
	{
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		m_ObjectList.reserve(100);
		for (size_t i = 0; i < 100; ++i)
		{
			CGameObject* pObject = new T(pGraphicDev);
			m_ObjectList.push_back(pObject);
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

