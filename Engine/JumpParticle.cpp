#include "stdafx.h"
#include "JumpParticle.h"

#include "Export_Function.h"
CJumpParticle::CJumpParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
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

CJumpParticle::CJumpParticle(const CJumpParticle & rhs)
	:CParticleSystem(rhs)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CJumpParticle::~CJumpParticle()
{
}

void CJumpParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	if (nullptr != m_pGameObject)
		particle->vPos = m_pGameObject->m_pTransform->m_vInfo[INFO_POS];
	particle->vPos.y -= 2.f;
	GetRandomVector(&particle->vVelocity, 
		&m_BoundingBox._offsetMin,
		&m_BoundingBox._offsetMax);
	D3DXVec3Normalize(&particle->vAccel, &particle->vVelocity);
	particle->vAccel.y += 2.f;
	particle->vVelocity = particle->vAccel * 10.f;
	particle->fAge = 0.f;
	particle->fLifeTime = 0.3f;
}

_int CJumpParticle::Update_Particle()
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
			it->vVelocity -= it->vAccel;
			if (D3DXVec3Length(&it->vAccel) > D3DXVec3Length(&it->vVelocity))
				it->vAccel = { 0.f, 0.f, 0.f };

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

CJumpParticle * CJumpParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CJumpParticle *	pInstance = new CJumpParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CJumpParticle::Clone(void)
{
	return new CJumpParticle(*this);
}

void CJumpParticle::Free(void)
{
	__super::Free();
}
