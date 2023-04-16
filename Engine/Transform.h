#pragma once

#include "Component.h"
#include "Include.h"

BEGIN(Engine)

class  CTransform : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	const _matrix*		Get_WorldMatrixPointer() { return &m_matWorld; }
	void				Get_Info(INFO eType, _vec3* pInfo)
	{
		*pInfo = m_vInfo[eType];
	}
	const _vec3& Get_Scale()
	{
		return m_vScale;
	}

	void Set_Pos(const _float& x, const _float& y, const _float& z)
	{
		m_vInfo[INFO_POS] = { x,y,z };
	}

	void				Move_Pos(_vec3* pDir, const _float& fTimeDelta, const _float& fSpeed)
	{
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(pDir, pDir) * fSpeed * fTimeDelta;
	}

	void		Rotation(ROTATION eRot, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eRot) += fAngle;
	}

	void	Set_BillboardX(_matrix* _matView)
	{
		D3DXMatrixIdentity(&m_matBillX);
		m_matBillX._22 = _matView->_22;
		m_matBillX._23 = _matView->_23;
		m_matBillX._32 = _matView->_32;
		m_matBillX._33 = _matView->_33;

		D3DXMatrixInverse(&m_matBillX, 0, &m_matBillX);
	}

	void	Set_BillboardY(_matrix* _matView)
	{
		D3DXMatrixIdentity(&m_matBillY);
		m_matBillY._11 = _matView->_11;
		m_matBillY._13 = _matView->_13;
		m_matBillY._31 = _matView->_31;
		m_matBillY._33 = _matView->_33;

		D3DXMatrixInverse(&m_matBillY, 0, &m_matBillY);
	}

	void			MakeMyMatrix(const _float& fTimeDelta);
	void			Set_ParentTransform(CGameObject * pParentObject, _float fX, _float fY, _float fZ);
	void			Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	const _matrix*	Compute_Lookattarget(const _vec3* pTargetPos);
public:
	HRESULT		Ready_Transform(void);
	virtual		_int	Update_Component(const _float& fTimeDelta);
	virtual		void	LateUpdate_Component(void);

	void Move_Floating(const _float& fTimeDelta, _float fPower = 10.0f, _float fSpeed = 60.0f);

	void		Update_Shake(_float fTimeDelta, _vec3& vPos);
	//����ũ �ð�, ���� �Է� �Լ�
	void		Set_Shake(_float fDuration, _int iPower, SHAKE_AXIS eShake_Axis = Engine::SHAKE_ALL)
	{
		m_fShakeTimer = fDuration;
		m_fShakePower = (_float)iPower;
		m_iShakeAxis = (_int)eShake_Axis;
		m_fWeakPoint = (_float)iPower / fDuration;
	}

	void SetParent(CTransform* trans) {
		if (trans != nullptr)
			m_Parent = trans;
		trans->m_Child.push_back(this);
	}
	CTransform* GetParent() { return m_Parent; }
	vector<CTransform*> GetChildren() { return m_Child; }
	CTransform * GetChild(size_t index) {
		if (index <= m_Child.size())
			return m_Child[index];
		else
			return nullptr;
	}
	_int		GetChildCount() { return m_Child.size(); }
	_matrix GetRotMat();
	_matrix GetRevolutionRotMat();
	_matrix GetTransMat(const _float& fTimeDelta);
	_matrix GetScaleMat();
	void SwapYZ();
private:
	CTransform* m_Parent;
	vector<CTransform*> m_Child;

public:
	_bool		m_bIsStatic = true;
	_vec3		m_vInfo[INFO_END];
	_vec3		m_vScale;
	_vec3		m_vAngle;
	_vec3		m_vRevolutionAngle;
	_matrix		m_matWorld;
	_matrix		m_matBillX;
	_matrix		m_matBillY;

	_matrix		m_matRT;

	_matrix		m_matPerantScale;

	_bool		m_bMove;

	_float		m_fFloating = 0.0f;
	//����ũ ����
	_bool	m_bUseWeak = false;
	_float	m_fWeakPoint = 0.0f;
	_int	m_iShakeAxis = Engine::SHAKE_END;
	_float	m_fShakeTimer = 0.0f;
	_float	m_fShakePower = 0;
	_vec3	m_vShakeOffset;

	_float m_YZValue;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);

private:
	virtual void	Free(void) override;

};
END