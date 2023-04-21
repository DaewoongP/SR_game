#pragma once
#include "Scene.h"

class CStage8 : public Engine::CScene
{
private:
	explicit CStage8(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage8();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT		Ready_Scene(void) override;
	virtual _int		Update_Scene(const _float& fTimeDelta)override;
	virtual void		LateUpdate_Scene(void)override;
	virtual void		Render_Scene(void) override;

private:
	HRESULT				Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);
	void				PatternSet(const _float & fTimeDelta);
	void				Player_Reset();

private:
	_int				m_iPreBossHp;
	_bool				m_bLerp;
	_bool				m_bLerpInit;
	_float				m_fDist;
	_vec3				m_vPos;

public:
	static CStage8*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};