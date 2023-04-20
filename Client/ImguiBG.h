#pragma once

#include"Include.h"
class CImguiBG
{
public:
	explicit CImguiBG(LPDIRECT3DDEVICE9 pGraphicDev);
	~CImguiBG();

public:
	void	Set_StageNumber(_int _iNumber) { m_iStageNumber = _iNumber; }

public:
	_int Update_Imgui_Unit();
	void CreateDefaultBG();
	void InstallBG();
	void Release();
	HRESULT BGMenu();


	HRESULT SaveBG(_int iStageNumber);
	HRESULT LoadBG(_int iStageNumber, CScene* pScene = nullptr);

	HRESULT	Undo(_int iStageNumber);
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	CGameObject* m_pDefaultBG=nullptr;
	vector<OBJINFO> m_vecBGInfo;
	_bool m_BG_On = false;
	_int m_iBG_Type=0;	
	vector<CGameObject*> m_vecGameObject;







private:
	_int m_iStageNumber;


public:
	static	CImguiBG* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	template<typename T>
	void MakeBG(CLayer* pLayer, const _tchar* pObjTag);

};

