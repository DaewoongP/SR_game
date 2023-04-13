#include "stdafx.h"
#include "Spark.h"

#include "Export_Function.h"
CSpark::CSpark(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
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

	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CSpark::CSpark(const CSpark & rhs)
	:CParticleSystem(rhs)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CSpark::~CSpark()
{
}

void CSpark::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vVelocity = { 0.f, 0.f, 0.f };
	GetRandomVector(
		&particle->vPos,
		&m_BoundingBox._min,
		&m_BoundingBox._max);
}

_int CSpark::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();

	__super::Render_Particle();
	return -1;
}

CSpark * CSpark::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CSpark *	pInstance = new CSpark(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CSpark::Clone(void)
{
	return new CSpark(*this);
}

void CSpark::Free(void)
{
	__super::Free();
}
