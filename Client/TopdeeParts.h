#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CTexture;
class CRcTex;
class CShader;
END

class CTopdeeParts :
	public CGameObject
{
public:
	explicit CTopdeeParts(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTopdeeParts();

	virtual HRESULT Ready_GameObject(
		_vec3& vPos, //생성 위치
		_tchar* texturename, //텍스쳐 이름
		_int idx, //랜더할 텍스쳐 인덱스
		_bool setAnim);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

	void	SetRenderState(_bool value) { m_RenderState = value; }
	void	SetTextureIdx(_int idx) { m_TextureIdx = idx; }
	_int	GetTextureIdx() { return m_TextureIdx; }
	void	MakeAnim(_tchar* name, _int startidx, _int lastidx, _float time, _bool loop) { m_pTextureCom->Add_Anim(name, startidx, lastidx, time, loop); }
	void	SetAnim(_tchar* name) { m_pTextureCom->Switch_Anim(name); }
	_bool	GetDieAnimEnd() {return m_pTextureCom->IsAnimationEnd(L"Die");}

protected:
	_float	m_dwTimer;
	_tchar* m_TextureName;
	_int	m_TextureIdx;
	_bool	m_RenderState;

	Engine::CTexture*		m_pTextureCom;
	Engine::CRcTex*			m_pBufferCom;
	Engine::CShader*		m_pShader;
protected:
	HRESULT		Add_Component(void);

public:
	static CTopdeeParts*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		_vec3& vPos, //생성 위치
		CTransform* trans, //부모
		_tchar* texturename, //텍스쳐 이름
		_int idx, //랜더할 텍스쳐 인덱스
		_bool setAnim); //애니메이션 사용 유무

protected:
	virtual void Free(void) override;
};

