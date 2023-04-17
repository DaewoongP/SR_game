#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CTexture;
class CRcTex;
END

class CTopdeeParts :
	public CGameObject
{
public:
	explicit CTopdeeParts(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTopdeeParts();

	virtual HRESULT Ready_GameObject(
		_vec3& vPos, //���� ��ġ
		_tchar* texturename, //�ؽ��� �̸�
		_int idx, //������ �ؽ��� �ε���
		_bool setAnim);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

	void	SetRenderState(_bool value) { m_RenderState = value; }

protected:
	_float	m_dwTimer;
	_tchar* m_TextureName;
	_int	m_TextureIdx;
	_bool	m_RenderState;

	Engine::CTexture*		m_pTextureCom;
	Engine::CRcTex*			m_pBufferCom;

protected:
	HRESULT		Add_Component(void);

public:
	static CTopdeeParts*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		_vec3& vPos, //���� ��ġ
		CTransform* trans, //�θ�
		_tchar* texturename, //�ؽ��� �̸�
		_int idx, //������ �ؽ��� �ε���
		_bool setAnim); //�ִϸ��̼� ��� ����

protected:
	virtual void Free(void) override;
};

