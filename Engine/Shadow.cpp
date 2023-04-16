#include "stdafx.h"
#include "Shadow.h"

#include "Export_Function.h"

CShadow::CShadow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_bUseOutLine(true), m_fOutLineHeight(0.0f), m_fOutLineScale(0.0f), m_bUseShadow(true)
{
}

CShadow::CShadow(const CShadow & rhs)
	: CComponent(rhs), m_bUseOutLine(rhs.m_bUseOutLine), m_fOutLineHeight(rhs.m_fOutLineHeight), m_fOutLineScale(rhs.m_fOutLineScale), m_bUseShadow(rhs.m_bUseShadow)
{
}

CShadow::~CShadow()
{
}

HRESULT CShadow::Ready_CComponent()
{
	m_fOutLineHeight = 0.001f;
	m_fOutLineScale = 0.5f;
	return S_OK;
}

void CShadow::Render_Shadow(CVIBuffer* VIBuffer)
{
	D3DXMATRIX matWorld;

	m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DXMATRIX matWorldStaticZ;

	if (m_bUseOutLine)
	{
		_vec3 vOutLineScale = { 1.0f + 0.1f * m_fOutLineScale, 1.0f + 0.1f * m_fOutLineScale, 1.0f };

		D3DXMATRIX matOutLine;

		D3DXMatrixScaling(&matOutLine, vOutLineScale.x, vOutLineScale.y, vOutLineScale.z);

		matWorldStaticZ = matOutLine * matWorld;

		matWorldStaticZ._43 += m_fOutLineHeight;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorldStaticZ);

		VIBuffer->Render_Buffer();
	}

	if (m_bUseShadow)
	{
		matWorldStaticZ = matWorld;
		matWorldStaticZ._43 = 11.0f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorldStaticZ);

		VIBuffer->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
}

void CShadow::Render_Shadow(CVIBuffer* VIBuffer, _float x, _float y, _float z)
{
	D3DXMATRIX matWorld;

	m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);

	_vec3 vOutLineScale = { x, y, z };

	D3DXMATRIX matOutLine;

	D3DXMatrixScaling(&matOutLine, vOutLineScale.x, vOutLineScale.y, vOutLineScale.z);

	D3DXMATRIX matWorldStaticZ = matWorld;


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//matWorldStaticZ._43 += 0.001f;
	matWorldStaticZ._43 = 10.0f;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorldStaticZ);

	VIBuffer->Render_Buffer();


	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorldStaticZ);

	//VIBuffer->Render_Buffer();
	

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
}

CShadow * CShadow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShadow *	pInstance = new CShadow(pGraphicDev);

	if (FAILED(pInstance->Ready_CComponent()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CShadow::Clone(void)
{
	return new CShadow(*this);
}

void CShadow::Free()
{
	__super::Free();
}
