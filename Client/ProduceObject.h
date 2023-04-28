#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END
class CProduceObject :
	public CGameObject
{
protected:
	explicit CProduceObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CProduceObject();

protected:
	void Update_Produce(const _float& fTimeDelta);
	void On_Next(int iIndex);
public:
	_bool Get_Next() { return m_bNext; }
protected:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
protected:
	_float m_fProdTimer;
	// XY½Ã°£
	vector<_vec3> m_vecLerpList;
	_int m_iIndex;
	_bool m_bStart;
	_bool m_bNext;
	LOADINGID m_eID;
protected:
	virtual void		Free(void);
};

