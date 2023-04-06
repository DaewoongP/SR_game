#pragma once

#include "Include.h"

class CImguiStage
{
private:
	explicit CImguiStage(LPDIRECT3DDEVICE9 pGraphicDev);
	~CImguiStage();

public:
	_int Update_Imgui_Stage(LPDIRECT3DDEVICE9 m_pGraphicDev);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// ť�� ������ ���� ����ü
	typedef struct CubeInfo
	{
		_vec3	vCubePos;					// ť�� ��ġ
		int		iCubeTextureNumber;			// ť�� ��Ÿ�� ��ȣ
	}CUBEINFO;

	// �׸��� ����
	vector<CGameObject*> m_vecGroundGrid;	// �׸��� ����
	bool m_bGridON;							// �׸��� on / off
	bool m_bGridCreate;						// ���� �׸��� ����

	// ť�� ����
	CGameObject* m_pDefaultCube;			// ����Ʈ ť��
	vector<CUBEINFO> m_vecCubeInfo;			// ť�� ����(���� �뵵)

	bool m_bCubePlaced;						// ����Ʈ ť�� ���� 
	int m_iCubeTextureNumber;				// ť�� ��Ÿ�� ��ȣ
	int m_iCubeIndex;						// ��ġ�� ť�� ����

private:
	// �׸��� �޴�
	HRESULT GridMeun(LPDIRECT3DDEVICE9 m_pGraphicDev);
	HRESULT GroundGridON(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CGameObject*>& vecGrid);

	// ť�� �޴�
	HRESULT CubeMeun(LPDIRECT3DDEVICE9 m_pGraphicDev);
	CGameObject* CreateDefaultCube(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void	CubeInstall(CGameObject* pDefaultCube, LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CUBEINFO>& vecCubeInfo, int& iCubeIndex, int iCubeTextureNumber);


	HRESULT SaveData(vector<CUBEINFO>& vecCubeInfo);
	HRESULT LoadData(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CUBEINFO>& vecCubeInfo, int& iCubeIndex);
	
public:
	static	CImguiStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

