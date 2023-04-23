#include "stdafx.h"
#include "SkyBox.h"

#include "Export_Function.h"
#include "..\Engine\SkyParticle.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	/* ���ڵ� Ʈ������ ������Ʈ �Ƚ���.*/
	m_pTransform->m_vScale = { 200.f, 200.f, 200.f };

	m_pSkyParticle->Start_Particle();

	return S_OK;
}
_int CSkyBox::Update_GameObject(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	_matrix		matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);
	_matrix viewRotMatrix = GetRotationMatrix(matCamWorld);

	_vec3 camTrans = { matCamWorld._41, matCamWorld._42 + 3.f, matCamWorld._43 };
	_matrix matScale, matTrans;
	D3DXMatrixTranslation(&matTrans, camTrans.x, camTrans.y, camTrans.z);
	D3DXMatrixScaling(&matScale, m_pTransform->m_vScale.x, m_pTransform->m_vScale.y, m_pTransform->m_vScale.z);
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_matWorld = matScale * viewRotMatrix * matTrans;
	return 0;
}
void CSkyBox::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

// D3DRS_ZENABLE			: Z���ۿ� Z���� ������ ����� �ϵ� ������ �������� ���� �����ϴ� �ɼ�
// D3DRS_ZWRITEENABLE		: Z���ۿ� �ȼ��� ���� ���� ����� ���� ���� �����ϴ� �ɼ�

void CSkyBox::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	m_pSkyParticle->Update_Particle();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CSkyBox::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"SkyBox_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SkyBox_Texture", pComponent });

	pComponent = m_pSkyParticle = dynamic_cast<CSkyParticle*>(Engine::Clone_Proto(L"SkyParticle", this));
	NULL_CHECK_RETURN(m_pSkyParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SkyParticle", pComponent });

	return S_OK;
}

_matrix CSkyBox::GetRotationMatrix(const _matrix & OriginMatrix)
{
	_matrix retMat;
	D3DXMatrixIdentity(&retMat);
	_vec3 vOrigin[3];
	_vec3 vRet[3];

	for (size_t i = 0; i < 3; ++i)
	{
		memcpy(&vOrigin[i], &OriginMatrix.m[i][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRet[i], &vOrigin[i]);
		memcpy(&retMat.m[i][0], &vRet[i], sizeof(_vec3));
	}

	return retMat;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox*		pInstance = new CSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSkyBox::Free(void)
{
	__super::Free();
}

