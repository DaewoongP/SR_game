#pragma once

#include"Include.h"
BEGIN(Engine)
class CRcTex;
class CTransform;
END
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
	void CreateBackData();

	void InstallBG();
	void Release();
	HRESULT BGMenu();
	void Preview();
	void Scale();
	void Stage1Object(CLayer* pStageLayer);
	void Stage2Object(CLayer* pStageLayer);
	
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
	
	CUBE_DIR	m_tDecoDir;

	Engine::CTransform* m_pTransformCom;



private:
	_int m_iStageNumber;

public:
	static	CImguiBG* Create(LPDIRECT3DDEVICE9 pGraphicDev);


	template<typename T>
	void MakeBG(CLayer* pLayer, const _tchar* pObjTag);

	template<typename T>
	void MakeBG_PS(CLayer* pLayer, const _tchar* pObjTag);

	template<typename T>
	void MakeBGNum(CLayer* pLayer, const _tchar* pObjTag, _int iNum);

	

};

