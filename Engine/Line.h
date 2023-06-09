#pragma once
#include "Component.h"

BEGIN(Engine)

class CLine : public CComponent
{
private:
	explicit CLine(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLine(const CLine& rhs);
	virtual ~CLine();

public:
	void		Set_Width(_float fWidth) { m_fWidth = fWidth; }

public:
	HRESULT		Ready_Line(_float width);
	// 시작점 좌표, 끝점 좌표, 컬러
	void		Set_Line(const _vec3& first, const _vec3& second, D3DXCOLOR color);
	// 라인의 트랜스폼 포인터, 현재 뷰행렬, 현재 투영행렬
	void		Draw_Line(const _matrix& matWorld, const _matrix& matView, const _matrix& matProj);
public:
	static CLine*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float width = 3.f);
	virtual CComponent * Clone(void) override;
private:
	virtual void Free() override;

private:
	LPD3DXLINE		m_pLine;
	_float			m_fWidth;
	_vec3			m_vecLine[2];
	_int			m_iCnt;
	D3DXCOLOR		m_Color;
};

END