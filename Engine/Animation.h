#pragma once
#include "Component.h"
#include "Export_Utility.h"
BEGIN(Engine)

//clip�� ����� �ϳ��� �̵� ���� �Դϴ�.
typedef struct AnimInfo 
{
	_vec3 trans;
	_vec3 rotation;
	_vec3 scale;
	//�������� �̵����Ѷ�! ��� ����Ҷ� �� ģ���Դϴ�.
	_ulong TillTime;
	//�ൿ�� ������ �ð��Դϴ�.
	_ulong ActionTime; 
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
	_ulong TotalTime;
	//�ϳ��� �ִϸ��̼��� ������ ������� �ƴ����� �����ִ� ģ���Դϴ�.
	_bool Useloop;
}ANIMCLIP;

class CAnimation :
	public CComponent
{
private:
	explicit CAnimation();
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

private:
	//�ִϸ��̼��� ���� ���ۺ� �Դϴ�.
	void	DoUpdateClip(const _float& fTimeDelta);

private:
	//�ִϸ��̼� Ŭ���� pair<tchar,clip>�������� �־��ݴϴ�.
	//�ִϸ��̼� Ŭ���� ȣ���� �� ����ϱ� �����Դϴ�.
	map<_tchar*, ANIMCLIP*> m_clipMap;
	//���� ����ؾ��� �ִϸ��̼��� �̸��Դϴ�.
	_tchar*					m_AnimState;
	_ulong					m_CurrentTime;

public:
	static CAnimation* Create();
	virtual CComponent * Clone(void) override;

private:
	virtual void Free(void) override;
};


END