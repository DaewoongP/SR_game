#include "stdafx.h"
#include "GravityParticle.h"

#include "Export_Function.h"
CGravityParticle::CGravityParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
	:CParticleSystem(pGraphicDev)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath);
	m_bIsWorld = isWorld;
	// ��ü�� Ready������ ���� ����
	m_Size = fSize;
	// �����ϰ� ũ�Ⱚ �����ϸ��.
	m_VBSize = 8;
	m_VBOffset = 0;
	m_VBBatchSize = 8;

	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CGravityParticle::CGravityParticle(const CGravityParticle & rhs)
	:CParticleSystem(rhs)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CGravityParticle::~CGravityParticle()
{
}

void CGravityParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vVelocity = { 0.f, -20.f, 0.f };
	GetRandomVector(&particle->vPos,
		&m_BoundingBox._min,
		&m_BoundingBox._max);
	particle->vPos.y = m_BoundingBox._max.y - (rand() % 30);
}

_int CGravityParticle::Update_Particle()
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
			if (m_BoundingBox.Intersect(it->vPos) == false)
			{
				ResetParticle(&(*it));
			}
		}
		else
			ResetParticle(&(*it));
	}
	__super::Render_Particle();
	return -1;
}

CGravityParticle * CGravityParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CGravityParticle *	pInstance = new CGravityParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CGravityParticle::Clone(void)
{
	return new CGravityParticle(*this);
}

void CGravityParticle::Free(void)
{
	__super::Free();
}
