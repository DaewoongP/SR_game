#pragma once
#include"BackGroundBase.h"



class CDefaultBG : public CBackGroundBase
{
private:
	explicit CDefaultBG(PDIRECT3DDEVICE9 pGraphicdev);
	virtual ~CDefaultBG();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);
	void Mouse_Move();

private:
	Engine::CTexture* m_pTextureCom2; // 맵 오브젝트 전용
	int m_iDefaultIndex; // 디폴트 종류 선택

public:
	static CDefaultBG* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;



};

