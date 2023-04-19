#include "stdafx.h"
#include "WaterPipe.h"

#include"Export_Function.h"
CWaterPipe::CWaterPipe(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CWaterPipe::~CWaterPipe()
{
}

HRESULT CWaterPipe::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 0.5f,0.5f,0.5f };
	m_pTransform->m_bIsStatic = false;

	m_pWaterFallTex->Add_Anim(L"Idle", 0, 5, 0.3f, true);
	m_pWaterFallTex->Switch_Anim(L"Idle");
	m_pWaterFallTex->m_bUseFrameAnimation = true;
	
	m_pWaterSplashTex->Add_Anim(L"Splash", 0, 9, 0.3f, true);
	m_pWaterSplashTex->Switch_Anim(L"Splash");
	m_pWaterSplashTex->m_bUseFrameAnimation = true;
	BoundingBox box;
	box.Offset(vPos);
	m_pCircularParticle->Set_BoundingBox(box);
	m_pCircularParticle->Set_Size(0.2f);
	//m_pCircularParticle->Set_Options(0.3f, 100.f);
	m_pCircularParticle->Set_RandomGen(5.f);
	box._offsetMin = { -0.2f, -0.2f, 0.f };
	box._offsetMax = { 0.2f, 0.2f, 0.f };

	m_pWaterParticle->Set_BoundingBox(box);
	m_pWaterParticle->Set_Offset(5);
	m_pWaterParticle->Set_AnimSpeed(0.001f);
	m_pWaterParticle->Set_LifeTime();
	m_pWaterParticle->Start_Particle();
	
    return S_OK;
}

_int CWaterPipe::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(fTimeDelta);
    return 0;
}

_int CWaterPipe::Update_Too(const _float& fTimeDelta)
{
	m_bInit = true;
	if (m_bInit)
	{
		m_pWaterFallTex->Update_Anim(fTimeDelta);
		m_pWaterSplashTex->Update_Anim(fTimeDelta);
		m_InitBox = m_Box = Check_BoundingBox();	
	}
	__super::Update_Too(fTimeDelta);

	return 0;
}

_int CWaterPipe::Update_Top(const _float& fTimeDelta)
{
	m_bInit = false;
	__super::Update_Top(fTimeDelta);

	return S_OK;
}

void CWaterPipe::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CWaterPipe::Render_GameObject(void)
{
	m_pWaterParticle->Update_Particle();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	if (m_bInit)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, WaterFallPos());
		m_pWaterFallTex->Set_Texture();
		m_pwaterFall->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, WaterSplashPos());
		m_pWaterSplashTex->Set_Texture();
		m_pwaterSplash->Render_Buffer();

	}
	

	
	__super::Render_GameObject();

}

BoundingBox CWaterPipe::Check_BoundingBox()
{
	BoundingBox box;
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS];
	_vec3 vDir = _vec3(0.f, -1.f, 0.f);
	_float len = centerpos.y + 1.f;
	_float fDist = len;
	_vec3 colmin, colmax;
	m_pCollider->Get_Point(&colmin, &colmax);

	vector<_tchar*> tagName;
	tagName.push_back(L"MapCube");
	tagName.push_back(L"MoveCube");
	tagName.push_back(L"GravityCube");
	tagName.push_back(L"InstallCube");
	tagName.push_back(L"SwitchCube");
	tagName.push_back(L"CrackCube");
	tagName.push_back(L"PortalCube");

	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(
		RAYCAST(centerpos, vDir, len), m_pCollider, tagName);
	if (_detectedCOL.size() >= 1)
		fDist = _detectedCOL[0].dist;
	else
	{
		box._min = colmin;
		box._min.y -= fDist;
		box._min.z = 10.f;
		box._max = _vec3(colmax.x, colmin.y, colmax.z);
		m_vWaterFall.y = box._max.y - box._min.y;

		return box;
	}

	
}

_matrix* CWaterPipe::WaterFallPos()
{
	_matrix matScale, matTrans;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	_vec3 vPos;

	m_pTransform->Get_Info(INFO_POS, &vPos);
	//vPos.z -= 0.1f;
	D3DXMatrixScaling(&matScale, 0.3f, m_vWaterFall.y / 2.f,vPos.z);
	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y - m_vWaterFall.y / 2.f,
		10.f);

	return &(matScale * matTrans);
}

_matrix* CWaterPipe::WaterSplashPos()
{
	_matrix matScale, matTrans;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	_vec3 vPos;

	m_pTransform->Get_Info(INFO_POS, &vPos);
	//vPos.z -= 0.1f;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, vPos.z);
	D3DXMatrixTranslation(&matTrans,
		vPos.x,
		vPos.y-m_vWaterFall.y+2.5f,
		10.f);

	return &(matScale * matTrans);
}


HRESULT CWaterPipe::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Sink_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture",pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });


	pComponent = m_pwaterFall = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pwaterFall, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"WaterfallTex" , pComponent });

	pComponent = m_pwaterSplash = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pwaterFall, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"WaterfallTex" , pComponent });

	pComponent = m_pWaterFallTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"WaterFall_Texture", this));
	NULL_CHECK_RETURN(m_pWaterFallTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"WaterFall",pComponent });

	pComponent = m_pWaterSplashTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"WaterSplash_Texture", this));
	NULL_CHECK_RETURN(m_pWaterSplashTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"WaterSplash",pComponent });

	pComponent = m_pWaterParticle = dynamic_cast<CWaterParticle*>(Engine::Clone_Proto(L"WaterParticle", this));
	NULL_CHECK_RETURN(m_pWaterParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"WaterParticle", pComponent });

	pComponent = m_pCircularParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"CircularParticle", this));
	NULL_CHECK_RETURN(m_pCircularParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CircularParticle", pComponent });
	
	return S_OK;
}

CWaterPipe* CWaterPipe::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CWaterPipe* pInstance = new CWaterPipe(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CWaterPipe::Free(void)
{
	__super::Free();
}
