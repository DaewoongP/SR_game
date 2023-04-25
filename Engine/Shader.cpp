#include "stdafx.h"
#include "Shader.h"
CShader::CShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
{
	m_pEffect->AddRef();
}

HRESULT CShader::Ready_Shader(const _tchar * pShaderFilePath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pShaderFilePath, nullptr, nullptr, 0, nullptr, &m_pEffect, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	_uint		iNumPasses = { 0 };

	m_pEffect->Begin(&iNumPasses, 0);

	m_pEffect->BeginPass(iPassIndex);

	return S_OK;
}

HRESULT CShader::End()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->EndPass();
	m_pEffect->End();

	return S_OK;
}


HRESULT CShader::Set_Float3(D3DXHANDLE hParameter, const _vec3 * pVector)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	if (FAILED(m_pEffect->SetVector(hParameter, &D3DXVECTOR4(*pVector, 1.f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::Set_Matrix(D3DXHANDLE hParameter, const _matrix* pMatrix)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	if (FAILED(m_pEffect->SetMatrix(hParameter, pMatrix)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::Set_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	return m_pEffect->SetTexture(hParameter, pTexture);
}

CShader * CShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pShaderFilePath)
{
	CShader *	pInstance = new CShader(pGraphic_Device);

	if (FAILED(pInstance->Ready_Shader(pShaderFilePath)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CShader::Clone(void)
{
	return new CShader(*this);
}


void CShader::Free(void)
{
	__super::Free();

	Safe_Release(m_pEffect);

}
