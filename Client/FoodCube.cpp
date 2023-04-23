#include "stdafx.h"
#include "Export_Function.h"
#include "FoodCube.h"

CFoodCube::CFoodCube(LPDIRECT3DDEVICE9 pGraphicDev):CMoveCube(pGraphicDev)
{
}

CFoodCube::~CFoodCube()
{
}

HRESULT CFoodCube::Ready_GameObject(_vec3 & vPos)
{
	__super::Ready_GameObject(vPos);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_bIsStatic = false;
	m_pCollider->m_bIsTrigger = false;
	m_pCollider->Set_BoundingBox({ 2.f,2.f,2.f });
	return S_OK;
}

_int CFoodCube::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(0.02f);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

_int CFoodCube::Update_Too(const _float & fTimeDelta)
{
	__super::Update_Too(fTimeDelta);
	return 0;
}

_int CFoodCube::Update_Top(const _float & fTimeDelta)
{
	__super::Update_Top(fTimeDelta);
	return 0;
}

void CFoodCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CFoodCube::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CFoodCube::SwapTrigger()
{

}

void CFoodCube::OnCollisionEnter(const Collision * collision)
{
	if (collision->_dir == DIR_DOWN)
	{
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"54.wav", SOUND_EFFECT_GIMMICK, 1.f);
	}

	__super::OnCollisionEnter(collision);
}

void CFoodCube::OnCollisionStay(const Collision * collision)
{
	CTransform* trans_other = collision->otherObj->m_pTransform;
	CCollider* collider_other = collision->otherCol;
	if (collider_other->m_bIsTrigger)
		return;
	//현재 게임 오브젝트의 콜라이더를 가져옵니다.
	CCollider* collider_this = dynamic_cast<CCollider*>(this->Get_Component(L"Collider", ID_DYNAMIC));

	if (trans_other->m_bIsStatic)
		return;

	//각 콜라이더의 충돌 영역을 가져옴.
	_vec3 center_this = collider_this->Get_BoundCenter();
	_vec3 size_this = collider_this->Get_BoundSize();
	_vec3 center_other = collider_other->Get_BoundCenter();
	_vec3 size_other = collider_other->Get_BoundSize();

	// 콜라이더 자체 오프셋
	_vec3 offset_other = collider_other->Get_BoundOffset();

	//충돌 영역을 이용한 위치 보정값
	_float min_x = center_this.x - (size_this.x*0.5f + size_other.x*0.5f);
	_float min_y = center_this.y - (size_this.y*0.5f + size_other.y*0.5f);
	_float max_y = center_this.y + (size_this.y*0.5f + size_other.y*0.5f);
	_float max_x = center_this.x + (size_this.x*0.5f + size_other.x*0.5f);


	if ((collision->_dir == DIR_LEFT&&trans_other->m_vInfo[INFO_POS].x + offset_other.x <= min_x) ||
		(collision->_dir == DIR_RIGHT&&trans_other->m_vInfo[INFO_POS].x + offset_other.x >= max_x) ||
		(collision->_dir == DIR_DOWN&&trans_other->m_vInfo[INFO_POS].y + offset_other.y <= min_y) ||
		(collision->_dir == DIR_UP&&trans_other->m_vInfo[INFO_POS].y + offset_other.y >= max_y)
		)
		return;

	if (collision->_dir == DIR_UP &&center_other.y > min_y)
		trans_other->m_vInfo[INFO_POS].y = max_y - offset_other.y;
	else if (collision->_dir == DIR_DOWN&&center_other.y < max_y)
		trans_other->m_vInfo[INFO_POS].y = min_y - offset_other.y;
	else if (collision->_dir == DIR_LEFT&&center_other.x < max_x)
		trans_other->m_vInfo[INFO_POS].x = min_x - offset_other.x;
	else if (collision->_dir == DIR_RIGHT&&center_other.x > min_x)
		trans_other->m_vInfo[INFO_POS].x = max_x - offset_other.x;

	//나랑 충돌한 물체가 리짓바디를 가지고있지 않다면 실행 X
	CRigidbody* _rigid;

	if ((collision->otherObj->Get_Component(L"Rigidbody", ID_DYNAMIC)) == nullptr) return;
	_rigid = dynamic_cast<CRigidbody*>(collision->otherObj->Get_Component(L"Rigidbody", ID_DYNAMIC));

	//velocity / trans 둘다 밀어내기를 진행해줘야함.
	_vec3 reaction = _vec3(0, 0, 0);

	switch (collision->_dir)
	{
	case DIR_UP:
		if (_rigid->m_Velocity.y < 0)
			reaction = _vec3(0, _rigid->m_Velocity.y, 0);
		_rigid->m_Velocity.x *= 0.8f;
		break;
	case DIR_DOWN:
		if (_rigid->m_Velocity.y > 0)
			reaction = _vec3(0, _rigid->m_Velocity.y, 0);
		_rigid->m_Velocity.x *= 0.8f;
		break;
	case DIR_LEFT:
		if (_rigid->m_Velocity.x > 0)
			reaction = _vec3(_rigid->m_Velocity.x, 0, 0);
		break;
	case DIR_RIGHT:
		if (_rigid->m_Velocity.x < 0)
			reaction = _vec3(_rigid->m_Velocity.x, 0, 0);
		break;
	}
	_rigid->m_Velocity -= reaction;
}

void CFoodCube::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

CFoodCube * CFoodCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CFoodCube*		pInstance = new CFoodCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFoodCube::Free(void)
{
	__super::Free();
}

HRESULT CFoodCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"PigCube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"PigCube", pComponent });

	
	return S_OK;
}
