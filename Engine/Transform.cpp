#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_bMove(true)
	, m_Parent(NULL)
	, m_Child(NULL)

{
	ZeroMemory(&m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matBillX);
	D3DXMatrixIdentity(&m_matBillY);
	m_YZValue = 1.f;
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
	, m_matWorld(rhs.m_matWorld)
	, m_matBillX(rhs.m_matBillX)
	, m_matBillY(rhs.m_matBillY)
	, m_bMove(rhs.m_bMove)
	, m_Parent(rhs.m_Parent)
	, m_Child(rhs.m_Child)
	, m_YZValue(rhs.m_YZValue)
{
	for (size_t i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];
}

CTransform::~CTransform()
{
}

void CTransform::MakeMyMatrix(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (size_t i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기 변환
	_matrix			matScale = GetScaleMat();
	// 회전 변환
	_matrix			matRotation = GetRotMat();
	// 위치 변환
	_matrix			matTrans = GetTransMat(fTimeDelta);

	//공전 변환
	_matrix			matRevolutionRotation = GetRevolutionRotMat();

	m_matRT = m_matBillY * m_matBillX * matRotation * matTrans;
	if (m_Parent != NULL)
		m_matRT = m_matRT * matRevolutionRotation * m_Parent->m_matRT;
	
	//매트릭스 생성
	m_matWorld = matScale * m_matBillY * m_matBillX * matRotation * matTrans;

	if (m_Parent == NULL)
		return;

	m_matWorld = matScale * m_matRT;
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

void CTransform::Set_ParentTransform(CGameObject * pParentObject)
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
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	// 부모값 넣음
	if (nullptr != pParentObject)
	{
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

	// 부모값 넣지 않음
	else
		m_matWorld = matScale * m_matBillY * m_matBillX * matRotation * matTrans;
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
	MakeMyMatrix(fTimeDelta);
	return 0;
}

void CTransform::LateUpdate_Component(void)
{
}

void CTransform::Move_Floating(const _float & fTimeDelta, _float fPower, _float fSpeed, FLOATING_AXIS eXYZ)
{
	m_fFloating += fTimeDelta * fSpeed;

	switch (eXYZ)
	{
	case Engine::FLOATING_X:
		Floating_X(fPower);
		break;
	case Engine::FLOATING_Y:
		Floating_Y(fPower);
		break;
	case Engine::FLOATING_Z:
		Floating_Z(fPower);
		break;
	case Engine::FLOATING_XY:
		Floating_X(fPower);
		Floating_Y(fPower);
		break;
	case Engine::FLOATING_XZ:
		Floating_X(fPower);
		Floating_Z(fPower);
		break;
	case Engine::FLOATING_YZ:
		Floating_Y(fPower);
		Floating_Z(fPower);
		break;
	case Engine::FLOATING_XYZ:
		Floating_X(fPower);
		Floating_Y(fPower);
		Floating_Z(fPower);
		break;
	case Engine::FLOATING_END:
		break;
	default:
		break;
	}

}
void CTransform::Update_Shake(_float fTimeDelta, _vec3& vPos)
{
	if (m_fShakeTimer > 0.0f)
	{
		switch (m_iShakeAxis)
		{
		case Engine::SHAKE_ALL:
			m_vShakeOffset = _vec3(
				(rand() % (int)m_fShakePower * 2 - m_fShakePower)*0.01f,
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

_matrix CTransform::GetRotMat()
{
	// 회전 변환
	_matrix			matRot[ROT_END];
	_matrix			matRotation;

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	return (matRot[ROT_Y] * matRot[ROT_Z] * matRot[ROT_X]);
}

_matrix CTransform::GetRevolutionRotMat()
{
	// 회전 변환
	_matrix			matRot[ROT_END];
	_matrix			matRotation;

	D3DXMatrixRotationX(&matRot[ROT_X], m_vRevolutionAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vRevolutionAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vRevolutionAngle.z);

	return (matRot[ROT_Y] * matRot[ROT_Z] * matRot[ROT_X]);
}

_matrix CTransform::GetTransMat(const _float& fTimeDelta)
{
	_vec3 vShake = { 0.0f,0.0f,0.0f };
	Update_Shake(fTimeDelta, vShake);

	// 위치 변환
	_matrix			matTrans;
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x + vShake.x, m_vInfo[INFO_POS].y + vShake.y, m_vInfo[INFO_POS].z + vShake.z);
	return matTrans;
}

_matrix CTransform::GetScaleMat()
{
	// 크기 변환
	_matrix matScale;
	//?
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	if (m_Parent != NULL)
	{
		D3DXMatrixScaling(&matScale,
			m_vScale.x * m_Parent->Get_Scale().x,
			m_vScale.y * m_Parent->Get_Scale().y,
			m_vScale.z * m_Parent->Get_Scale().z);
	}
	return matScale;
}

void CTransform::SwapYZ()
{
	for (int i = 0; i <GetChildCount(); i++)
		GetChild(i)->SwapYZ();

	m_matWorld._42 *= m_YZValue;
	m_matWorld._43 *= m_YZValue;

	if (!g_Is2D)
		m_YZValue = Lerp(m_YZValue, 0.8f, 0.1f);
	else
		m_YZValue = Lerp(m_YZValue, 1.f, 0.1f);
}

void CTransform::Floating_X(_float fPower)
{
	m_vInfo[INFO_POS].x += fPower * cosf(D3DXToRadian(m_fFloating));
}

void CTransform::Floating_Y(_float fPower)
{
	m_vInfo[INFO_POS].y += fPower * sinf(D3DXToRadian(m_fFloating));
}

void CTransform::Floating_Z(_float fPower)
{
	m_vInfo[INFO_POS].z += fPower * cosf(D3DXToRadian(m_fFloating));
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