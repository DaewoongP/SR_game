#pragma once
#include "Component.h"
#include "Export_Utility.h"
BEGIN(Engine)

enum VECTYPE
{
	VECTYPE_TRANS,
	VECTYPE_ROTATION,
	VECTYPE_SCALE
};

//clip�� ����� �ϳ��� �̵� ���� �Դϴ�.
typedef struct AnimInfo 
{
	_vec3 trans;
	_vec3 rotation;
	_vec3 scale;
	//�������� �̵����Ѷ�! ��� ����Ҷ� �� ģ���Դϴ�.
	_float TillTime;
	//�ൿ�� ������ �ð��Դϴ�.
	_float ActionTime;
}ANIMINFO;

//����� �ִϸ��̼� Ŭ���Դϴ�.
//�ϳ��� �ִϸ��̼ǿ� ���� �� �������� �̵�/ȸ��/������ ���� ������ ����ֽ��ϴ�.
typedef struct AnimClip
{
	//�ൿ�� ����� �����Դϴ�.
	vector<CTransform*> parts;
	//�ൿ�� ���� ������ ����ִ� ģ���Դϴ�.
	vector<vector<ANIMINFO>> source;
	//�ִϸ��̼��� ����� �ð��Դϴ�.
	_float TotalTime;
	//�ϳ��� �ִϸ��̼��� ������ ������� �ƴ����� �����ִ� ģ���Դϴ�.
	_bool Useloop;
}ANIMCLIP;

class CAnimation :
	public CComponent
{
private:
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CAnimation();

public:
	//�ִϸ��̼ǿ� �ʱ� �غ��Դϴ�.
	HRESULT Ready_Animation(void);
	//�ִϸ��̼��� ������Ʈ
	virtual _int Update_Component(const _float& fTimeDelta);
	//�ִϸ��̼��� late������Ʈ
	virtual void LateUpdate_Component(void);
	//�ִϸ��̼��� �����ϴ� ģ���Դϴ�.
	HRESULT SetAnimation(_tchar* Name);
	HRESULT AddClip(_tchar* Name, ANIMCLIP* clip);
	_tchar*	GetAnimationName() { return m_AnimState; }
	_bool	GetAnimEnd() { return m_clipMap.at(m_AnimState)->TotalTime< m_CurrentTime; }

	//�Ӹ��븸�� ���� �������.
	void	DynamicChangeAnimation(_tchar* name, _int partIdx, _int sourceIdx, VECTYPE type, _vec3 value);

private:
	//�ִϸ��̼��� ���� ���ۺ� �Դϴ�.
	void	DoUpdateClip(const _float& fTimeDelta);

private:
	//�ִϸ��̼� Ŭ���� pair<tchar,clip>�������� �־��ݴϴ�.
	//�ִϸ��̼� Ŭ���� ȣ���� �� ����ϱ� �����Դϴ�.
	map<_tchar*, ANIMCLIP*> m_clipMap;
	//���� ����ؾ��� �ִϸ��̼��� �̸��Դϴ�.
	_tchar*					m_AnimState;
	_float					m_CurrentTime;

public:
	static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent * Clone(void) override;

private:
	virtual void Free(void) override;
};


END