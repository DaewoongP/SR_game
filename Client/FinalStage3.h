#pragma once
#include "Scene.h"

enum Final3_MonkeyState
{
	F3_SpawnCube,
	F3_MonkeyAppear,
	F3_Destroy,
	F3_NONE
};

#define Final_STATE_FUNC vector<void(CFinalStage3::*)(const _float& fTimeDelta)>
class CFinalStage3 : public Engine::CScene
{
private:
	explicit CFinalStage3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalStage3();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT		Ready_Scene(void) override;
	virtual _int		Update_Scene(const _float& fTimeDelta)override;
	virtual void		LateUpdate_Scene(void)override;
	virtual void		Render_Scene(void) override;

	void				Set_State(Final3_MonkeyState eState) 
	{ 
		if (m_StageState != eState)
			m_StageState = eState;
		else
			return;
	}
private:
	HRESULT				Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);

private:
	void				SpawnCube_Monkey(const _float& fTimeDelta);
	void				Check_Cube_Place(const _float& fTimeDelta);
	void				MonkeyAppear(const _float& fTimeDelta);
	void				MonkeyDisAppear(const _float& fTimeDelta);
	void				ActionNone(const _float& fTimeDelta);

private:
	Final_STATE_FUNC funcAction;
	vector<CGameObject*> m_MokeyCube;
	_bool			 	 m_SpwanCube;
	CGameObject*		 m_pMonkey;
	Final3_MonkeyState	 m_StageState;

	class CFinal3Boss1*			m_pBoss;
	_bool				m_bMonkeySpwanTrigger;
public:
	static CFinalStage3*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
	_int m_iCount = 0;

};

