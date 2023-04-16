#include "stdafx.h"
#include "TexParticle.h"

#include "Export_Function.h"
CTexParticle::CTexParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum,  _bool isWorld)
	:CParticleSystem(pGraphicDev)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath,
		iTextureNum);
	if (iTextureNum > 1)
	{
		m_pTexture->Add_Anim(L"Idle", 0, iTextureNum - 1, 1.f, false);
		m_pTexture->Switch_Anim(L"Idle");
		m_pTexture->m_bUseFrameAnimation = true;
	}
	
	m_bIsWorld = isWorld;
	// ��ü�� Ready������ ���� ����
	m_Size = fSize;
	// �����ϰ� ũ�Ⱚ �����ϸ��.
	m_VBSize = 8;
	m_VBOffset = 0;
	m_VBBatchSize = 8;
	m_fSizeoverLifetime = 1.f;
	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CTexParticle::CTexParticle(const CTexParticle & rhs)
	:CParticleSystem(rhs),
	m_fSizeoverLifetime(rhs.m_fSizeoverLifetime)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CTexParticle::~CTexParticle()
{
}

void CTexParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vPos = m_BoundingBox.Get_Center();
	particle->fSizeoverLifetime = m_fSizeoverLifetime;
	particle->fAge = 0.f;
	particle->fLifeTime = 0.9f;
}

_int CTexParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();
	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");
	for (auto& it : m_Particles)
	{
		// �ִϸ��̼��� �����µ� ��������� ��ƼŬ �ʱ�ȭ �� ���̰� �ִϸ��̼� �ʱ�ȭ
		// �ִϸ��̼� ������ �ʱ�ȭ ����� ���� ��ü�� ��밡����.
		if (m_pTexture->Get_TextureSize() == 1)
		{
			if (!it.bIsAlive)
				continue;
			it.fAge += fTimeDelta;

			m_Size *= it.fSizeoverLifetime;

			if (it.fAge > it.fLifeTime)
				it.bIsAlive = false;
			continue;
		}
		if (m_pTexture->IsAnimationEnd(L"Idle") && true == it.bIsAlive)
		{
			ResetParticle(&it);
			it.bIsAlive = false;
			m_pTexture->Reset_Anim();
		}
	}
	__super::Render_Particle();
	return -1;
}

CTexParticle * CTexParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CTexParticle *	pInstance = new CTexParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CTexParticle::Clone(void)
{
	return new CTexParticle(*this);
}

void CTexParticle::Free(void)
{
	__super::Free();
}
