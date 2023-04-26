#include "stdafx.h"
#include "GravityCube.h"
#include "Export_Function.h"
#include "CrackCube.h"

CGravityCube::CGravityCube(LPDIRECT3DDEVICE9 pGraphicDev) :CMoveCube(pGraphicDev)
{
}

CGravityCube::~CGravityCube()
{
}

HRESULT CGravityCube::Ready_GameObject(_vec3 & vPos)
{
	__super::Ready_GameObject(vPos);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pRigid->m_bUseLimitVelocity = true;
	m_pRigid->m_fLimitVelocity = 30.f;
	m_bUseGraivty = false;
	m_pRigid->m_bUseGrivaty = false;
	m_pTransform->m_bIsStatic = false;
	m_pCollider->m_bIsTrigger = false;
	m_pCollider->Set_BoundingBox({ 2.f,2.f,2.f });
	return S_OK;
}

_int CGravityCube::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

_int CGravityCube::Update_Too(const _float & fTimeDelta)
{
	if (!m_bIsFall)
		Do_CheckRay_Down();
	__super::Update_Too(fTimeDelta);
	return 0;
}

_int CGravityCube::Update_Top(const _float & fTimeDelta)
{
	__super::Update_Top(fTimeDelta);
	return 0;
}

void CGravityCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CGravityCube::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CGravityCube::SwapTrigger()
{
	m_pRigid->m_Velocity = _vec3(0, 0, 0);

	if (!g_Is2D)
	{
		m_pTransform->m_vInfo[INFO_POS].x =
			((int)m_pTransform->m_vInfo[INFO_POS].x % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].x) : ((int)m_pTransform->m_vInfo[INFO_POS].x + 1);
		m_pTransform->m_vInfo[INFO_POS].y =
			((int)m_pTransform->m_vInfo[INFO_POS].y % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].y) : ((int)m_pTransform->m_vInfo[INFO_POS].y + 1);
		m_pRigid->m_bUseGrivaty = false;
	}
	else if(!m_bIsStone){
		m_pRigid->m_bUseGrivaty = true;
	}
}

void CGravityCube::OnCollisionEnter(const Collision * collision)
{
	if (lstrcmp(collision->otherObj->m_pTag, L"InstallGrid")&&
		lstrcmp(collision->otherObj->m_pTag, L"Portal")&&
		collision->_dir == DIR_DOWN)
	{
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"54.wav", SOUND_EFFECT_GIMMICK, 1.f);
	}

	__super::OnCollisionEnter(collision);
}

void CGravityCube::OnCollisionStay(const Collision * collision)
{
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
}

void CGravityCube::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

CGravityCube * CGravityCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CGravityCube*		pInstance = new CGravityCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGravityCube::Do_CheckRay_Down()
{
	if (m_bIsStone)
		return;
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(m_pTransform->m_vInfo[INFO_POS], _vec3(0, -1, 0), 1.01f), m_pCollider);
	for (int i = 0; i < _detectedCOL.size(); i++)
	{
		if (dynamic_cast<CCube*>(_detectedCOL[i].col->m_pGameObject))
		{
			if (!lstrcmp(_detectedCOL[i].tag, L"CrackCube") &&
				!dynamic_cast<CCrackCube*>(_detectedCOL[i].col->m_pGameObject)->GetCrackDead())
			{
				dynamic_cast<CCrackCube*>(_detectedCOL[i].col->m_pGameObject)->DoShootRay(DIR_DOWN);
			}
			m_pTransform->m_bIsStatic = true;
			m_pRigid->m_bUseGrivaty = false;
			return;
		}
	}
	m_pTransform->m_bIsStatic = false;
	m_pRigid->m_bUseGrivaty = true;
}

void CGravityCube::Free(void)
{
	__super::Free();
}

HRESULT CGravityCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Gravity_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Gravity_Cube", pComponent });


	return S_OK;
}