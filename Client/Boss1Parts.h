#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CTexture;
class CRcTex;
END

class CBoss1Parts :
	public CGameObject
{
private:
	explicit CBoss1Parts(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss1Parts();

	virtual HRESULT Ready_GameObject(
		_vec3& vPos, //���� ��ġ
		_tchar* texturename, //�ؽ��� �̸�
		_int idx, //������ �ؽ��� �ε���
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
	static CBoss1Parts*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		_vec3& vPos, //���� ��ġ
		CTransform* trans, //�θ�
		_tchar* texturename, //�ؽ��� �̸�
		_int idx, //������ �ؽ��� �ε���
		_bool setAnim); //�ִϸ��̼� ��� ����
protected:
	virtual void Free(void) override;

};

