#include "stdafx.h"
#include "Texture.h"



CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_bUseFrameAnimation(false)
	, m_fTimer(0)
	, m_CurIdx(0)
	, m_bisLoop(false)
{
	ZeroMemory(&m_CurrentAnim, sizeof(Anim_Info));
}


CTexture::CTexture(const CTexture & rhs)
	:CComponent(rhs)
	, m_bUseFrameAnimation(rhs.m_bUseFrameAnimation)
	, m_fTimer(rhs.m_fTimer)
	, m_CurrentAnim(rhs.m_CurrentAnim)
	, m_CurIdx(rhs.m_CurIdx)
	, m_AnimMap(rhs.m_AnimMap)
	, m_bisLoop(rhs.m_bisLoop)
{
	_uint	iSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iSize);

	m_vecTexture = rhs.m_vecTexture;

	for (auto& iter : m_vecTexture)
		iter->AddRef();
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(TEXTYPE eTextype, const _tchar * pPath, const _uint & iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*		pTexture = nullptr;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR	szFileName[256] = L"";

		wsprintf(szFileName, pPath, i);

		switch (eTextype)
		{
		case TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;

		case TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		}
	
		m_vecTexture.push_back(pTexture);
	}
	
	return S_OK;
}

void CTexture::Add_Anim(_tchar * name, int startIdx, int endIdx, _float fCycle,bool loop)
{
	m_AnimMap.insert({ name,Anim_Info{startIdx,endIdx,fCycle,loop} });
}

void CTexture::Switch_Anim(_tchar * name)
{
	if (m_CurrentAnimName == name)
		return;

	auto iter = find_if(m_AnimMap.begin(), m_AnimMap.end(), CTag_Finder(name));

	if (iter == m_AnimMap.end())
		return;
	m_CurrentAnimName = name;
	m_fTimer = 0;
	m_CurrentAnim = iter->second;
	m_bisLoop = m_CurrentAnim.bisLoop;
}

void CTexture::Update_Anim(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;

	if (m_fTimer > m_CurrentAnim.fCycle)
		if (m_bisLoop)
			m_fTimer = 0;
		else
			m_fTimer = m_CurrentAnim.fCycle;

	int iIndex = (m_CurrentAnim.iEndIdx - m_CurrentAnim.iStartIdx) * (m_fTimer/ m_CurrentAnim.fCycle) + m_CurrentAnim.iStartIdx;
	m_CurIdx = iIndex;
}

void CTexture::Set_Texture(const _uint & iIndex)
{
	if (m_vecTexture.size() <= iIndex)
		return;
	if (!m_bUseFrameAnimation)
		m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
	else
		m_pGraphicDev->SetTexture(0, m_vecTexture[m_CurIdx]);
}

_bool CTexture::IsAnimationEnd(_tchar* name)
{
	auto iter = find_if(m_AnimMap.begin(), m_AnimMap.end(), CTag_Finder(name));

	if (iter == m_AnimMap.end())
		return false;

	if (iter->second.iEndIdx == m_CurIdx)
		return true;

	return false;
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTYPE eTextype, const _tchar * pPath, const _uint & iCnt)
{
	CTexture *	pInstance = new	CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(eTextype, pPath, iCnt)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CTexture::Clone(void)
{
	return new CTexture(*this);
}

void CTexture::Free(void)
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();
	m_vecTexture.shrink_to_fit();

	__super::Free();

}
