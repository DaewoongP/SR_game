#pragma once
#include "Monster.h"

class CDefaultUnit : public CMonster
{
public:
	enum DefaultObject
	{
		DMonster,
		DMapObject
	};

private:
	explicit CDefaultUnit(PDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDefaultUnit();

public:
	void	Set_DefaultIndex(DefaultObject _iIndex) { m_iDefaultIndex = _iIndex; }

public:
	// CMonster��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);
	void Mouse_Move();

private:
	Engine::CTexture*		m_pTextureCom2; // �� ������Ʈ ����

	int m_iDefaultIndex; // ����Ʈ ���� ����

public:
	static CDefaultUnit*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

