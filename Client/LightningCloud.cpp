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

	m_pLightningTex1->Add_Anim(L"Lightning1", 0, 8, 0.5f, true);
	m_pLightningTex1->Switch_Anim(L"Lightning1");
	m_pLightningTex1->m_bUseFrameAnimation = true;

	m_pLightningTex2->Add_Anim(L"Lightning2", 0, 8, 1.f, true);
	m_pLightningTex2->Switch_Anim(L"Lightning2");
	m_pLightningTex2->m_bUseFrameAnimation = true;

	m_pLightningTex3->Add_Anim(L"Lightning3", 0, 8, 1.5f, true);
	m_pLightningTex3->Switch_Anim(L"Lightning3");
	m_pLightningTex3->m_bUseFrameAnimation = true;
	
	m_pCollider->Set_BoundingBox({ m_pTransform->m_vScale.x + 3.f,2.f,1.0f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

_int CLightningCloud::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CheckPlayerInLightning();
	m_pTextureCom->Update_Anim(fTimeDelta);
	m_pLightningTex1->Update_Anim(fTimeDelta);
	m_pLightningTex2->Update_Anim(fTimeDelta);
	m_pLightningTex3->Update_Anim(fTimeDelta);
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
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
	
	// 각각 번개에 월드행렬 적용
	// 객체 새로 만들기 너무 귀찮았어요
	m_pGraphicDev->SetTransform(D3DTS_WORLD, MakeLightningMatrix(1));
	m_pLightningTex1->Set_Texture();
	m_pLightningRc1->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, MakeLightningMatrix(2));
	m_pLightningTex2->Set_Texture();
	m_pLightningRc2->Render_Buffer();
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, MakeLightningMatrix(3));
	m_pLightningTex3->Set_Texture();
	m_pLightningRc3->Render_Buffer();

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

void CLightningCloud::OnCollisionEnter(const Collision * collsion)
{
	__super::OnCollisionEnter(collsion);
}

void CLightningCloud::OnCollisionStay(const Collision * collsion)
{
	__super::OnCollisionStay(collsion);
}

void CLightningCloud::OnCollisionExit(const Collision * collsion)
{
	__super::OnCollisionExit(collsion);
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

	pComponent = m_pLightningTex1 = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Lightning_Texture", this));
	NULL_CHECK_RETURN(m_pLightningTex1, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Lightning1", pComponent });

	pComponent = m_pLightningTex2 = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Lightning_Texture", this));
	NULL_CHECK_RETURN(m_pLightningTex2, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Lightning2", pComponent });

	pComponent = m_pLightningTex3 = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Lightning_Texture", this));
	NULL_CHECK_RETURN(m_pLightningTex3, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Lightning3", pComponent });

	pComponent = m_pLightningRc1 = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pLightningRc1, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"LightningRc1", pComponent });

	pComponent = m_pLightningRc2 = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pLightningRc2, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"LightningRc2", pComponent });

	pComponent = m_pLightningRc3 = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pLightningRc3, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"LightningRc3", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

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
		m_vLightningSize1.y = box._max.y - box._min.y;
		m_pRainParticle1->Set_Offset(fDist);
		break;
	case 2:
		m_vLightningSize2.y = box._max.y - box._min.y;
		m_pRainParticle2->Set_Offset(fDist);
		break;
	case 3:
		m_vLightningSize3.y = box._max.y - box._min.y;
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

_matrix* CLightningCloud::MakeLightningMatrix(size_t LightningNum)
{
	_matrix matScale, matTrans;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	_vec3	vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);
	vPos.y -= 1.f;
	if (!g_Is2D)
	{
		m_vLightningSize1.y = 5.f;
		m_vLightningSize2.y = 5.f;
		m_vLightningSize3.y = 5.f;
		m_vLightningSize1.z = 7.f;
		m_vLightningSize2.z = 7.f;
		m_vLightningSize3.z = 7.f;
	}
	else
	{
		m_vLightningSize1.z = 10.f;
		m_vLightningSize2.z = 10.f;
		m_vLightningSize3.z = 10.f;
	}
	switch (LightningNum)
	{
	case 1:
		// x 2고정, y 번개 사이즈, z값 고정
		D3DXMatrixScaling(&matScale, 1.f, m_vLightningSize1.y / 2.f, 1.f);
		// x 왼쪽 -2, y 스케일 줄어든 만큼 위로보정, z값 고정
		D3DXMatrixTranslation(&matTrans, 
			vPos.x - 2.f, 
			vPos.y - m_vLightningSize1.y / 2.f,
			m_vLightningSize1.z);
		break;
	case 2:
		// x 2고정, y 번개 사이즈, z값 고정
		D3DXMatrixScaling(&matScale, 1.f, m_vLightningSize2.y / 2.f, 1.f);
		// x = pos.x , y 스케일 줄어든 만큼 위로보정, z값 고정
		D3DXMatrixTranslation(&matTrans,
			vPos.x,
			vPos.y - m_vLightningSize2.y / 2.f,
			m_vLightningSize2.z);
		break;
	case 3:
		// x 2고정, y 번개 사이즈, z값 고정
		D3DXMatrixScaling(&matScale, 1.f, m_vLightningSize3.y / 2.f, 1.f);
		// x + 2.f , y 스케일 줄어든 만큼 위로보정, z값 고정
		D3DXMatrixTranslation(&matTrans,
			vPos.x + 2.f,
			vPos.y - m_vLightningSize3.y / 2.f,
			m_vLightningSize3.z);
		break;
	}

	return &(matScale * matTrans);
}

void CLightningCloud::CheckPlayerInLightning()
{
	_vec3 RayPos1, RayPos2, RayPos3;
	RayPos1 = RayPos2 = RayPos3 = m_pTransform->m_vInfo[INFO_POS];
	RayPos1.z = RayPos2.z = RayPos3.z = 10.f;
	RayPos1.x -= 2.f;
	RayPos3.x += 2.f;
	if (CheckRay(RayPos1, m_vLightningSize1.y) ||
		CheckRay(RayPos2, m_vLightningSize2.y) || 
		CheckRay(RayPos3, m_vLightningSize3.y))
	{
		//Player In Lightning
		// 사망처리같은거 여기서하면됨.
		int a = 1;
	}
}

_bool CLightningCloud::CheckRay(_vec3 vPos, _float fLightningSizeY)
{
	_vec3 vDir;
	_float len = CUBEY * 2.f;
	
	vector<_tchar*> tagName;
	tagName.push_back(L"Toodee");
	tagName.push_back(L"Topdee");
	if (g_Is2D)
		vDir = { 0.f, -1.f, 0.f };
	else
	{
		vPos.y -= 6.f;
		vDir = { 0.f, 0.f, 1.f };
	}
		
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(
		RAYCAST(vPos, vDir, len), m_pCollider, tagName);
	if (_detectedCOL.size() >= 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"Toodee") && _detectedCOL[0].dist <= fLightningSizeY)
		{
			return true;
		}
		if (!lstrcmp(_detectedCOL[0].tag, L"Topdee") && _detectedCOL[0].dist <= fLightningSizeY)
		{
			return true;
		}
	}
	return false;
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
