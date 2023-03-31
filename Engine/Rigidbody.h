#pragma once
#include "Component.h"

BEGIN(Engine)

typedef	enum FroceMode
{
	FORCE,//연속 + 질량무시 X
	ACCELERATION, //  연속 + 질량무시 O
	IMPULSE, //불연속 + 질량무시 X
	VELOCITYCHANGE // 불연속 + 질량무시 O
}FORCEMODE;

typedef enum WorldAxis
{
	WORLDAXIS_X, //X축 
	WORLDAXIS_Y, //Y축
	WORLDAXIS_Z  //Z축
}WORLDAXIS;

class CRigidbody :
	public CComponent
{
private:
	explicit CRigidbody(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRigidbody(const CRigidbody& rhs);
	virtual ~CRigidbody();

public:
	//리지드 바디 초기설정 함수.
	HRESULT Ready_Rigidbody(void);

	//컴포넌트 업데이트
	virtual _int Update_Component(const _float& fTimeDelta);

	//컴포넌트 late업데이트
	virtual void LateUpdate_Component(void);

public:
	//힘을 가하는 함수 //방향,힘,모드,시간(충격효과때만 사용함)을 입력받음.
	//월드
	void AddForce(_vec3 _toward, _float _force, FORCEMODE _mode = FORCE, const _float & fTimeDelta=1);
	void AddForce(_vec3 _force, FORCEMODE _mode = FORCE, const _float & fTimeDelta =1);
	//로컬
	void AddRelativeForce(_vec3 _toward, _float _force, FORCEMODE _mode = FORCE, const _float & fTimeDelta = 1);
	void AddRelativeForce(_vec3 _force, FORCEMODE _mode = FORCE, const _float & fTimeDelta = 1);

	//회전을 시키는 함수 //축,힘,모드,시간(충격효과떄만 사용함)을 입력받음.
	//로컬
	void AddTorque(_vec3 _axis, _float _force, FORCEMODE _mode = FORCE, const _float & fTimeDelta = 1);
	void AddTorque(_vec3 _force, FORCEMODE _mode = FORCE, const _float & fTimeDelta = 1);

public:
	/////////이동///////////// 
	//힘
	_vec3 m_Force;
	//현재 속도
	_vec3 m_Velocity;
	//초기속도
	_vec3 m_InitialVelocity;
	//가속도
	_vec3 m_Accele;

	//////////회전///////////
	//x,y,z각 힘
	_vec3 m_AngularForce;
	//각속도
	_vec3 m_AngularVelocity;
	//각가속도
	_vec3 m_AngularAccele;

	////////공통////////////
	//중력 변수 (중력은 설정 나름)
	_vec3 m_fGravity;
	//질량
	_float m_fMass;
	//공기저항
	_float m_fAirResistance;

	////////옵션///////////
	//중력 적용 여부
	_bool m_bUseGrivaty;
	//rigidbody로 인한 포지션 변환 고정 여부
	_bool m_bFreezePos_X;
	_bool m_bFreezePos_Y;
	_bool m_bFreezePos_Z;
	//rigidbody로 인한 로테이션 변환 고정 여부
	_bool m_bFreezeRot_X;
	_bool m_bFreezeRot_Y;
	_bool m_bFreezeRot_Z;

public:
	static CRigidbody* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent * Clone(void) override;

private:
	virtual void Free(void) override;
};

END