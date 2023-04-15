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
		m_InitBox1 = m_Box1 = Check_BoundingBox(1);
		m_pRainParticle1->Set_BoundingBox(m_Box1);
		m_pRainParticle1->Start_Particle();

		m_InitBox2 = m_Box2 = Check_BoundingBox(2);
		m_pRainParticle2->Set_BoundingBox(m_Box2);
		m_pRainParticle2->Start_Particle();

		m_InitBox3 = m_Box3 = Check_BoundingBox(3);
		m_pRainParticle3->Set_BoundingBox(m_Box3);
		m_pRainParticle3->Start_Particle();
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

	m_pRainParticle1->Update_Particle();
	m_pRainParticle2->Update_Particle();
	m_pRainParticle3->Update_Particle();
	__super::Render_GameObject();
}

void CLightningCloud::SwapTrigger()
{
	if (!g_Is2D)
	{
		m_pRainParticle1->Set_Offset(3);
		m_InitBox1 = m_Box1;
		m_Box1._max.y += 1.5f;
		m_Box1._min.y = m_Box1._max.y - 7.f;
		m_pRainParticle1->Set_BoundingBox(m_Box1);

		m_pRainParticle2->Set_Offset(3);
		m_InitBox2 = m_Box2;
		m_Box2._max.y += 1.5f;
		m_Box2._min.y = m_Box2._max.y - 7.f;
		m_pRainParticle2->Set_BoundingBox(m_Box2);

		m_pRainParticle3->Set_Offset(3);
		m_InitBox3 = m_Box3;
		m_Box3._max.y += 1.5f;
		m_Box3._min.y = m_Box3._max.y - 7.f;
		m_pRainParticle3->Set_BoundingBox(m_Box3);
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

	pComponent = m_pRainParticle1 = dynamic_cast<CRainParticle*>(Engine::Clone_Proto(L"RainParticle", this));
	NULL_CHECK_RETURN(m_pRainParticle1, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RainParticle1", pComponent });

	pComponent = m_pRainParticle2 = dynamic_cast<CRainParticle*>(Engine::Clone_Proto(L"RainParticle", this));
	NULL_CHECK_RETURN(m_pRainParticle2, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RainParticle2", pComponent });

	pComponent = m_pRainParticle3 = dynamic_cast<CRainParticle*>(Engine::Clone_Proto(L"RainParticle", this));
	NULL_CHECK_RETURN(m_pRainParticle3, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RainParticle3", pComponent });
	return S_OK;
}

BoundingBox CLightningCloud::Check_BoundingBox(size_t ParticleNum)
{
	BoundingBox box;
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS];
	_vec3 vDir = _vec3(0.f, -1.f, 0.f);
	_float len = centerpos.y + 2.f;
	_float fDist = len;
	_vec3 colmin, colmax;
	m_pCollider->Get_Point(&colmin, &colmax);
	switch (ParticleNum)
	{
	case 1:
		centerpos.x -= 2.f;
		colmax.x -= 4.f;
		break;
	case 2:
		colmin.x += 2.f;
		colmax.x -= 2.f;
		break;
	case 3:
		colmin.x += 4.f;
		centerpos.x += 2.f;
		break;
	}
	
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
		switch (ParticleNum)
		{
		case 1:
			return m_InitBox1;
		case 2:
			return m_InitBox2;
		case 3:
			return m_InitBox3;
		}
	}

	box._min = colmin;
	box._min.y -= fDist ;
	box._min.z = 10.f;
	box._max = _vec3(colmax.x, colmin.y, colmax.z);
	switch (ParticleNum)
	{
	case 1:
		m_pRainParticle1->Set_Offset(fDist);
		break;
	case 2:
		m_pRainParticle2->Set_Offset(fDist);
		break;
	case 3:
		m_pRainParticle3->Set_Offset(fDist);
		break;
	}
	
	return box;
}

void CLightningCloud::Check_ParticleBox()
{
	BoundingBox box1, box2, box3;
	box1 = Check_BoundingBox(1);
	box2 = Check_BoundingBox(2);
	box3 = Check_BoundingBox(3);

	if ((_int)m_Box1._min.y != (_int)box1._min.y)
	{
		m_pRainParticle1->Set_BoundingBox(box1);
	}
	m_Box1 = box1;

	if ((_int)m_Box2._min.y != (_int)box2._min.y)
	{
		m_pRainParticle2->Set_BoundingBox(box2);
	}
	m_Box2 = box2;

	if ((_int)m_Box3._min.y != (_int)box3._min.y)
	{
		m_pRainParticle3->Set_BoundingBox(box3);
	}
	m_Box3 = box3;
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
