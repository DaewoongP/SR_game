//#pragma once
//#include "Include.h"
//#include "GameObject.h"
//BEGIN(Engine)
//template<typename T>
//class CObjectPool
//{
//private:
//	explicit CObjectPool() 
//	{
//		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
//		m_ObjectList.reserve(100);
//		for (size_t i = 0; i < 100; ++i)
//		{
//			CGameObject* pObject = new T(pGraphicDev);
//			m_ObjectList.push_back(pObject);
//		}
//	}
//	~CObjectPool()
//	{
//		for_each(m_ObjectList.begin(), m_ObjectList.end(), CDeleteObj());
//		m_ObjectList.clear();
//	}
//public:
//	T*	ReuseObject()
//	{
//		T* pObject;
//		if (!m_ObjectList.empty())
//		{
//			pObject = m_ObjectList.front();
//			m_ObjectList.pop_front();
//		}
//		else
//		{
//			pObject = new T(pGraphicDev);
//		}
//		NULL_CHECK_RETURN(pObject, nullptr);
//		return pObject;
//	}
//	void			CollectObject(T* pObj)
//	{
//		if (nullptr == pObj)
//			return;
//
//		m_ObjectList.push_back(pObj);
//	}
//
//	list<CGameObject*>		m_ObjectList;
//	static CObjectPool*		m_pInstance = nullptr;
//public:
//	static CObjectPool* GetInstance()
//	{	
//		if (nullptr == m_pInstance)
//		{
//			m_pInstance = new CObjectPool;
//		}
//		return m_pInstance;
//	}
//
//	static void DestroyInstance()
//	{
//		if (nullptr != m_pInstance)
//		{
//			delete m_pInstance;
//			m_pInstance = nullptr;
//		}
//	}
//};
//
//END