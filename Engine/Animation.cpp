#include "stdafx.h"
#include "Animation.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphicDev):
	CComponent(pGraphicDev)
	,m_AnimState(0)
{
}


CAnimation::~CAnimation()
{
}

HRESULT CAnimation::Ready_Animation(void)
{
	return S_OK;
}

_int CAnimation::Update_Component(const _float & fTimeDelta)
{
	//애니메이션의 실제 동작부입니다.
	DoUpdateClip(fTimeDelta);

	return 0;
}

void CAnimation::LateUpdate_Component(void)
{
}

HRESULT CAnimation::SetAnimation(_tchar * Name)
{
	//만약 찾는 애니메이션이 존재하지 않는다면 FALSE를 리턴해줍니다.
	if (m_clipMap.find(Name)== m_clipMap.end())
		return E_NOTIMPL;

	//애니메이션의 현재 상태를 받은 매개변수로 지정해줍니다.
	m_AnimState = Name;
	//경과 시간을 초기화해줍니다.
	m_CurrentTime = 0;
	return S_OK;
}

HRESULT CAnimation::AddClip(_tchar * Name, ANIMCLIP* clip)
{
	//만약 해당 이름의 애니메이션이 이미 존재한다면 false를 리턴해줍니다.
	if (m_clipMap.find(Name)!=m_clipMap.end())
		return E_NOTIMPL;

	//아니면 맵에 정보를 추가해줍니다.
	m_clipMap.insert({ Name ,clip });
	return S_OK;
}

void CAnimation::DoUpdateClip(const _float& fTimeDelta)
{
	//애니메이션이 하나도 없다면 업데이트를 하지 않습니다.
	if (m_clipMap.empty())
		return;

	//애니메이션의 경과시간입니다.
	m_CurrentTime += fTimeDelta;

	//애니메이션의 클립정보를 가져옵니다.
	ANIMCLIP* clip= m_clipMap.at(m_AnimState);

	//경과시간이 clip의 총 시간을 넘었다면? 다시 0으로 초기화해줍니다.
	if (clip->Useloop&&m_CurrentTime > clip->TotalTime)
		m_CurrentTime = 0;
	//clip 의 각 파츠를 순회합니다.
	for (size_t i = 0; i < clip->parts.size(); i++)
	{
		//파츠에 등록된 애니메이션을 가져옵니다.
		for (size_t j = 0; j < clip->source[i].size(); j++)
		{
			//찾아온 info의 발동 시간이 지금 시간보다 아래라면?
			if (clip->source[i][j].ActionTime <= m_CurrentTime&&
				clip->source[i][j].ActionTime + clip->source[i][j].TillTime >= m_CurrentTime)
			{
				//지정된 rotation이 있다면 행동
				if (clip->source[i][j].rotation != _vec3(0, 0, 0))
					clip->parts[i]->m_vAngle = Lerp(clip->parts[i]->m_vAngle, clip->source[i][j].rotation,(1/clip->source[i][j].TillTime)* fTimeDelta);
				
				//지정된 scale이 있다면 행동
				if (clip->source[i][j].scale != _vec3(0, 0, 0))
					clip->parts[i]->m_vScale = Lerp(clip->parts[i]->m_vScale, clip->source[i][j].scale,(1/clip->source[i][j].TillTime) * fTimeDelta);

				//지정된 trans가 있다면 행동
				if (clip->source[i][j].trans != _vec3(0, 0, 0))
					clip->parts[i]->m_vInfo[INFO_POS] = Lerp(clip->parts[i]->m_vInfo[INFO_POS], clip->source[i][j].trans, (1/clip->source[i][j].TillTime)* fTimeDelta);
			
			}
		}
	}
	
}

CAnimation * CAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAnimation* pInstance = new CAnimation(pGraphicDev);

	if (FAILED(pInstance->Ready_Animation()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent * CAnimation::Clone(void)
{
	return new CAnimation(*this);
}

void CAnimation::Free(void)
{
	//할당해줬던 맵을해제해줍니다.
	for_each(m_clipMap.begin(), m_clipMap.end(), CDeleteMap_MY());
	m_clipMap.clear();
	__super::Free();
}
