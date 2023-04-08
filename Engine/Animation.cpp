#include "stdafx.h"
#include "Animation.h"

CAnimation::CAnimation()
	: m_AnimState(0)
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
	//�ִϸ��̼��� ���� ���ۺ��Դϴ�.
	DoUpdateClip(fTimeDelta);

	return 0;
}

void CAnimation::LateUpdate_Component(void)
{
}

HRESULT CAnimation::SetAnimation(_tchar * Name)
{
	//���� ã�� �ִϸ��̼��� �������� �ʴ´ٸ� FALSE�� �������ݴϴ�.
	if (m_clipMap.at(Name))
		return E_NOTIMPL;

	//�ִϸ��̼��� ���� ���¸� ���� �Ű������� �������ݴϴ�.
	m_AnimState = Name;
	//��� �ð��� �ʱ�ȭ���ݴϴ�.
	m_CurrentTime = 0;
	return S_OK;
}

HRESULT CAnimation::AddClip(_tchar * Name, ANIMCLIP* clip)
{
	//���� �ش� �̸��� �ִϸ��̼��� �̹� �����Ѵٸ� false�� �������ݴϴ�.
	if (m_clipMap.at(Name))
		return E_NOTIMPL;

	//�ƴϸ� �ʿ� ������ �߰����ݴϴ�.
	m_clipMap.insert({ Name ,clip });
	return S_OK;
}

void CAnimation::DoUpdateClip(const _float& fTimeDelta)
{
	//�ִϸ��̼��� �ϳ��� ���ٸ� ������Ʈ�� ���� �ʽ��ϴ�.
	if (m_clipMap.empty())
		return;

	//�ִϸ��̼��� ����ð��Դϴ�.
	m_CurrentTime += fTimeDelta;

	//�ִϸ��̼��� Ŭ�������� �����ɴϴ�.
	ANIMCLIP* clip= m_clipMap.at(m_AnimState);

	//����ð��� clip�� �� �ð��� �Ѿ��ٸ�? �ٽ� 0���� �ʱ�ȭ���ݴϴ�.
	if (m_CurrentTime > clip->TotalTime)
		m_CurrentTime = 0;
	//clip �� �� ������ ��ȸ�մϴ�.
	for (int i = 0; i < clip->parts.size(); i++)
	{
		//������ ��ϵ� �ִϸ��̼��� �����ɴϴ�.
		for (int j = 0; j < clip->source[i].size(); j++)
		{
			//ã�ƿ� info�� �ߵ� �ð��� ���� �ð����� �Ʒ����?
			if (clip->source[i][j].ActionTime < m_CurrentTime)
			{
				//�ൿ�� ��������߰���
				//�ش�� ������ dwTime���� ��ȯ�����ִ� �ൿ�� �մϴ�. 
				//�θ��� world matrix����
				//clip->parts[i]
			}
		}
	}
	
}

CAnimation * CAnimation::Create()
{
	CAnimation* pInstance = new CAnimation();

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
	//�Ҵ������ �����������ݴϴ�.
	/*for_each(m_clipMap.begin(), m_clipMap.end(), CDeleteMap());
	m_clipMap.clear();*/
	__super::Free();
}
