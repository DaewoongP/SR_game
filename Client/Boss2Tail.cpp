#include "stdafx.h"
#include "Boss2Tail.h"
#include "Boss2.h"

CBoss2Tail::CBoss2Tail(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_pPreTail(nullptr)
{
}

CBoss2Tail::~CBoss2Tail()
{
}

HRESULT CBoss2Tail::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;

	m_pRigid->m_bUseGrivaty = false;
	m_pRigid->m_bUseLimitVelocity = true;
	m_pRigid->m_fLimitVelocity = 8.0f;
	m_pRigid->m_bFreezePos_Z = true;

	return S_OK;
}

_int CBoss2Tail::Update_GameObject(const _float & fTimeDelta)
{
	//m_pRigid->m_fLimitVelocity = TAILVELOCITYLIMT;


	if (m_pPreTail)
	{
		//ù �θ��� y �� �Ǵ� z �� ȸ�� ���� ����ȭ ��ŭ ���ϸ� ȸ������ ��������
		_matrix matBoss = dynamic_cast<CBoss2*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2"))->m_pTransform->m_matWorld;
		_vec3 vBossX = { matBoss._11,matBoss._12,matBoss._13 };
		_float fYDis = D3DXVec3Length(&vBossX);
		vBossX /= fYDis;

		_vec3 vPrePos = {
			m_pPreTail->Get_WorldMatrixPointer()->_41 + vBossX.x * 0.5f,//offset ������ �� �غ��� �ҵ�...
			m_pPreTail->Get_WorldMatrixPointer()->_42,
			m_pPreTail->Get_WorldMatrixPointer()->_43 };
		//������ �̵� ���� �� �Ÿ� ����
		_vec3 vDis = vPrePos - m_pTransform->m_vInfo[INFO_POS];
		//�Ÿ� ����
		_float fLength = D3DXVec3Length(&vDis);
		//�������� ���
		D3DXVec3Normalize(&vDis, &vDis);
		
		m_pRigid->AddForce(vDis, 1200.f, FORCE, fTimeDelta);
		
		vDis.z = 0.0f;

		if (0.7f < fLength)
		{
			_vec3 vRePosition = vPrePos - vDis;
			
			vRePosition.z = 0.0f;

			m_pTransform->Set_Pos(vRePosition.x, vRePosition.y, m_pTransform->m_vInfo[INFO_POS].z);
		}
	}

__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	/*if (m_bMetronome)
	{
		if (1.0f > m_fTimer)
		{
			m_fTimer += fTimeDelta;

			if (1.0f < m_fTimer)
				m_fTimer = 1.0f;

			m_pTransform->m_vAngle.z = D3DXToRadian(Lerp(BOSETAILPARTWAVESTART, BOSETAILPARTWAVEEND, m_fTimer));

			if (1.0f <= m_fTimer)
			{
				m_fTimer = 0.0f;

				m_bMetronome = false;
			}

		}
	}
	else if (!m_bMetronome)
	{
		if (1.0f > m_fTimer)
		{
			m_fTimer += fTimeDelta;

			if (1.0f < m_fTimer)
				m_fTimer = 1.0f;

			m_pTransform->m_vAngle.z = D3DXToRadian(Lerp(BOSETAILPARTWAVEEND, BOSETAILPARTWAVESTART, m_fTimer));

			if (1.0f <= m_fTimer)
			{
				m_fTimer = 0.0f;

				m_bMetronome = true;
			}
		}
	}*/

	return 0;
}

_int CBoss2Tail::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Tail::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Tail::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss2Tail::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Tail::Render_Too(void)
{
}

void CBoss2Tail::Render_Top(void)
{
}

HRESULT CBoss2Tail::Add_Component(void)
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

	return S_OK;
}

CBoss2Tail * CBoss2Tail::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Tail*		pInstance = new CBoss2Tail(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Tail::Free(void)
{
	__super::Free();
}
