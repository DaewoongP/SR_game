#include "stdafx.h"
#include "CrackCube.h"

#include"Export_Function.h"

CCrackCube::CCrackCube(LPDIRECT3DDEVICE9 pGraphicDev):CCube(pGraphicDev)
{
	
}

CCrackCube::~CCrackCube()
{
}

HRESULT CCrackCube::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_fBlockTime = CRACKTIME;
	m_pCollider->Set_BoundingBox({ 2.f,2.f,2.f });
	m_bCrackDead = false;
	m_pCollider->Set_Group(COL_ENV);
	m_bShakeDir = (rand() % 2);
	return S_OK;
}

_int CCrackCube::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CCrackCube::Update_Too(const _float& fTimeDelta)
{
	if (m_bCrackDead)
	{
		m_fBlockTime -= fTimeDelta;
		_vec3 ShakePos;
		ShakePos = m_pTransform->m_vInfo[INFO_POS];
		Shaking(ShakePos, fTimeDelta);
		if (m_fBlockTime <= 0.f)
		{
			m_bDead = true;
			return OBJ_DEAD;
		}
	}
	return 0;
}

_int CCrackCube::Update_Top(const _float& fTimeDelta)
{
	if (m_fBlockTime <= 0.f)
		return OBJ_DEAD;

	return 0;
}

void CCrackCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CCrackCube::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();
	CGameObject::Render_GameObject();
}

void CCrackCube::OnCollisionEnter(const Collision* collision)
{
	if(!lstrcmp(collision->otherObj->m_pTag, L"Toodee") &&collision->_dir == DIR_UP)
		DoShootRay(DIR_UP);
	__super::OnCollisionEnter(collision);

}

void CCrackCube::DoShootRay(COL_DIR exceptDir)
{
	m_bCrackDead = true;
	for (int i = 0; i < DIR_FRONT; i++)
	{
		if (exceptDir == i)
			continue;
		ShootRay((COL_DIR)i);
	}
}

HRESULT CCrackCube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"CrackBlock_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CrackBlock_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	return S_OK;
}

void CCrackCube::ShootRay(COL_DIR exceptDir)
{
	_vec3 dir[DIR_END] = { { 0,1,0 },{ 0,-1,0 },{ -1,0,0 },{ 1,0,0 } };
	vector<_tchar*> findTag;
	findTag.push_back(L"CrackCube");
	vector<RayCollision> _detectedCOL = 
		Engine::Check_Collision_Ray(RAYCAST(m_pTransform->m_vInfo[INFO_POS], dir[exceptDir], 1.5f), m_pCollider, findTag);
	if (_detectedCOL.size() >= 1)
	{
		if (exceptDir % 2 == 0)
			exceptDir = (COL_DIR)(exceptDir + 1);
		else
			exceptDir = (COL_DIR)(exceptDir - 1);

		if(!dynamic_cast<CCrackCube*>(_detectedCOL[0].col->m_pGameObject)->GetCrackDead())
			dynamic_cast<CCrackCube*>(_detectedCOL[0].col->m_pGameObject)->DoShootRay(exceptDir);
	}
}


void CCrackCube::Shaking(_vec3& vPos, const _float& fTimeDelta)
{
	static _float fTime = 0.f;
	fTime += fTimeDelta;

	if (fTime > 0.05f)
	{
		fTime = 0.f;
		_float fRandomX = 0;
		if (m_bShakeDir)
			fRandomX = -1;
		else
			fRandomX = 1;
		m_bShakeDir = !m_bShakeDir;
		
		_float fRandomY = (_float)(rand() % 100) / 100.f;
		_vec3 ShakePos = {vPos.x + 0.075f * fRandomX,
					vPos.y ,  10.f};
		m_pTransform->Set_Pos(ShakePos.x,ShakePos.y,10.f);
	}
}

CCrackCube* CCrackCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CCrackCube* pInstance = new CCrackCube(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCrackCube::Free(void)
{
	__super::Free();
}
