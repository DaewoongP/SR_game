#pragma once
#include "Component.h"

BEGIN(Engine)

class CShader final : public CComponent
{
private:
	CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	HRESULT Ready_Shader(const _tchar* pShaderFilePath);
	HRESULT Begin(_uint iPassIndex);
	HRESULT End();

public:
	HRESULT Set_Float3(D3DXHANDLE hParameter, const _vec3* pVector);
	HRESULT Set_Matrix(D3DXHANDLE hParameter, const _matrix* pMatrix);
	HRESULT Set_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture);

private:
	LPD3DXEFFECT			m_pEffect = { nullptr };

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual	CComponent*	Clone(void);
	virtual void Free(void) override;
};
END