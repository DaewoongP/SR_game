#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CTexture;
class CRcTex;
END

class CPatternParts :
	public CGameObject
{
private:
	explicit CPatternParts(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPatternParts();

	virtual HRESULT Ready_GameObject(
		_vec3& vPos, //생성 위치
		_tchar* texturename, //텍스쳐 이름
		_int idx, //랜더할 텍스쳐 인덱스
		_bool setAnim);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	void	SetTextureIdx(_int idx) { m_TextureIdx = idx; }
	_int	GetTextureIdx() { return m_TextureIdx; }

protected:
	_float	m_dwTimer;
	_tchar* m_TextureName;
	_int	m_TextureIdx;

	Engine::CTexture*		m_pTextureCom;
	Engine::CRcTex*			m_pBufferCom;

protected:
	HRESULT		Add_Component(void);

public:
	static CPatternParts*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		_vec3& vPos, //생성 위치
		CTransform* trans, //부모
		_tchar* texturename, //텍스쳐 이름
		_int idx, //랜더할 텍스쳐 인덱스
		_bool setAnim); //애니메이션 사용 유무
protected:
	virtual void Free(void) override;

};

