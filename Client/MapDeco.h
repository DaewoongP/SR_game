#pragma once

#include "Include.h"
#include "BackGroundBase.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CMapDeco:
	public CBackGroundBase
{
private:
	explicit CMapDeco(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapDeco();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	CUBE_DIR Get_CubeDir() { return m_eDir; }
	HRESULT Add_Component(void);

	void Set_CubeDir(CUBE_DIR dir);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
public:
	static CMapDeco* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos,_int eDir);

private:
	virtual void Free(void);
	CUBE_DIR				m_eDir;	
	_vec3					m_DirVec;


};

