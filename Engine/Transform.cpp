#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
	,m_bMove(true)
	, m_Parent(NULL)
	, m_Child(NULL)

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
	, m_Parent(rhs.m_Parent)
	, m_Child(rhs.m_Child)
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
	this;
	//매트릭스 생성
	m_matWorld = matScale * m_matBillY * m_matBillX * matRotation * matTrans;
	_tchar*	 name = m_pGameObject->m_pTag;
	
	if (m_Parent == NULL)
		return;

	//스자이 공(부모의 회전행렬)부(부모의 이동행렬)
	m_matWorld = matScale * m_matRT;
	//이거랑 같음
	//부모의 스케일만큼 곱해줌.
	/*_vec3 x = _vec3(m_matWorld._11, m_matWorld._12, m_matWorld._13) * m_Parent->Get_Scale().x;
	_vec3 y = _vec3(m_matWorld._21, m_matWorld._22, m_matWorld._23) * m_Parent->Get_Scale().y;
	_vec3 z = _vec3(m_matWorld._31, m_matWorld._32, m_matWorld._33) * m_Parent->Get_Scale().z;

	m_matWorld._11 = x.x; m_matWorld._12 = x.y; m_matWorld._13 = x.z;
	m_matWorld._21 = y.x; m_matWorld._22 = y.y; m_matWorld._23 = y.z;
	m_matWorld._31 = z.x; m_matWorld._32 = z.y; m_matWorld._33 = z.z;*/
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
	MakeMyMatrix(fTimeDelta);
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

_matrix CTransform::GetRotMat()
{
	// 회전 변환
	_matrix			matRot[ROT_END];
	_matrix			matRotation;

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	return ( matRot[ROT_Y] * matRot[ROT_Z] * matRot[ROT_X]);
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
