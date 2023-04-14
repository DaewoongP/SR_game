#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_bMove(true)

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

	// ũ�� ��ȯ
	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	// ȸ�� ��ȯ
	_matrix			matRot[ROT_END];
	_matrix			matRotation;

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	matRotation = matRot[ROT_Y] * matRot[ROT_Z] * matRot[ROT_X];

	//����ũ
	_vec3 vShake = { 0.0f,0.0f,0.0f };

	Update_Shake(fTimeDelta, vShake);

	// ��ġ ��ȯ
	_matrix			matTrans;
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x + vShake.x, m_vInfo[INFO_POS].y + vShake.y, m_vInfo[INFO_POS].z + vShake.z);

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
void CTransform::Update_Shake(_float fTimeDelta, _vec3& vPos)
{
	if (m_fShakeTimer > 0.0f)
	{
		switch (m_iShakeAxis)
		{
		case Engine::SHAKE_ALL:
			m_vShakeOffset = _vec3(
				(rand() %(int)m_fShakePower * 2 - m_fShakePower)*0.01f,
				(rand() % (int)m_fShakePower * 2 - m_fShakePower)*0.01f,
				(rand() % (int)m_fShakePower * 2 - m_fShakePower)*0.01f);
			break;
		case Engine::SHAKE_X:
			m_vShakeOffset = _vec3(
				(rand() % (int)m_fShakePower * 2 - m_fShakePower)*0.01f,
				0.0f,
				0.0f);
			break;
		case Engine::SHAKE_Y:
			m_vShakeOffset = _vec3(
				0.0f,
				(rand() % (int)m_fShakePower * 2 - m_fShakePower)*0.01f,
				0.0f);
			break;
		case Engine::SHAKE_Z:
			m_vShakeOffset = _vec3(
				0.0f,
				0.0f,
				(rand() % (int)m_fShakePower * 2 - m_fShakePower)*0.01f);
			break;
		case Engine::SHAKE_END:
			break;
		default:
			break;
		}
		vPos += m_vShakeOffset;
		if (m_bUseWeak)
		{
			m_fShakePower -= m_fWeakPoint * fTimeDelta;
			if (1.0f >= m_fShakePower)
			{
				m_fShakePower = 1.0f;
			}
		}
		m_fShakeTimer -= fTimeDelta;
	}
	else
	{
		m_fShakePower = 0.0f;
		m_vShakeOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

void CTransform::Set_ParentTransform(CGameObject * pParentObject, _float fX, _float fY, _float fZ)
{
	// 초기화
	D3DXMatrixIdentity(&m_matWorld);

	// 스케일
	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	// 회전
	_matrix matRot[ROT_END];
	_matrix matRotation;
	
	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	matRotation = matRot[ROT_Y] * matRot[ROT_Y] * matRot[ROT_X];

	//이동
	_matrix matTrans;
	D3DXMatrixTranslation(&matTrans, fX, fY, fZ);

	// 부모 회전
	_matrix matRotP[ROT_END];
	_matrix matRotationP;

	D3DXMatrixRotationX(&matRotP[ROT_X], pParentObject->m_pTransform->m_vAngle.x);
	D3DXMatrixRotationY(&matRotP[ROT_Y], pParentObject->m_pTransform->m_vAngle.y);
	D3DXMatrixRotationZ(&matRotP[ROT_Z], pParentObject->m_pTransform->m_vAngle.z);

	matRotationP = matRotP[ROT_Y] * matRotP[ROT_Y] * matRotP[ROT_X];

	// 부모 이동
	_matrix matTransP;
	_vec3 vTransP = pParentObject->m_pTransform->m_vInfo[INFO_POS];
	D3DXMatrixTranslation(&matTransP, vTransP.x, vTransP.y, vTransP.z);

	m_matWorld = matScale * m_matBillY * m_matBillX * matRotation * matTrans * matRotationP * matTransP;
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