#pragma once
#include "Scene.h"

class CFinalStage1 : public Engine::CScene
{
private:
	explicit CFinalStage1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalStage1();

public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT		Ready_Scene(void) override;
	virtual _int		Update_Scene(const _float& fTimeDelta)override;
	virtual void		LateUpdate_Scene(void)override;
	virtual void		Render_Scene(void) override;

private:
	HRESULT				Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);
public:
	static CFinalStage1*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
	_int m_iCount = 0;

};

