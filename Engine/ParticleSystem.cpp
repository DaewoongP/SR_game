#include "stdafx.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"

CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphicDev) :
	CComponent(pGraphicDev),
	m_BoundingBox({0.f,0.f,0.f},{0.f,0.f,0.f}),
	m_bIsWorld(true),
	m_bTrigger(false),
	m_fAnimSpeed(0.1f)
{
}

CParticleSystem::CParticleSystem(const CParticleSystem & rhs) :
	CComponent(rhs),
	m_VB(rhs.m_VB),
	m_VBSize(rhs.m_VBSize),
	m_VBOffset(rhs.m_VBOffset),
	m_VBBatchSize(rhs.m_VBBatchSize),
	m_vOrigin(rhs.m_vOrigin),
	m_EmitRate(rhs.m_EmitRate),
	m_Size(rhs.m_Size),
	m_BoundingBox(rhs.m_BoundingBox),
	m_pTexture(rhs.m_pTexture),
	m_bIsWorld(rhs.m_bIsWorld),
	m_bTrigger(rhs.m_bTrigger),
	m_fAnimSpeed(rhs.m_fAnimSpeed)
{
	m_VB->AddRef();
}

CParticleSystem::~CParticleSystem()
{
}

HRESULT CParticleSystem::Ready_Particle(void)
{
	FAILED_CHECK_RETURN
	(
		m_pGraphicDev->CreateVertexBuffer(
			m_VBSize * sizeof(PTCTEX),
			D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
			FVF_PTC,
			D3DPOOL_DEFAULT,
			&m_VB,
			0),
		E_FAIL
	);

	return S_OK;
}

_int CParticleSystem::Update_Particle()
{
	if (nullptr != m_pTexture)
		m_pTexture->Update_Anim(m_fAnimSpeed);

	// 월드 옵션이 꺼져있을경우 현재 오브젝트의 좌표값을 따라감.
	// 바운딩박스는 콜라이더 개념과 같음 (중점 기본 객체 중점, 오프셋 가능)
	if (!m_bIsWorld)
	{
		_vec3 offsetPoint;
		m_pGameObject->m_pTransform->Get_Info(INFO_POS, &offsetPoint);

		m_BoundingBox.Offset(offsetPoint);
	}
	
	return 0;
}

void CParticleSystem::Render_Particle(void)
{
	_matrix mat;
	D3DXMatrixIdentity(&mat);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	if (!m_Particles.empty())
	{
		PreRender();
		if (nullptr != m_pTexture)
			m_pTexture->Set_Texture(0);
		m_pGraphicDev->SetFVF(FVF_PTC);
		m_pGraphicDev->SetStreamSource(0, m_VB, 0, sizeof(PTCTEX));
		
		if (m_VBOffset >= m_VBSize)
			m_VBOffset = 0;

		PTCTEX* v = 0;

		m_VB->Lock(
			m_VBOffset * sizeof(PTCTEX),
			m_VBBatchSize * sizeof(PTCTEX),
			(void**)&v,
			m_VBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		for (auto it = m_Particles.begin(); it != m_Particles.end(); it++)
		{
			if (it->bIsAlive)
			{
				v->vPos = it->vPos;
				v->vTexUV = it->vTexUV;
				v->dwColor = it->dwColor;
				v++;
				numParticlesInBatch++;
	
				if (numParticlesInBatch == m_VBBatchSize)
				{
					m_VB->Unlock();
					m_pGraphicDev->DrawPrimitive(
						D3DPT_POINTLIST,
						m_VBOffset,
						m_VBBatchSize);
					m_VBOffset += m_VBBatchSize;

					if (m_VBOffset >= m_VBSize)
						m_VBOffset = 0;

					m_VB->Lock(
						m_VBOffset * sizeof(PTCTEX),
						m_VBBatchSize * sizeof(PTCTEX),
						(void**)&v,
						m_VBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
					numParticlesInBatch = 0;
				}
			}
		}

		m_VB->Unlock();

		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(
				D3DPT_POINTLIST,
				m_VBOffset,
				numParticlesInBatch);
		}

		m_VBOffset += m_VBBatchSize;

		PostRender();
	}
}

bool CParticleSystem::IsEmpty()
{
	return m_Particles.empty();
}

bool CParticleSystem::IsDead()
{
	for (const Particle& it : m_Particles)
	{
		if (it.bIsAlive == true) return false;
	}

	return true;
}

void CParticleSystem::RemoveDeadParticles()
{
	list<Particle>::iterator it;

	it = m_Particles.begin();

	while (it != m_Particles.end())
	{
		if (it->bIsAlive == false)
		{
			it = m_Particles.erase(it);
		}
		else it++;
	}
}

void CParticleSystem::Reset()
{
	list<Particle>::iterator it;
	for (it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		ResetParticle(&(*it));
	}
}

void CParticleSystem::AddParticle()
{
	Particle particle;
	ResetParticle(&particle);
	m_Particles.push_back(particle);
}

void CParticleSystem::Free(void)
{
	if (!m_bClone)
		Safe_Release(m_pTexture);
	CComponent::Free();
	m_VB->Release();
}

void CParticleSystem::PreRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_Size));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.f));

	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.f));
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.f));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CParticleSystem::PostRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
