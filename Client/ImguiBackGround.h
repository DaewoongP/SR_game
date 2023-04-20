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
	//이미지 버튼으로 선택?
	//콤보 박스로 스테이지 선택
	void Stage_ComboBox();
	//스위치 문으로 업데이트 나눔
	void Show_Img();
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// 스테이지 저장용 변수
	_int m_iStageNumber;

	// 이미지 순서 저장용 변수
	_int m_iSelectImage;

	//스테이지 번호와 순서대로 저장한 이미지로 오브젝트 키를 지정할 변수
	vector<vector<const _tchar*>> vecpObj;

private:
	

public:
	static	CImguiBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	
};

