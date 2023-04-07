#include "stdafx.h"
#include "CrackBlock.h"

#include"..\Engine\Export_Function.h"

CCrackBlock::CCrackBlock(LPDIRECT3DDEVICE9 pGraphicDev):CCube(pGraphicDev)
{
}

CCrackBlock::~CCrackBlock()
{
}

HRESULT CCrackBlock::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_vInfo[INFO_POS] = _vec3(10.f, 3.f, 10.f);
	m_pCollider->Set_Group(COL_OBJ);


	return S_OK;
}

_int CCrackBlock::Update_GameObject(const _float& fTimeDelta)
{
	return S_OK();
}

_int CCrackBlock::Update_Too(const _float& fTimeDelta)
{
	if (m_bCrackDead)
	{
		m_fBlockTime -= fTimeDelta;
		_vec3 ShakePos;
		ShakePos = m_pTransform->m_vInfo[INFO_POS];
		Shaking(ShakePos, fTimeDelta);
		m_pTransform->m_vInfo[INFO_POS];
		//m_pTransform->m_vInfo[INFO_POS]=Shaking(fTimeDelta);

		if (m_fBlockTime <= 0.f)
			return OBJ_DEAD;
	}

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

_int CCrackBlock::Update_Top(const _float& fTimeDelta)
{
	if (m_fBlockTime <= 0.f)
		return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CCrackBlock::LateUpdate_GameObject(void)
{
}

void CCrackBlock::LateUpdate_Too()
{
	CGameObject::LateUpdate_GameObject();
}

void CCrackBlock::LateUpdate_Top()
{
	CGameObject::LateUpdate_GameObject();
}

void CCrackBlock::Render_GameObject(void)
{
}

void CCrackBlock::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CCrackBlock::Render_Top()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CCrackBlock::OnCollisionEnter(const Collision* collision)
{

	//부딪힌게 2d인지 topd인지 판단할것이 필요 layer에 map object사용하면좋을듯
	//CGameObject* pPlayer = Get_GameObject(L"Layer_GameLogic", L"Player");
	
	
	/*for (auto& iter : m_uMapComponent[ID_DYNAMIC])
			if(iter.first == L"Player")*/
				m_bCrackDead = true;

		//m_uMapComponent[ID_DYNAMIC].-> == L"Player")
		

}

void CCrackBlock::OnCollisionStay(const Collision* collision)
{
}

void CCrackBlock::OnCollisionExit(const Collision* collision)
{
}

HRESULT CCrackBlock::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"CrackBlock_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CrackBlock_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 1.99f,1.99f,1.99f });
	
	return S_OK;
}



void CCrackBlock::Shaking(_vec3& vPos, const _float& fTimeDelta)
{
	static _float fTime = 0.f;
	fTime += fTimeDelta;

	vPos;
	if (fTime > 0.03f)
	{
		fTime = 0.f;

		_float fRandomX = (_float)(rand() % 100) / 100.f;
		_float fRandomY = (_float)(rand() % 100) / 100.f;
		
		_vec3 i = { vPos.x + 0.15f * (fRandomX - 0.5f),
					vPos.y ,
						  10.f };
		m_pTransform->Set_Pos(i.x,	i.y, 10.f);
		_matrix			matTrans;
		D3DXMatrixTranslation(&matTrans, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y,10.f);
		
		//return i;

	}
}
void CCrackBlock::Link()
{
	if (m_bDead);
	


}
CCrackBlock* CCrackBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrackBlock* pInstance = new CCrackBlock(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCrackBlock::Free(void)
{
	__super::Free();

}
