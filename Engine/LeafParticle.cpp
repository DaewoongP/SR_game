#include "stdafx.h"
#include "LeafParticle.h"

#include "Export_Function.h"
CLeafParticle::CLeafParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
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

	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CLeafParticle::CLeafParticle(const CLeafParticle & rhs)
	:CParticleSystem(rhs)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CLeafParticle::~CLeafParticle()
{
}

void CLeafParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vVelocity = { -20.f + rand() % 10, -13.f + rand() % 8, 0.f };
	GetRandomVector(&particle->vPos,
		&_vec3(CUBEX * 2.f, CUBEY, 3.f),
		&_vec3(CUBEX * 5.f, CUBEY * 6.f, 11.f));
}

_int CLeafParticle::Update_Particle()
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
			it->vVelocity.y += (rand() % 2) * 0.1f;
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

CLeafParticle * CLeafParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CLeafParticle *	pInstance = new CLeafParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CLeafParticle::Clone(void)
{
	return new CLeafParticle(*this);
}

void CLeafParticle::Free(void)
{
	__super::Free();
}
