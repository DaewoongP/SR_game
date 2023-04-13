#include "stdafx.h"
#include "BlockExp.h"

#include "Export_Function.h"
CBlockExp::CBlockExp(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum,  _bool isWorld)
	:CParticleSystem(pGraphicDev)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath,
		iTextureNum);
	m_pTexture->Add_Anim(L"Idle", 0, 8, 1.f, false);
	m_pTexture->Switch_Anim(L"Idle");
	m_pTexture->m_bUseFrameAnimation = true;
	m_bIsWorld = isWorld;

	m_Size = fSize;
	m_VBSize = 64;
	m_VBOffset = 0;
	m_VBBatchSize = 16;

	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CBlockExp::CBlockExp(const CBlockExp & rhs)
	:CParticleSystem(rhs)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CBlockExp::~CBlockExp()
{
}

void CBlockExp::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	// 파티클 설정
	// 바운딩박스 위치값 설정을 어디에 줘야할지,._
	particle->vVelocity = { 0.f, 0.f, 0.f };
	particle->vPos = m_BoundingBox.Get_Center();
}

_int CBlockExp::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();

	list<Particle>::iterator it;

	for (it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (m_pTexture->IsAnimationEnd(L"Idle") && true == it->bIsAlive)
		{
			ResetParticle(&(*it));
			it->bIsAlive = false;
			m_pTexture->Reset_Anim();
		}
	}
	__super::Render_Particle();
	return -1;
}

CBlockExp * CBlockExp::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CBlockExp *	pInstance = new CBlockExp(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CBlockExp::Clone(void)
{
	return new CBlockExp(*this);
}

void CBlockExp::Free(void)
{
	__super::Free();
}
