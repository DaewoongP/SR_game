#include "stdafx.h"
#include "Boss2.h"
#include "Export_Function.h"

CBoss2::CBoss2(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CGameObject(pGraphicDev)
{
}


CBoss2::~CBoss2()
{
}

HRESULT CBoss2::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eCurrentState = B2_IDLE;
	m_ePreState = B2_END;
	m_bInit = false;

	m_fJumpPos[0] = _vec3(10,25,10);
	m_fJumpPos[1] = _vec3(30,25,10);
	m_fJumpPos[2] = _vec3(50,25,10);
	m_iJumpPosidx = 0;
	ReadyPartten();

	m_pTransform->m_vScale = { 3.f, 3.f, 3.f };
	m_pCollider->Set_BoundingBox(_vec3(4, 4, 2));
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pRigid->m_bUseGrivaty = false;

	return S_OK;
}

_int CBoss2::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	//�÷��̾� ��������
	if (!m_bInit)
	{
		FAILED_CHECK_RETURN(Find_PlayerBoth(), -1);
		m_bInit = true;
	}
	//���� ����
	(this->*funcAction[m_eCurrentState][m_iCurrentActionIdx])(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

_int CBoss2::Update_Too(const _float & fTimeDelta)
{
	return 0;
}

_int CBoss2::Update_Top(const _float & fTimeDelta)
{
	return 0;
}

void CBoss2::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss2::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2::Render_Too(void)
{
}

void CBoss2::Render_Top(void)
{
}

void CBoss2::SwapTrigger()
{
}

void CBoss2::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CBoss2::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CBoss2::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

HRESULT CBoss2::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Head", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

HRESULT CBoss2::Find_PlayerBoth()
{
	m_pPlayer01_trans = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Toodee", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPlayer01_trans, -1);
	m_pPlayer02_trans = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Topdee", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPlayer02_trans, -1);
	return S_OK;
}

void CBoss2::Do_Stump()
{
}

void CBoss2::Do_Jump_Ready(const _float& fTimeDelta)
{
	if (m_iJumpPosidx == 0)
		if (rand() % 2 == 0)
			m_iJumpPosidx = m_iJumpPosidx;
		else
			m_iJumpPosidx++;
	else if (m_iJumpPosidx == 1)
		if (rand() % 3 == 0)
			m_iJumpPosidx--;
		else if (rand() % 3 == 1)
			m_iJumpPosidx = m_iJumpPosidx;
		else
			m_iJumpPosidx++;
	else
		if (rand() % 2 == 0)
			m_iJumpPosidx--;
		else
			m_iJumpPosidx = m_iJumpPosidx;
	m_iCurrentActionIdx++;
}

void CBoss2::Do_Jump_01(const _float& fTimeDelta)
{
	_vec3 _dir, originlen;
	originlen = _vec3(m_fJumpPos[m_iJumpPosidx] - m_pTransform->m_vInfo[INFO_POS]);
	D3DXVec3Normalize(&_dir, &originlen);
	m_pTransform->m_vInfo[INFO_POS] += _dir * fTimeDelta * 40;
	if (D3DXVec3Length(&originlen) <1.f)
	{
		//���� �ൿ���� ����
		m_iCurrentActionIdx++;
	}
}

void CBoss2::Do_Jump_02(const _float& fTimeDelta)
{
	_vec3 _dir, originlen;
	originlen = _vec3(m_fJumpPos[m_iJumpPosidx] - m_pTransform->m_vInfo[INFO_POS]);
	D3DXVec3Normalize(&_dir, &originlen);
	m_pTransform->m_vInfo[INFO_POS] += _dir * fTimeDelta * 40;
	if (D3DXVec3Length(&originlen) <1.f)
	{
		//���� �ൿ���� ����
		m_iCurrentActionIdx++;
	}
}

void CBoss2::Do_Scream()
{
}

void CBoss2::Do_Punch()
{
}

void CBoss2::Do_Idle()
{
}

_bool CBoss2::SetPartten()
{
	//100 ������ ���� ���� ����
	int ranIdx = (rand()+1) % 100;

	switch (ranIdx)
	{
	case 0: //30% idle
		if (m_eCurrentState != B2_IDLE)
		{
			m_ePreState = m_eCurrentState;
			m_eCurrentState = B2_IDLE;
		}
		else
			return false;
		break;
	case 30: //40% jump
		m_ePreState = m_eCurrentState;
		m_eCurrentState = B2_JUMPING;
		break;
	case 70: //10% scream
		if (m_eCurrentState != B2_SCREAM)
		{
			m_ePreState = m_eCurrentState;
			m_eCurrentState = B2_SCREAM;
		}
		else
			return false;
		break;
	case 80: // 10% punch
		if (m_eCurrentState != B2_PUNCH)
		{
			m_ePreState = m_eCurrentState;
			m_eCurrentState = B2_PUNCH;
		}
		else
			return false;
		break;
	case 90: // 10% stump
		if (m_eCurrentState != B2_STUMP)
		{
			m_ePreState = m_eCurrentState;
			m_eCurrentState = B2_STUMP;
		}
		else
			return false;
		break;
	}
	//���� ������Ʈ ���� idx�� ����
	m_eCurrentState = (BOSS2STATE)ranIdx;
	return true;
}

void CBoss2::ReadyPartten()
{
	//������� Ȯ��
	funcAction.reserve(B2_END);
	
	BOSS2_STATE_FUNC func;
	func.push_back(&CBoss2::Do_Jump_Ready);
	func.push_back(&CBoss2::Do_Jump_01);
	func.push_back(&CBoss2::Do_Jump_02);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Jump_01);
	func.push_back(&CBoss2::Do_Jump_02);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Jump_01);
	func.push_back(&CBoss2::Do_Jump_02);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Jump_01);
	func.push_back(&CBoss2::Do_Jump_02);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Jump_01);
	func.push_back(&CBoss2::Do_Jump_02);
	funcAction.push_back(func);
	func.clear();
}

CBoss2 * CBoss2::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2*		pInstance = new CBoss2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2::Free(void)
{
	__super::Free();
}
