#include "stdafx.h"
#include "MiniGameBat.h"
#include "Cube.h"
#include "GravityCube.h"
#include "CrackCube.h"
#include "Export_Function.h"
#include "AbstractFactory.h"
#include "StageCamera.h"
#include "MoveCube.h"

CMiniGameBat::CMiniGameBat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev), m_bBackSprite(false)
{
}

CMiniGameBat::~CMiniGameBat()
{
}

HRESULT CMiniGameBat::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 10.0f;
	m_pTransform->m_vScale = { -BATSCALE, BATSCALE, 5.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;
	// 리지드 바디
	m_pRigid->m_bUseGrivaty = false;
	m_pRigid->m_bUseLimitVelocity = true;
	m_pRigid->m_fLimitVelocity = 10.0f;
	m_pRigid->m_bFreezePos_Z = true;
	// 기본 텍스처
	m_pTextureCom->Add_Anim(L"Idle", 0, 6, 0.6f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	// back 텍스처
	m_pTextureCom_Back->Add_Anim(L"Idle", 0, 6, 0.6f, true);
	m_pTextureCom_Back->Switch_Anim(L"Idle");
	m_pTextureCom_Back->m_bUseFrameAnimation = true;

	m_pCollider->Set_Options({ 2.f, 2.f, 4.f }, COL_OBJ, true);

	m_pTransform->m_vInfo[INFO_POS].z = BATTOOZ;

	m_vecCol.push_back(L"MapCube");
	m_vecCol.push_back(L"InstallCube");
	m_vecCol.push_back(L"CrackCube");
	m_vecCol.push_back(L"SwitchCube");
	m_vecCol.push_back(L"MoveCube");
	m_vecCol.push_back(L"GravityCube");
	m_vecCol.push_back(L"KeyCube");

	m_bInit = true;
	return S_OK;
}

_int CMiniGameBat::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bInit)
	{
		//상자를 들고 시작하게 해줍니다.
		m_bInit = false;
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);
		int _rand = rand() % 4;
		switch (_rand)
		{
		case 0:
			FACTORY<CCube>::Create(L"MapCube", pStageLayer, _vec3(0, -1, 0), m_pTransform);
			break;
		case 1:
			FACTORY<CMoveCube>::Create(L"MoveCube", pStageLayer, _vec3(0, -1, 0), m_pTransform);
			break;
		case 2:
			FACTORY<CGravityCube>::Create(L"GravityCube", pStageLayer, _vec3(0, -1, 0), m_pTransform);
			dynamic_cast<CGravityCube*>(m_pTransform->GetChild(0)->m_pGameObject)->SetGravity(false);
			break;
		case 3:
			FACTORY<CCrackCube>::Create(L"CrackCube", pStageLayer, _vec3(0, -1, 0), m_pTransform);
			break;
		}
		
		m_GrabCube = dynamic_cast<CCube*>(m_pTransform->GetChild(0)->m_pGameObject);
		m_GrabCube->m_pTransform->m_vScale = _vec3(0.2f, 0.2f, 0.2f);

		m_pCollider->m_bIsTrigger = true;
		m_bMoveLeft = (rand() % 2 == 0 ? true : false);
		m_pTransform->m_vInfo[INFO_POS] = _vec3(40, 30, 10);
	}
	//위치 영역 이탈시 사망처리
	if (2.0f * (CUBEX - 1) + 1 < m_pTransform->m_vInfo[INFO_POS].x ||
		m_pTransform->m_vInfo[INFO_POS].x < 0 ||
		2.0f * (CUBEY - 1) < m_pTransform->m_vInfo[INFO_POS].y ||
		m_pTransform->m_vInfo[INFO_POS].y < 0)
	{
		m_bDead = true;
		if (m_GrabCube != nullptr)
			m_GrabCube->Set_Dead();
	}

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

_int CMiniGameBat::Update_Too(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	m_bBackSprite = false;
	m_pRigid->m_Velocity = { 0.0f, 0.0f, 0.0f };
	_vec3 MoveDir = { 1.0f,0.0f,0.0f };
	if (m_bMoveLeft)
	{
		m_pTransform->m_vScale.x = BATSCALE;
		if (m_GrabCube != nullptr)m_GrabCube->m_pTransform->m_vScale.x = 0.2f;
	}
	else if (!m_bMoveLeft)
	{
		m_pTransform->m_vScale.x = -BATSCALE;
		if (m_GrabCube != nullptr)m_GrabCube->m_pTransform->m_vScale.x = -0.2f;
		MoveDir *= -1;
	}

	m_pTransform->Move_Pos(&MoveDir, fTimeDelta, m_fSpeed);

	if (m_pTransform->m_vInfo[INFO_POS].z != BATTOOZ)
	{
		m_fTimer += fTimeDelta * 4.0f;

		if (1.0f <= m_fTimer)
		{
			m_fTimer = 1.0f;
		}

		m_pTransform->m_vInfo[INFO_POS].z = Lerp(BATTOPZ, BATTOOZ, m_fTimer);

		if (1.0f == m_fTimer)
		{
			m_fTimer = 0.0f;
		}
	}
	return 0;
}

