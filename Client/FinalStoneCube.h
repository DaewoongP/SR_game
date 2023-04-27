#pragma once
#include "GameObject.h"
#define		LERPCUBEZ		-30.f
BEGIN(Engine)
class CCircularParticle;
END
class CBoss3;
class CFinalStoneCube : public CGameObject
{
private:
	explicit CFinalStoneCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalStoneCube();
public:
	HRESULT			Ready_GameObject(_vec3& vPos, CLayer* pLayer);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject();
	void	StartLerp() { m_bTrigger = true; }
private:
	HRESULT			Ready_MapCubes();

	void			Cube_Lerp(const _float& fTimeDelta, _int iIndex);
	void			Make_Boss3();
	_bool			Start_BossPattern(const _float& fTimeDelta);
	_bool			End_Pattern(const _float& fTimeDelta);
	void			Make_Particle();
private:
	vector<pair<CGameObject*, _vec3>>	m_vecCube;

	_vec3						m_vInitPos;
	vector<_float>				m_vecLerp;
	_float						m_fLerp;
	_float						m_fSpeed;
	_bool						m_bStart;
	_bool						m_bTrigger;
	_bool						m_bEnd;
	_int						m_iBossSpawn;
	_float						m_fTime;
	CLayer*						m_pGameLogicLayer = nullptr;
	CBoss3*						m_pBoss;

	Engine::CCircularParticle*	m_pLandingParticle;
public:
	static CFinalStoneCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, CLayer* pLayer);
private:
	virtual void Free();
};

