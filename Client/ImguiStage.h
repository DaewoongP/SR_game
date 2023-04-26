#pragma once
#include "Include.h"

class CImguiStage
{
public:
	explicit CImguiStage(LPDIRECT3DDEVICE9 pGraphicDev);
	~CImguiStage();

public:
	void	Set_StageNumber(_int _iNumber) { m_iStageNumber = _iNumber; }
	vector<OBJINFO>* Get_GridVector() { return &m_vecInstallGrid; }
	vector<OBJINFO>* Get_CubeVector() { return &m_vecCubeInfo; }

public:
	_int Update_Imgui_Stage();
	void Release();
	
	HRESULT SaveGrid(_int iStageNumber);
	HRESULT LoadGrid(_int iStageNumber, CScene* pScene = nullptr);
	HRESULT SaveCube(_int iStageNumber);
	HRESULT LoadCube(_int iStageNumber, CScene* pScene = nullptr);

	HRESULT	Undo(_int iStageNumber);
private:
	// �׸��� �޴�
	HRESULT GridMenu();
	void	GroundGridON();
	void	CreateDefaultGrid();
	void	GridInstall();

	void Scale();

	// ť�� �޴�
	HRESULT CubeMenu();
	void	CreateDefaultCube();
	void	CubeInstall();

public:
	static bool m_bGridON;					// �׶��� �׸��� on / off

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// �׸��� ����
	CGameObject* m_pDefaultGrid;			// ����Ʈ �׸��� ����
	vector<CGameObject*> m_vecGroundGrid;	// �׶��� �׸��� ����
	vector<OBJINFO> m_vecInstallGrid;		// ��ġ�� �׸��� ����(���� �뵵)
	vector<CGameObject*> m_vecGameObject;	// ������ ������Ʈ ����

	bool m_bGridCreate;						// ���� �׶��� �׸��� ����
	bool m_bDefaultGridCreate;				// ����Ʈ �׸��� ����

	// ť�� ����
	CGameObject* m_pDefaultCube;			// ����Ʈ ť��
	vector<OBJINFO> m_vecCubeInfo;			// ť�� ����(���� �뵵)

	bool m_bCubePlaced;						// ����Ʈ ť�� ���� 
	int m_iCubeTextureNumber;				// ť�� ��Ÿ�� ��ȣ

	_int m_iStageNumber;					// �������� ����� ����
	
public:
	static	CImguiStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void MakeGameObject(CLayer* pLayer, const _tchar * pObjTag);
	void MakeGameObject(CLayer* pLayer, const _tchar * pObjTag, _int iNum);
};