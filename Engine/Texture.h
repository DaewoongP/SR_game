#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class  CTexture : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT			Ready_Texture(TEXTYPE eTextype, const _tchar* pPath, const _uint& iCnt = 1);
	void			Add_Anim(_tchar* name, int startIdx, int endIdx, _float dwCycle,bool loop);
	void			Set_Loop(_tchar* name);
	void			Switch_Anim(_tchar* name);
	void			Update_Anim(const _float & fTimeDelta);
	void			Set_Texture(const _uint& iIndex = 0);
	void			Set_Cube(_tchar* pProtoTag);
	_bool			IsAnimationEnd(_tchar* name);
	_tchar*			Get_AnimState() { return m_CurrentAnimName; }
	
	void			Reset_Anim() 
	{
		m_fTimer = 0;
		m_CurIdx = 0; 
	}
public:
	_bool			m_bUseFrameAnimation;

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;
	unordered_map<_tchar*, Anim_Info>	m_AnimMap;

	Anim_Info m_CurrentAnim;
	_tchar* m_CurrentAnimName;
	_int m_CurIdx;
	bool m_bisLoop;
	_float m_fTimer;

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTYPE eTextype, const _tchar* pPath, const _uint& iCnt = 1);
	virtual CComponent*		Clone(void);

private:
	virtual void Free(void);
};

END