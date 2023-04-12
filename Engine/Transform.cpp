#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
	,m_bMove(true)
{
	ZeroMemory(&m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matBillX);
	D3DXMatrixIdentity(&m_matBillY);
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
	, m_matWorld(rhs.m_matWorld)
	, m_matBillX(rhs.m_matBillX)
	, m_matBillY(rhs.m_matBillY)
	,m_bMove(rhs.m_bMove)
{
	for (size_t i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];
}

CTransform::~CTransform()
{
}

void Engine::CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matRot, matTrans;

	matRot = *Compute_Lookattarget(pTargetPos);

	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
		m_vInfo[INFO_POS].z);

	m_matWorld = matRot * matTrans;

}

const _matrix* Engine::CTransform::Compute_Lookattarget(const _vec3* pTargetPos)
{
	_vec3	vDir = *pTargetPos - m_vInfo[INFO_POS];

	_matrix	matRot;
	_vec3	vAxis, vUp;

	return D3DXMatrixRotationAxis(&matRot, 
								  D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir),
								  acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), 
												    D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}


HRESULT CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (size_t i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int CTransform::Update_Component(const _float & fTimeDelta)
{
	if (!m_bMove)
		return 0;
	D3DXMatrixIdentity(&m_matWorld);

	for (size_t i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기 변환
	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	// 회전 변환
	_matrix			matRot[ROT_END];
	_matrix			matRotation;

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	matRotation = matRot[ROT_Y] * matRot[ROT_Z] * matRot[ROT_X];

	// 위치 변환
	_matrix			matTrans;
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * m_matBillY * m_matBillX * matRotation * matTrans;

	return 0;
}

void CTransform::LateUpdate_Component(void)
{
}

void CTransform::Move_Floating(const _float & fTimeDelta, _float fPower, _float fSpeed)
{
	m_fFloating += fTimeDelta * fSpeed;

	m_vInfo[INFO_POS].y += fPower * sinf(D3DXToRadian(m_fFloating));
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform *	pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CTransform::Clone(void)
{
	return new CTransform(*this);
}

void CTransform::Free(void)
{
	__super::Free();
}
