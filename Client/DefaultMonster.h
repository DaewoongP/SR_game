#pragma once
#include "Monster.h"

class CDefaultMonster : public CMonster
{
public:
	enum DefaultObject
	{
		DMonster,
		DMapObject
	};

private:
	explicit CDefaultMonster(PDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDefaultMonster();

public:
	void	Set_DefaultIndex(DefaultObject _iIndex) { m_iDefaultIndex = _iIndex; }

public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);
	void Mouse_Move();

private:
	Engine::CTexture*		m_pTextureCom2; // 맵 오브젝트 전용

	int m_iDefaultIndex; // 디폴트 종류 선택

public:
	static CDefaultMonster*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

