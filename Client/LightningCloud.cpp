#include"stdafx.h"
#include "LightningCloud.h"

#include"..\Engine\Export_Function.h"
#include "AbstractFactory.h"
CLightningCloud::CLightningCloud(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCloud(pGraphicDev),
	m_bInit(true)
{
}

CLightningCloud::~CLightningCloud()
{
}

HRESULT CLightningCloud::Ready_GameObject(_vec3& vPos)
{
	m_pTransform->m_vScale = { 4.f,4.f,2.f };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_bIsStatic = true;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTextureCom->Add_Anim(L"Idle", 0, 5, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	
	m_pCollider->Set_BoundingBox({ m_pTransform->m_vScale.x + 3.f,2.f,1.0f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

_int CLightningCloud::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

_int CLightningCloud::Update_Too(const _float& fTimeDelta)
{
	if (m_bInit)
	{
		m_InitBox = m_Box = Check_BoundingBox();
		m_pRainParticle->Set_BoundingBox(m_Box);
		m_pRainParticle->Start_Particle();
		m_bInit = false;
	}
	Check_ParticleBox();
	__super::Update_Too(fTimeDelta);

	return S_OK;
}

_int CLightningCloud::Update_Top(const _float& fTimeDelta)
{
	__super::Update_Top(fTimeDelta);

	return S_OK;
}

void CLightningCloud::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pRainParticle->Update_Particle();
	__super::Render_GameObject();
}

void CLightningCloud::SwapTrigger()
{
	if (!g_Is2D)
	{
		m_InitBox = m_Box;
		m_Box._max.y += 1.5f;
		m_Box._min.y = m_Box._max.y - 7.f;
		m_pRainParticle->Set_BoundingBox(m_Box);
	}
}

HRESULT CLightningCloud::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	m_pTransform->m_bIsStatic = false;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"LightningCloud_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"LightningCloud_Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pRainParticle = dynamic_cast<CRainParticle*>(Engine::Clone_Proto(L"RainParticle", this));
	NULL_CHECK_RETURN(m_pRainParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RainParticle", pComponent });
	return S_OK;
}

BoundingBox CLightningCloud::Check_BoundingBox()
{
	BoundingBox box;
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS];
	_vec3 vDir = _vec3(0.f, -1.f, 0.f);
	_float len = centerpos.y + 2.f;
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
		return m_InitBox;
	box._min = colmin;
	box._min.y -= fDist ;
	box._min.z = 10.f;
	box._max = _vec3(colmax.x, colmin.y, colmax.z);
	return box;
}

void CLightningCloud::Check_ParticleBox()
{
	BoundingBox box = Check_BoundingBox();
	if ((_int)m_Box._min.y != (_int)box._min.y)
	{
		m_pRainParticle->Set_BoundingBox(box);
	}
	m_Box = box;
}

CLightningCloud* CLightningCloud::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CLightningCloud* pInstance = new CLightningCloud(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLightningCloud::Free(void)
{
	__super::Free();
}
