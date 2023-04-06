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
	HRESULT GridMeun();
	HRESULT GroundGridON(vector<CGameObject*>& vecGrid);

	// ť�� �޴�
	HRESULT CubeMeun();
	CGameObject* CreateDefaultCube();
	void	CubeInstall(CGameObject* pDefaultCube, vector<CUBEINFO>& vecCubeInfo, int& iCubeIndex, int iCubeTextureNumber);

	HRESULT SaveCube(vector<CUBEINFO>& vecCubeInfo);
	HRESULT LoadCube(vector<CUBEINFO>& vecCubeInfo, int& iCubeIndex);
	
public:
	static	CImguiStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

