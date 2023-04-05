#include "stdafx.h"
#include "WindParticle.h"


//CWindParticle::CWindParticle(BoundingBox* boundingBox, int numParticles)
//{
//	m_BoundingBox = *boundingBox;
//	m_fSize = 0.25f;
//	m_dwSize = 2048;
//	m_dwOffset = 0;
//	m_dwBatchSize = 512;
//
//	for (int i = 0; i < numParticles; i++)
//		addParticle();
//}
//
//void CWindParticle::resetParticle(Particle* attribute)
//{
//	attribute->bIsAlive = true;
//
//	// get random x, z coordinate for the position of the snow flake.
//	GetRandomVector(
//		&attribute->vPos,
//		&m_BoundingBox._min,
//		&m_BoundingBox._max);
//
//	// no randomness for height (y-coordinate).  Snow flake
//	// always starts at the top of bounding box.
//	attribute->vPos.y = m_BoundingBox._max.y;
//
//	// snow flakes fall downwards and slightly to the left
//	attribute->vVelocity.x = GetRandomFloat(0.0f, 1.0f) * -3.0f;
//	attribute->vVelocity.y = GetRandomFloat(0.0f, 1.0f) * -10.0f;
//	attribute->vVelocity.z = 0.0f;
//
//	// white snow flake
//	attribute->dwColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//}
//
//void CWindParticle::update(float timeDelta)
//{
//	list<Particle>::iterator i;
//	for (i = m_ParticlesList.begin(); i != m_ParticlesList.end(); i++)
//	{
//		i->vPos += i->vVelocity* timeDelta;
//
//		// is the point outside bounds?
//		if (m_BoundingBox.isPointInside(i->vPos) == false)
//		{
//			// nope so kill it, but we want to recycle dead 
//			// particles, so respawn it instead.
//			resetParticle(&(*i));
//		}
//	}
//}