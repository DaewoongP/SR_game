#pragma once
#include "Include.h"

class CImguiBackGround
{
public:
	explicit CImguiBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	~CImguiBackGround();

public:
	void	Set_StageNumber(_int _iNumber) { m_iStageNumber = _iNumber; }

public:
	HRESULT Ready_Imgui();

	_int Update_Imgui_BackGround();
	void Release();
	//�̹��� ��ư���� ����?
	//�޺� �ڽ��� �������� ����
	void Stage_ComboBox();
	//����ġ ������ ������Ʈ ����
	void Show_Img();
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// �������� ����� ����
	_int m_iStageNumber;

	// �̹��� ���� ����� ����
	_int m_iSelectImage;

	//�������� ��ȣ�� ������� ������ �̹����� ������Ʈ Ű�� ������ ����
	vector<vector<const _tchar*>> vecpObj;

private:
	

public:
	static	CImguiBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	
};

