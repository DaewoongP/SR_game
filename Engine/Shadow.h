#pragma once
#include "Component.h"

BEGIN(Engine)

class CVIBuffer;

class CShadow :
	public CComponent
{
private:
	explicit CShadow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShadow(const CShadow& rhs);
	virtual ~CShadow();

public:
	HRESULT		Ready_CComponent();
	//���� ��İ� �ؽ�ó ���� ���� ���
	void		Render_Shadow(CVIBuffer* VIBuffer);

public:
	static CShadow*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent * Clone(void) override;


private:
	virtual void Free() override;
};

END