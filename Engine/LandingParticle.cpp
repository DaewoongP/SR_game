#include "stdafx.h"
#include "LandingParticle.h"

#include "Export_Function.h"
CLandingParticle::CLandingParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
	:CParticleSystem(pGraphicDev)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath);
	m_bIsWorld = isWorld;
	// 객체의 Ready에서도 변경 가능
	m_Size = fSize;
	// 적절하게 크기값 조절하면됨.
	m_VBSize = 8;
	m_VBOffset = 0;
	m_VBBatchSize = 8;

	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CLandingParticle::CLandingParticle(const CLandingParticle & rhs)
	:CParticleSystem(rhs)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CLandingParticle::~CLandingParticle()
{
}

void CLandingParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	if (nullptr != m_pGameObject && m_bClone)
		particle->vPos = m_pGameObject->m_pTransform->m_vInfo[INFO_POS];
	particle->vPos.y -= rand() % 1000 * 0.0017;
	switch (rand() % 2)
	{
	case 0:
		particle->vVelocity = { 10.f, 0.f, 0.f };
		break;
	case 1:
		particle->vVelocity = { -10.f, 0.f, 0.f };
		break;
	}
	particle->fAge = 0.f;
	particle->fLifeTime = 0.5f;
}

_int CLandingParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();

	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");
	for (auto& it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (it->bIsAlive)
		{
			it->vPos += it->vVelocity * fTimeDelta;
			m_Size *= 0.995f;
			it->fAge += fTimeDelta;
			if (it->fAge > it->fLifeTime)
				it->bIsAlive = false;
		}
		else
			ResetParticle(&(*it));
	}
	__super::Render_Particle();
	return -1;
}

CLandingParticle * CLandingParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CLandingParticle *	pInstance = new CLandingParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CLandingParticle::Clone(void)
{
	return new CLandingParticle(*this);
}

void CLandingParticle::Free(void)
{
	__super::Free();
}
