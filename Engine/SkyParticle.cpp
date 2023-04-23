#include "stdafx.h"
#include "SkyParticle.h"

#include "Export_Function.h"
CSkyParticle::CSkyParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
	:CParticleSystem(pGraphicDev)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath,
		iTextureNum);
	m_bIsWorld = isWorld;
	// 객체의 Ready에서도 변경 가능
	m_Size = fSize;
	// 적절하게 크기값 조절하면됨.
	m_VBSize = 8;
	m_VBOffset = 0;
	m_VBBatchSize = 8;

	m_pTexture->Add_Anim(L"Idle", 2, 8, 20.f, true);
	m_pTexture->Switch_Anim(L"Idle");
	m_pTexture->m_bUseFrameAnimation = true;
	m_Size = 5.f;
	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CSkyParticle::CSkyParticle(const CSkyParticle & rhs)
	:CParticleSystem(rhs)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CSkyParticle::~CSkyParticle()
{
}

void CSkyParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vPos = { 300.f, 20.f, 10.f };
	GetRandomVectorIncircle(&particle->vVelocity, GetRandomFloat(10.f, 300.f));
	particle->vVelocity.z = particle->vVelocity.x;
	particle->vVelocity.x = 0.f;
	particle->vPos += particle->vVelocity;
}

_int CSkyParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();
	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");
	
	__super::Render_Particle();
	return -1;
}

CSkyParticle * CSkyParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CSkyParticle *	pInstance = new CSkyParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CSkyParticle::Clone(void)
{
	return new CSkyParticle(*this);
}

void CSkyParticle::Free(void)
{
	__super::Free();
}
