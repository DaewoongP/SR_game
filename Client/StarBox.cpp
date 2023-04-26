#include "stdafx.h"
#include "StarBox.h"

#include "Export_Function.h"

CStarBox::CStarBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStarBox::~CStarBox()
{
}

HRESULT CStarBox::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	/* 이코드 트랜스폼 컴포넌트 안썼음.*/
	m_pTransform->m_vScale = { 200.f, 200.f, 200.f };

	m_pStarParticle->Start_Particle();

	return S_OK;
}
_int CStarBox::Update_GameObject(const _float& fTimeDelta)
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
void CStarBox::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

// D3DRS_ZENABLE			: Z버퍼에 Z값을 무조건 기록은 하되 정렬을 수행할지 말지 결정하는 옵션
// D3DRS_ZWRITEENABLE		: Z버퍼에 픽셀의 깊이 값을 기록을 할지 말지 결정하는 옵션

void CStarBox::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	m_pStarParticle->Update_Particle();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CStarBox::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"SkyBox_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SkyBox_Texture", pComponent });

	pComponent = m_pStarParticle = dynamic_cast<CStarParticle*>(Engine::Clone_Proto(L"StarParticle", this));
	NULL_CHECK_RETURN(m_pStarParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"StarParticle", pComponent });

	return S_OK;
}

_matrix CStarBox::GetRotationMatrix(const _matrix & OriginMatrix)
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

CStarBox* CStarBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStarBox*		pInstance = new CStarBox(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStarBox::Free(void)
{
	__super::Free();
}

