#pragma once
#include "Include.h"
#include "GameObject.h"
#include "Export_Function.h"
BEGIN(Engine)
#define FACTORY		CAbstractFactory
template<typename T>
class CAbstractFactory 
{
private:
	explicit CAbstractFactory(){}
	~CAbstractFactory(){}

public:
	static HRESULT	Create(
		LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene,
		CLayer* pLayer)
	{
		// ����
		T* nameT = nullptr;
		CGameObject* pGameObject = T::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		// Ŭ���� Ÿ�� ��ȯ
		const char* cName = typeid(nameT).name();
		_tchar tcName[MAX_STR] = L"";
		size_t cn;
		const size_t cSize = strlen(cName) + 1;
		// Ŭ�����̸� ����
		mbstowcs_s(&cn, tcName, cSize, cName, cSize);
		wcsncpy_s(tcName, wcslen(tcName) - 1, wcschr(tcName, 'C'), _TRUNCATE);
		_tchar cpyStr[MAX_STR] = L"";
		wcsncpy_s(cpyStr, wcslen(tcName) - 1, tcName, _TRUNCATE);
		_tchar revStr[MAX_STR] = L"";
		wcsncpy_s(revStr, wcslen(tcName) - 2, _wcsrev(cpyStr), _TRUNCATE);
		// ���α���ü �����Ҵ� (���ڿ� ��ü �����Ҵ��ϸ� �����Ұ���)
		Mytchar* mytchar = new Mytchar;
		wcsncpy_s(mytchar->str, wcslen(tcName) - 2, _wcsrev(revStr), _TRUNCATE);

		// ���� Ŭ���� ���� ��ȯ
		size_t cnt = pScene->Find_ClassName(mytchar);
		// Ŭ���� ������ ���� ���̹�
		wcsncat_s(mytchar->str, wcslen(tcName), L"_", wcslen(tcName));
		_tchar strCnt[MAX_STR] = L"";
		_stprintf_s(strCnt, L"%d", (cnt));
		wcsncat_s(mytchar->str, wcslen(tcName) + wcslen(strCnt), strCnt, wcslen(tcName) + wcslen(strCnt));
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(mytchar->str, pGameObject), E_FAIL);
		return  S_OK;
	}

	static HRESULT	Create(
		LPDIRECT3DDEVICE9 pGraphicDev, CScene* pScene,
		CLayer* pLayer, _vec3& vPos)
	{
		// ����
		T* nameT = nullptr;
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		// Ŭ���� Ÿ�� ��ȯ
		const char* cName = typeid(nameT).name();
		_tchar tcName[MAX_STR] = L"";
		size_t cn;
		const size_t cSize = strlen(cName) + 1;
		// Ŭ�����̸� ����
		mbstowcs_s(&cn, tcName, cSize, cName, cSize);
		wcsncpy_s(tcName, wcslen(tcName) - 1, wcschr(tcName, 'C'), _TRUNCATE);
		_tchar cpyStr[MAX_STR] = L"";
		wcsncpy_s(cpyStr, wcslen(tcName) - 1, tcName, _TRUNCATE);
		_tchar revStr[MAX_STR] = L"";
		wcsncpy_s(revStr, wcslen(tcName) - 2, _wcsrev(cpyStr), _TRUNCATE);
		// ���α���ü �����Ҵ� (���ڿ� ��ü �����Ҵ��ϸ� �����Ұ���)
		Mytchar* mytchar = new Mytchar;
		wcsncpy_s(mytchar->str, wcslen(tcName) - 2, _wcsrev(revStr), _TRUNCATE);

		// ���� Ŭ���� ���� ��ȯ
		size_t cnt = pScene->Find_ClassName(mytchar);
		// Ŭ���� ������ ���� ���̹�
		wcsncat_s(mytchar->str, wcslen(tcName), L"_", wcslen(tcName));
		_tchar strCnt[MAX_STR] = L"";
		_stprintf_s(strCnt, L"%d", (cnt));
		wcsncat_s(mytchar->str, wcslen(tcName) + wcslen(strCnt), strCnt, wcslen(tcName) + wcslen(strCnt));
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(mytchar->str, pGameObject), E_FAIL);
		return S_OK;
	}
};

END