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

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// �׸��� ����
	CGameObject* m_pDefaultGrid;			// ����Ʈ �׸��� ����
	vector<CGameObject*> m_vecGroundGrid;	// �׸��� ����
	vector<OBJINFO> m_vecInstallGrid;		// ��ġ�� �׸��� ����(���� �뵵)

	bool m_bGridON;							// �׶��� �׸��� on / off
	bool m_bGridCreate;						// ���� �׶��� �׸��� ����
	bool m_bDefaultGridCreate;				// ����Ʈ �׸��� ����
	int  m_iInstallGridIndex;				// ��ġ�� �׸��� ����

	// ť�� ����
	CGameObject* m_pDefaultCube;			// ����Ʈ ť��
	vector<OBJINFO> m_vecCubeInfo;			// ť�� ����(���� �뵵)

	bool m_bCubePlaced;						// ����Ʈ ť�� ���� 
	int m_iCubeTextureNumber;				// ť�� ��Ÿ�� ��ȣ
	int m_iCubeIndex;						// ��ġ�� ť�� ����

private:
	// �׸��� �޴�
	HRESULT GridMeun();
	HRESULT GroundGridON();
	CGameObject* CreateDefaultGrid();
	void	GridInstall();

	HRESULT SaveGrid();
	HRESULT LoadGrid();

	// ť�� �޴�
	HRESULT CubeMeun();
	CGameObject* CreateDefaultCube();
	void	CubeInstall();

	HRESULT SaveCube();
	HRESULT LoadCube();
	
public:
	static	CImguiStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

