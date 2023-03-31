#include "stdafx.h"
#include "Rigidbody.h"
#include "GameObject.h"
#include "Transform.h"

CRigidbody::CRigidbody(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_fGravity(0.f, -9.8f, 0.f)
	, m_Velocity(0.f, 0.f, 0.f)
	, m_InitialVelocity(0.f, 0.f, 0.f)
	, m_bUseGrivaty(true)
	, m_fMass(1.f)
	, m_Accele(0.f, 0.f, 0.f)
	, m_fAirResistance(1.f)
	, m_bFreezePos_X(false)
	, m_bFreezePos_Y(false)
	, m_bFreezePos_Z(false)
	, m_AngularForce(0.f,0.f,0.f)
	, m_AngularVelocity(0.f, 0.f, 0.f)
	, m_AngularAccele(0.f, 0.f, 0.f)
	, m_bFreezeRot_X(false)
	, m_bFreezeRot_Y(false)
	, m_bFreezeRot_Z(false)
{
}

CRigidbody::CRigidbody(const CRigidbody & rhs)
	:CComponent(rhs)
	, m_fGravity(rhs.m_fGravity)
	, m_Velocity(rhs.m_Velocity)
	, m_InitialVelocity(rhs.m_InitialVelocity)
	, m_bUseGrivaty(rhs.m_bUseGrivaty)
	, m_fMass(rhs.m_fMass)
	, m_Accele(rhs.m_Accele)
	, m_fAirResistance(rhs.m_fAirResistance)
	, m_bFreezePos_X(rhs.m_bFreezePos_X)
	, m_bFreezePos_Y(rhs.m_bFreezePos_Y)
	, m_bFreezePos_Z(rhs.m_bFreezePos_Z)
	, m_AngularForce(rhs.m_AngularForce)
	, m_AngularVelocity(rhs.m_AngularVelocity)
	, m_AngularAccele(rhs.m_AngularAccele)
	, m_bFreezeRot_Y(rhs.m_bFreezeRot_X)
	, m_bFreezeRot_Z(rhs.m_bFreezeRot_Y)
	, m_bFreezeRot_X(rhs.m_bFreezeRot_Z)
{
}


CRigidbody::~CRigidbody()
{
}

HRESULT CRigidbody::Ready_Rigidbody(void)
{
	return S_OK;
}

_int CRigidbody::Update_Component(const _float & fTimeDelta)
{
	//플레이어 위치 받아오기
	_vec3 transPos, originPos;
	m_pGameObject->m_pTransform->Get_Info(INFO_POS, &originPos);
	
	//공기저항
	AddTorque(m_AngularVelocity*m_fAirResistance*-1.f,FORCE,fTimeDelta);
	///////////회전//////////
	_vec3 rot = _vec3(0,0,0), originRot = _vec3(0, 0, 0);
	//입력받은 axis를 xyz로 나눌거임.
	originRot = m_pGameObject->m_pTransform->m_vAngle;
	//가속도
	m_AngularAccele = m_AngularForce * (1 / m_fMass);
	//속도
	m_AngularVelocity += m_AngularAccele * fTimeDelta;
	//x축 기준 회전
	rot += originRot + fTimeDelta * m_AngularVelocity;
	m_pGameObject->m_pTransform->m_vAngle = _vec3(
		  (!m_bFreezeRot_X) ? (rot.x) : (originRot.x)
		, (!m_bFreezeRot_Y) ? (rot.y) : (originRot.y)
		, (!m_bFreezeRot_Z) ? (rot.z) : (originRot.z)
		);

	///////////이동//////////////////
	//중력 f= m*g
	if(m_bUseGrivaty)
		AddForce(m_fGravity * m_fMass);

	//공기저항 f = -kv
	AddForce(m_Velocity*m_fAirResistance*-1.0f);

	//가속도  a =f/m
	m_Accele = m_Force * (1 / m_fMass);

	//속도  v = a*dt
	m_Velocity += m_Accele * fTimeDelta;

	//위치 pos2 = pos1 + dt*v
	transPos = originPos + fTimeDelta * m_Velocity;
		m_pGameObject->m_pTransform->Set_Pos(
			  (!m_bFreezePos_X)?(transPos.x) : (originPos.x)
			, (!m_bFreezePos_Y)?(transPos.y) : (originPos.y)
			, (!m_bFreezePos_Z)?(transPos.z) : (originPos.z));

	m_Force = _vec3(0.f, 0.f, 0.f);
	m_AngularForce = _vec3(0.f, 0.f, 0.f);
	return 0;
}

void CRigidbody::LateUpdate_Component(void)
{
}

void CRigidbody::AddTorque(_vec3 _axis, _float _force, FORCEMODE _mode, const _float & fTimeDelta)
{
	D3DXVec3Normalize(&_axis, &_axis);
	switch (_mode)
	{
		case FORCE://연속 질량 o
		{
			m_AngularForce += _axis*_force;
			break;
		}
		case ACCELERATION:
		{
			m_AngularForce += _axis*_force * m_fMass;
			break;
		}
		case IMPULSE:
		{
			m_AngularForce += _axis*_force / fTimeDelta;
			break;
		}
		case VELOCITYCHANGE:
		{
			m_AngularForce += _axis*_force * m_fMass / fTimeDelta;
			break;
		}
	}
}

void CRigidbody::AddTorque(_vec3 _force, FORCEMODE _mode, const _float & fTimeDelta)
{
	switch (_mode)
	{
		case FORCE://연속 질량 o
		{
			m_AngularForce += _force;
			break;
		}
		case ACCELERATION:
		{
			m_AngularForce += _force * m_fMass;
			break;
		}
		case IMPULSE:
		{
			m_AngularForce += _force / fTimeDelta;
			break;
		}
		case VELOCITYCHANGE:
		{
			m_AngularForce += _force * m_fMass / fTimeDelta;
			break;
		}
	}
}

void CRigidbody::AddForce(_vec3 _toward, _float _force, FORCEMODE _mode, const _float & fTimeDelta)
{
	//velocity를 수정해주는 친구로 보면 될듯.
	//특정 모드를 받아서 해당 모드에 맞게 힘을 주는 친구임.
	//지금은 모드 없이 이 친구로 힘들 주게 설정되어 있음.
	switch (_mode)
	{
		case FORCE://연속 질량 o
		{
			//받아온 방향벡터 단위벡터로 만들고
			D3DXVec3Normalize(&_toward, &_toward);

			//힘을 곱해서 넣어줌.
			m_Force += _toward * (_force);
			break;	
		}
		case ACCELERATION:
		{
			D3DXVec3Normalize(&_toward, &_toward);
			m_Force += _toward * (_force)*m_fMass;
			break;
		}
		case IMPULSE:
		{
			D3DXVec3Normalize(&_toward, &_toward);
			m_Force += _toward * (_force)/ fTimeDelta;
			break;
		}
		case VELOCITYCHANGE:
		{
			D3DXVec3Normalize(&_toward, &_toward);
			m_Force += _toward * (_force)*m_fMass / fTimeDelta;
			break;
		}
	}
}

void CRigidbody::AddForce(_vec3 _force, FORCEMODE _mode, const _float & fTimeDelta )
{
	switch (_mode)
	{
		case FORCE://연속 질량 o
		{
			m_Force += _force;
			break;
		}
		case ACCELERATION:
		{
			m_Force += _force * m_fMass;
			break;
		}
		case IMPULSE:
		{
			m_Force += _force / fTimeDelta;
			break;
		}
		case VELOCITYCHANGE:
		{
			m_Force += _force * m_fMass/fTimeDelta;
			break;
		}
	}
}

void CRigidbody::AddRelativeForce(_vec3 _toward, _float _force, FORCEMODE _mode, const _float & fTimeDelta)
{
	D3DXVec3Normalize(&_toward, &_toward);
	_vec3 transPos = _vec3(0, 0, 0);
	
	switch (_mode)
	{
		case FORCE://연속 질량 o
		{
			D3DXVec3TransformNormal(&transPos, &(_toward * (_force)), &(m_pGameObject->m_pTransform->m_matWorld));
			m_Force += transPos;
			break;
		}
		case ACCELERATION:
		{
			D3DXVec3TransformNormal(&transPos, &(_toward * (_force)), &(m_pGameObject->m_pTransform->m_matWorld));
			m_Force += transPos * m_fMass;
			break;
		}
		case IMPULSE:
		{
			D3DXVec3TransformNormal(&transPos, &(_toward * (_force)), &(m_pGameObject->m_pTransform->m_matWorld));
			m_Force += transPos / fTimeDelta;
			break;
		}
		case VELOCITYCHANGE:
		{
			D3DXVec3TransformNormal(&transPos, &(_toward * (_force)), &(m_pGameObject->m_pTransform->m_matWorld));
			m_Force += transPos * m_fMass / fTimeDelta;
			break;
		}
	}
}

void CRigidbody::AddRelativeForce(_vec3 _force, FORCEMODE _mode, const _float & fTimeDelta)
{
	_vec3 transPos = _vec3(0, 0, 0);
	switch (_mode)
	{
		case FORCE://연속 질량 o
		{
			D3DXVec3TransformNormal(&transPos, &_force, &(m_pGameObject->m_pTransform->m_matWorld));
			m_Force += transPos;
			break;
		}
		case ACCELERATION:
		{
			D3DXVec3TransformNormal(&transPos, &_force, &(m_pGameObject->m_pTransform->m_matWorld));
			m_Force += transPos * m_fMass;
			break;
		}
		case IMPULSE:
		{
			D3DXVec3TransformNormal(&transPos, &_force, &(m_pGameObject->m_pTransform->m_matWorld));
			m_Force += transPos / fTimeDelta;
			break;
		}
		case VELOCITYCHANGE:
		{
			D3DXVec3TransformNormal(&transPos, &_force, &(m_pGameObject->m_pTransform->m_matWorld));
			m_Force += transPos * m_fMass / fTimeDelta;
			break;
		}
	}
}

CRigidbody * CRigidbody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRigidbody* pInstance = new CRigidbody(pGraphicDev);

	if (FAILED(pInstance->Ready_Rigidbody()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent * CRigidbody::Clone(void)
{
	return new CRigidbody(*this);
}

void CRigidbody::Free(void)
{
	__super::Free();
}
