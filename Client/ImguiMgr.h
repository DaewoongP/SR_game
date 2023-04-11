#pragma once
#include"..\Engine\Define.h"
#include "Include.h"

class CImguiStage;
class CImguiUnit;

class CImguiMgr
{
	DECLARE_SINGLETON(CImguiMgr)
		
private:
	explicit CImguiMgr();
	~CImguiMgr();

public:
	HRESULT Ready_Imgui(LPDIRECT3DDEVICE9 m_pGraphicDev);
	HRESULT Update_Imgui(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void	Release();

private:
	CImguiStage* m_pImguiStage;		// �������� ��
	CImguiUnit*  m_pImguiUnit;		// ���� ��

	CGameObject* m_pToodee;			// ���� �ִ��� Ȯ��
	bool		 m_bOnceLoad;		// ���� �� �� �ε�
	_int		 m_iStageNumber;	// �������� ���� ����

	// window â onoff ���� ����
	bool m_bStageTool; // �������� onoff
	bool m_bUnitTool;  // ���� onoff
};