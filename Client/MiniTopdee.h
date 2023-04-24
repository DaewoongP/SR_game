#pragma once
#include "GameObject.h"

#define		TOPY			18.f
#define		BOTY			8.f
#define		SHOOTCUBETIME	0.5f
BEGIN(Engine)
class CRcTex;
class CTexture;
END

class CMiniTopdee : public CGameObject
{
private:
	explicit CMiniTopdee(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniTopdee();

public:
	HRESULT Ready_GameObject(CLayer* pLayer, _vec3& vPos);
	virtual _int	Update_GameObject(const _float& fTimeDelta) final;
	virtual void	LateUpdate_GameObject(void) final;
	virtual void	Render_GameObject(void) final;
private:
	HRESULT			Add_Component(void);
	void			MoveY(const _float& fTimeDelta);
	void			ShootCube(const _float& fTimeDelta);
	template<typename T>
	void			CreateCube(const _tchar* pTag);
	template<typename T>
	void			CreateCubewithDir(const _tchar* pTag, _int iDir);
public:
	static CMiniTopdee* Create(LPDIRECT3DDEVICE9 pGraphicDevm, _vec3& vPos, CLayer* pLayer);
private:
	virtual void Free() final;

private:
	Engine::CRcTex*			m_pBuf;
	Engine::CTexture*		m_pTex;

	_float					m_fSpeed;
	CLayer*					m_pLayer;
	_float					m_fCubeSpeed;
	vector<CGameObject*>	m_vecCube;

	_int					m_iChkPortalCube;
};