_int CMiniGameBat::Update_Top(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Topdee", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	if (vPlayerPos.y > m_pTransform->m_vInfo[INFO_POS].y)
		m_bBackSprite = true;
	else
		m_bBackSprite = false;

	if (vPlayerPos.x < m_pTransform->m_vInfo[INFO_POS].x)
	{
		m_pTransform->m_vScale.x = -BATSCALE;
		if (m_GrabCube != nullptr)m_GrabCube->m_pTransform->m_vScale.x  =-0.2f;
	}
	else
	{
		m_pTransform->m_vScale.x = BATSCALE;
		if (m_GrabCube != nullptr)m_GrabCube->m_pTransform->m_vScale.x = 0.2f;
	}
		

	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &(vPlayerPos - m_pTransform->m_vInfo[INFO_POS]));

	if (m_GrabCube != nullptr)
	{
		m_pRigid->AddForce(vDir, m_fSpeed * 10.f, FORCE, fTimeDelta);
	}
	else 
	{
		_vec3 MoveDir = { 1.0f,0.0f,0.0f };
		if(!m_bMoveLeft)
			MoveDir *= -1;
		m_pTransform->Move_Pos(&MoveDir, fTimeDelta, m_fSpeed);
	}
	
	m_pTextureCom_Back->Update_Anim(fTimeDelta);
	m_bStart2D = true;

	if (m_pTransform->m_vInfo[INFO_POS].z != BATTOPZ)
	{
		m_fTimer += fTimeDelta * 4.0f;
		if (1.0f <= m_fTimer)
			m_fTimer = 1.0f;
		m_pTransform->m_vInfo[INFO_POS].z = Lerp(BATTOOZ, BATTOPZ, m_fTimer);
		if (1.0f == m_fTimer)
			m_fTimer = 0.0f;
	}

	if (Engine::Get_DIKeyState(DIK_A) == Engine::KEYDOWN&&m_GrabCube!=nullptr)
	{
		//상자를 놓고 런친다.
		m_GrabCube->m_pTransform->SetParent(nullptr);
		m_GrabCube->m_pTransform->m_vInfo[INFO_POS].x=
			(_int)m_pTransform->m_vInfo[INFO_POS].x/2*2;
		m_GrabCube->m_pTransform->m_vInfo[INFO_POS].y =
			(_int)m_pTransform->m_vInfo[INFO_POS].y / 2 * 2;
		m_GrabCube->m_pTransform->m_vInfo[INFO_POS].z =10;
		m_GrabCube->m_pTransform->m_vScale = _vec3(1, 1, 1);
		m_pCollider->Set_BoundOffset(_vec3(0, 0, -30));
		if(dynamic_cast<CGravityCube*>(m_pTransform->GetChild(0)->m_pGameObject))
		dynamic_cast<CGravityCube*>(m_pTransform->GetChild(0)->m_pGameObject)->SetGravity(true);
		m_pCollider->Set_BoundingBox({ 0,0,0 });
		m_GrabCube = nullptr;
	}

	return 0;
}

void CMiniGameBat::LateUpdate_Too()
{
}

void CMiniGameBat::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CMiniGameBat::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CMiniGameBat::SwapTrigger()
{
	if (g_Is2D)
		m_pCollider->Set_BoundingBox({ 2.f, 2.f, 4.f }, { 0.f, 0.f, 0.f });
	else
		m_pCollider->Set_BoundingBox({ 2.f, 2.f, 4.f }, { 0.f, 0.f, 4.f });
}

void CMiniGameBat::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);

	m_pShadowCom->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
}

void CMiniGameBat::Render_Top()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	if (m_bBackSprite)
	{
		m_pTextureCom_Back->Set_Texture(0);
	}
	else
		m_pTextureCom->Set_Texture(0);

	m_pShadowCom->Render_Shadow(m_pBufferCom);


	m_pBufferCom->Render_Buffer();
}

void CMiniGameBat::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CMiniGameBat::OnCollisionStay(const Collision * collision)
{
	//만약 충돌체가 박쥐면 강제로 밀어준다.
	if (!lstrcmp(collision->otherObj->m_pTag, L"MiniGameBat"))
	{
		CTransform* trans_other = collision->otherObj->m_pTransform;
		CCollider* collider_other = collision->otherCol;
		CCollider* collider_this = dynamic_cast<CCollider*>(this->Get_Component(L"Collider", ID_DYNAMIC));

		_vec3 center_this = collider_this->Get_BoundCenter();
		_vec3 size_this = collider_this->Get_BoundSize();
		_vec3 center_other = collider_other->Get_BoundCenter();
		_vec3 size_other = collider_other->Get_BoundSize();

		_vec3 offset_other = collider_other->Get_BoundOffset();

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

		CRigidbody* _rigid;
		if ((collision->otherObj->Get_Component(L"Rigidbody", ID_DYNAMIC)) == nullptr) return;
		_rigid = dynamic_cast<CRigidbody*>(collision->otherObj->Get_Component(L"Rigidbody", ID_DYNAMIC));

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
	__super::OnCollisionStay(collision);
}

void CMiniGameBat::OnCollisionExit(const Collision * collision)
{
	if (dynamic_cast<CCube*>(collision->otherObj))
	{
		m_pShadowCom->m_fShadowHeight = 11.0f;
	}
	__super::OnCollisionExit(collision);
}

HRESULT CMiniGameBat::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom_Back = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bat_Back_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom_Back, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Bat_Back_Texture", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bat_Front_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Bat_Front_Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pShadowCom = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Shadow", pComponent });

	m_pTransform->m_bIsStatic = false;
	return S_OK;
}

CMiniGameBat * CMiniGameBat::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CMiniGameBat*		pInstance = new CMiniGameBat(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniGameBat::Free(void)
{
	__super::Free();
}
