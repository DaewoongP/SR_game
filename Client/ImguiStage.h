#pragma once
#include "Include.h"

class CImguiStage
{
public:
	explicit CImguiStage(LPDIRECT3DDEVICE9 pGraphicDev);
	~CImguiStage();

public:
	_int Update_Imgui_Stage();
	void Release();
	
	HRESULT SaveGrid();
	HRESULT LoadGrid();
	HRESULT SaveCube();
	HRESULT LoadCube();

private:
	// �׸��� �޴�
	HRESULT GridMenu();
	void	GroundGridON();
	void	CreateDefaultGrid();
	void	GridInstall();

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

	bool m_bGridCreate;						// ���� �׶��� �׸��� ����
	bool m_bDefaultGridCreate;				// ����Ʈ �׸��� ����

											// ť�� ����
	CGameObject* m_pDefaultCube;			// ����Ʈ ť��
	vector<OBJINFO> m_vecCubeInfo;			// ť�� ����(���� �뵵)

	bool m_bCubePlaced;						// ����Ʈ ť�� ���� 
	int m_iCubeTextureNumber;				// ť�� ��Ÿ�� ��ȣ
	
public:
	static	CImguiStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

