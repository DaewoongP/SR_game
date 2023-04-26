#include "stdafx.h"
#include "StarParticle.h"

#include "Export_Function.h"
CStarParticle::CStarParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
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
	m_VBSize = 128;
	m_VBOffset = 0;
	m_VBBatchSize = 8;

	m_Size = 5.f;
	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CStarParticle::CStarParticle(const CStarParticle & rhs)
	:CParticleSystem(rhs)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CStarParticle::~CStarParticle()
{
}

void CStarParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	GetRandomVector(&particle->vPos, &_vec3(-1, -1, -1), &_vec3(1, 1, 1));
	particle->vPos *= 200.f; // 그냥 원점에서 200거리인 벡터 랜덤생성함.
	particle->vVelocity = _vec3(-1.f, 0.f, 0.f);
	particle->vVelocity *= 100.f;
}

_int CStarParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();
	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);
	_vec3 vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));
	for (auto& it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (it->bIsAlive)
		{
			// 캠과의 거리에 따라 속도가 달라짐 -> 가까울수록 빠름
			_float fLen = D3DXVec3Length(&(vCamPos - it->vPos)) / 100;
			it->vPos += it->vVelocity * fTimeDelta * 1 / fLen;
			if (it->vPos.x <= 0.f)
				it->bIsAlive = false;
		}
		else
			ResetParticle(&*it);
		
	}
	__super::Render_Particle();
	return -1;
}

CStarParticle * CStarParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CStarParticle *	pInstance = new CStarParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CStarParticle::Clone(void)
{
	return new CStarParticle(*this);
}

void CStarParticle::Free(void)
{
	__super::Free();
}
