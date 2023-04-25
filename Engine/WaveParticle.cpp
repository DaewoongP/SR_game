#include "stdafx.h"
#include "WaveParticle.h"

#include "Export_Function.h"
CWaveParticle::CWaveParticle(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar * pPath,
	_int iTextureNum,
	_float fSize,
	_int iParticleNum,
	_bool isWorld,
	_float fLifeTime)
	:CParticleSystem(pGraphicDev)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath,
		iTextureNum);
	if (iTextureNum > 1)
	{
		m_pTexture->Add_Anim(L"Idle", 0, iTextureNum - 1, 5.f, false);
		m_pTexture->Switch_Anim(L"Idle");
		m_pTexture->m_bUseFrameAnimation = true;
	}
	m_bIsWorld = isWorld;
	// 객체의 Ready에서도 변경 가능
	m_Size = fSize;
	// 적절하게 크기값 조절하면됨.
	m_VBSize = 8;
	m_VBOffset = 0;
	m_VBBatchSize = 8;
	m_vUp = { 0,1, 0.2f };
	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CWaveParticle::CWaveParticle(const CWaveParticle & rhs)
	:CParticleSystem(rhs),
	m_fLifeTime(rhs.m_fLifeTime),
	m_vUp(rhs.m_vUp),
	m_fSin(rhs.m_fSin)
{
	m_fCurRad = 0;
	m_fSin = 0;
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CWaveParticle::~CWaveParticle()
{
}

void CWaveParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vPos = m_BoundingBox.Get_Center();
	// 파티클 기준 벨로시티 정하고 그거에서 360/개수 한만큼의 각도를 돌려서 출력
	// 현재 기준 벨로시티 -> 1,0,0
	_float offsetRad = 360.f / m_Particles.size();
	_matrix mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixRotationAxis(&mat, &m_vUp, D3DXToRadian(m_fCurRad));
	D3DXVec3TransformNormal(&particle->vVelocity, &_vec3(1.f, 0.f, 0.f), &mat);
	particle->vVelocity *= 20.f;
	m_fCurRad += offsetRad;
	if (m_fCurRad > 360)
		m_fCurRad -= 360;

	particle->fGenTime = 0;
	particle->fAge = 0;
	particle->fLifeTime = m_fLifeTime;
	particle->fSizeoverLifetime = 1;
	m_fSin = 0.f;
}

_int CWaveParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();
	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");

	for (auto& it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (it->bIsAlive)
		{
			it->fAge += fTimeDelta;
			if (it->fGenTime != 0 && it->fGenTime > it->fAge)
				continue;

			it->vVelocity.y = sinf(m_fSin) * 20.f;
			m_fSin += D3DXToRadian(1.f);
			it->vPos += it->vVelocity * fTimeDelta;

			m_Size *= it->fSizeoverLifetime;
			if (m_pTexture->IsAnimationEnd(L"Idle"))
				m_pTexture->Reset_Anim();

			if (m_BoundingBox.Intersect(it->vPos) == false)
			{
				it->bIsAlive = false;
			}

			if (it->fAge > it->fLifeTime)
				it->bIsAlive = false;
		}
	}
	__super::Render_Particle();
	return -1;
}

CWaveParticle * CWaveParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar * pPath,
	_int iTextureNum,
	_float fSize,
	_int iParticleNum,
	_bool isWorld,
	_float fLifeTime)
{
	CWaveParticle *	pInstance = new CWaveParticle(pGraphicDev,
		pPath,
		iTextureNum,
		fSize,
		iParticleNum,
		isWorld,
		fLifeTime);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CWaveParticle::Clone(void)
{
	return new CWaveParticle(*this);
}

void CWaveParticle::Free(void)
{
	__super::Free();
}
