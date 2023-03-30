#include "stdafx.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "Transform.h"



CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphicDev) :
	CComponent(pGraphicDev),
	m_BoundingBox({0.f,0.f,0.f},{0.f,0.f,0.f})
{
}

CParticleSystem::CParticleSystem(const CParticleSystem & rhs) :
	CComponent(rhs),
	m_VB(rhs.m_VB),
	m_VBSize(rhs.m_VBSize),
	m_VBOffset(rhs.m_VBOffset),
	m_VBBatchSize(rhs.m_VBBatchSize),
	m_Origin(rhs.m_Origin),
	m_Texture(rhs.m_Texture),
	m_EmitRate(rhs.m_EmitRate),
	m_Size(rhs.m_Size),
	m_BoundingBox(rhs.m_BoundingBox)
{
	m_VB->AddRef();
	/*m_Texture->AddRef();*/
}

CParticleSystem::~CParticleSystem()
{
}

HRESULT CParticleSystem::Ready_Particle(void)
{
	FAILED_CHECK_RETURN
	(
		m_pGraphicDev->CreateVertexBuffer(
			m_VBSize * sizeof(PTCCOL),
			D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
			FVF_COL,
			D3DPOOL_DEFAULT,
			&m_VB,
			0),
		E_FAIL
	);

	return S_OK;
}

_int CParticleSystem::Update_Component(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Component(fTimeDelta);
	if (iExit != 0) return iExit;

	_vec3 offsetPoint;
	m_pGameObject->m_pTransform->Get_Info(INFO_POS, &offsetPoint);

	m_BoundingBox.Offset(offsetPoint);

	return 0;
}

void CParticleSystem::LateUpdate_Component()
{

}

void CParticleSystem::Render_Particle(void)
{
	if (!m_Particles.empty())
	{
		PreRender();

		m_pGraphicDev->SetTexture(0, m_Texture);
		m_pGraphicDev->SetFVF(FVF_COL);
		m_pGraphicDev->SetStreamSource(0, m_VB, 0, sizeof(PTCCOL));

		if (m_VBOffset >= m_VBSize)
			m_VBOffset = 0;

		PTCCOL* v = 0;

		m_VB->Lock(
			m_VBOffset * sizeof(PTCCOL),
			m_VBBatchSize * sizeof(PTCCOL),
			(void**)&v,
			m_VBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		list<Particle>::iterator it;
		for (it = m_Particles.begin(); it != m_Particles.end(); it++)
		{
			if (it->bIsAlive)
			{
				v->vPos = it->vPos;
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
						m_VBOffset * sizeof(PTCCOL),
						m_VBBatchSize * sizeof(PTCCOL),
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
	bool ret;

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
	CComponent::Free();
	m_VB->Release();
	//m_Texture->Release();

	
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
