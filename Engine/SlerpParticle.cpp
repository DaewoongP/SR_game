#include "stdafx.h"
#include "SlerpParticle.h"

#include "Export_Function.h"
CSlerpParticle::CSlerpParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
	:CParticleSystem(pGraphicDev)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath,
		iTextureNum);
	m_pTexture->Add_Anim(L"Idle", 0, iTextureNum - 1, 20.f, true);
	m_pTexture->Switch_Anim(L"Idle");
	m_pTexture->m_bUseFrameAnimation = true;
	m_bIsWorld = isWorld;
	// 객체의 Ready에서도 변경 가능
	m_Size = fSize;
	// 적절하게 크기값 조절하면됨.
	m_VBSize = 8;
	m_VBOffset = 0;
	m_VBBatchSize = 8;
	m_fLifeTime = 0.5f;

	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CSlerpParticle::CSlerpParticle(const CSlerpParticle & rhs)
	:CParticleSystem(rhs),
	m_fLifeTime(rhs.m_fLifeTime),
	m_v1(rhs.m_v1),
	m_v2(rhs.m_v2),
	m_fTime(rhs.m_fTime),
	m_vRandUp(rhs.m_vRandUp)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CSlerpParticle::~CSlerpParticle()
{
}

void CSlerpParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 0.5f);
	particle->fLifeTime = _float(rand() % 10) * 30.f;
	switch (rand() % 2)
	{
	case 0:
		m_vRandUp = { 0.f, 0.f, -1.f };
		break;
	case 1:
		m_vRandUp = { 0.f, 0.f, 1.f };
		break;
	}
}

_int CSlerpParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();
	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");
	m_fTime += 0.02f;
	
	for (auto& it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		GetVectorSlerp(&it->vPos, &m_v1, &m_v2, &m_vRandUp,it->fLifeTime, m_fTime);
		it->vPos.z -= 1.5f;

		if (m_fTime >= 1.f)
		{
			it->bIsAlive = false;
			End_Particle();
			m_fTime = 0.f;
		}
	}
	__super::Render_Particle();
	return -1;
}

CSlerpParticle * CSlerpParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CSlerpParticle *	pInstance = new CSlerpParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CSlerpParticle::Clone(void)
{
	return new CSlerpParticle(*this);
}

void CSlerpParticle::Free(void)
{
	__super::Free();
}
