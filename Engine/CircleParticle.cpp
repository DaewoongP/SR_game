#include "stdafx.h"
#include "CircleParticle.h"

#include "Export_Function.h"
CCircleParticle::CCircleParticle(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar * pPath,
	_int iTextureNum,
	_float fSize,
	_int iParticleNum,
	_bool isWorld,
	_float fLifeTime,
	_float fRadius)
	:CParticleSystem(pGraphicDev)
{
	m_fLifeTime = fLifeTime;
	m_fRadius = fRadius;
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
	m_vUp = { 0,1, 0.2f };
	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CCircleParticle::CCircleParticle(const CCircleParticle & rhs)
	:CParticleSystem(rhs),
	m_fLifeTime(rhs.m_fLifeTime),
	m_fRadius(rhs.m_fRadius),
	m_vUp(rhs.m_vUp)
{
	m_fCurRad = 0;
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CCircleParticle::~CCircleParticle()
{
}

void CCircleParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	particle->dwColorFade = D3DXCOLOR(0.01f, 0.01f, 0.01f, 0.01f);
	particle->vPos = m_BoundingBox.Get_Center();
	// ��ƼŬ ���� ���ν�Ƽ ���ϰ� �װſ��� 360/���� �Ѹ�ŭ�� ������ ������ ���
	// ���� ���� ���ν�Ƽ -> 1,0,0
	_float offsetRad = 360.f / m_Particles.size();
	_matrix mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixRotationAxis(&mat, &m_vUp, D3DXToRadian(m_fCurRad));
	D3DXVec3TransformCoord(&particle->vVelocity, &_vec3(10.f, 0.f, 0.f), &mat);
	m_fCurRad += offsetRad;
	if (m_fCurRad > 360)
		m_fCurRad -= 360;
	//particle->vVelocity.z *= 0.01f;

	particle->fGenTime = 0;
	particle->fAge = 0;
	particle->fLifeTime = m_fLifeTime;
	particle->fSizeoverLifetime = 1;
	particle->vAccel = { 0,0,0 };
}

_int CCircleParticle::Update_Particle()
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
			
			it->vPos += it->vVelocity * fTimeDelta;
			it->vVelocity += it->vAccel;
			if (D3DXVec3Length(&it->vAccel) > D3DXVec3Length(&it->vVelocity))
				it->vAccel = { 0.f, 0.f, 0.f };
			m_Size *= it->fSizeoverLifetime;

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

CCircleParticle * CCircleParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar * pPath,
	_int iTextureNum,
	_float fSize,
	_int iParticleNum,
	_bool isWorld,
	_float fLifeTime,
	_float fRadius)
{
	CCircleParticle *	pInstance = new CCircleParticle(pGraphicDev,
		pPath,
		iTextureNum,
		fSize,
		iParticleNum,
		isWorld,
		fLifeTime,
		fRadius);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CCircleParticle::Clone(void)
{
	return new CCircleParticle(*this);
}

void CCircleParticle::Free(void)
{
	__super::Free();
}