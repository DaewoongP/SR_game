#pragma once
#include "Component.h"
#include "Export_Utility.h"
BEGIN(Engine)

//clip에 저장될 하나의 이동 정보 입니다.
typedef struct AnimInfo 
{
	_vec3 trans;
	_vec3 rotation;
	_vec3 scale;
	//언제까지 이동시켜라! 라고 사용할때 쓸 친구입니다.
	_ulong TillTime;
	//행동을 개시할 시간입니다.
	_ulong ActionTime; 
}ANIMINFO;

//재생할 애니메이션 클립입니다.
//하나의 애니메이션에 대한 각 파츠들의 이동/회전/스케일 조정 정보를 담고있습니다.
typedef struct AnimClip
{
	//행동이 적용될 파츠입니다.
	vector<CTransform*> parts;
	//행동에 대한 정보를 담고있는 친구입니다.
	vector<vector<ANIMINFO>> source;
	//애니메이션이 종료될 시간입니다.
	_ulong TotalTime;
	//하나의 애니메이션이 루프를 사용할지 아닐지를 정해주는 친구입니다.
	_bool Useloop;
}ANIMCLIP;

class CAnimation :
	public CComponent
{
private:
	explicit CAnimation();
	virtual ~CAnimation();

public:
	//애니메이션에 초기 준비입니다.
	HRESULT Ready_Animation(void);
	//애니메이션의 업데이트
	virtual _int Update_Component(const _float& fTimeDelta);
	//애니메이션의 late업데이트
	virtual void LateUpdate_Component(void);
	//애니메이션을 변경하는 친구입니다.
	HRESULT SetAnimation(_tchar* Name);
	HRESULT AddClip(_tchar* Name, ANIMCLIP* clip);

private:
	//애니메이션의 실제 동작부 입니다.
	void	DoUpdateClip(const _float& fTimeDelta);

private:
	//애니메이션 클립을 pair<tchar,clip>형식으로 넣어줍니다.
	//애니메이션 클립을 호출할 떄 사용하기 위함입니다.
	map<_tchar*, ANIMCLIP*> m_clipMap;
	//현재 재생해야할 애니메이션의 이름입니다.
	_tchar*					m_AnimState;
	_ulong					m_CurrentTime;

public:
	static CAnimation* Create();
	virtual CComponent * Clone(void) override;

private:
	virtual void Free(void) override;
};


END