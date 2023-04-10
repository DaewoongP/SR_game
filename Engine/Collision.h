#pragma once
#include "Include.h"
BEGIN(Engine)
class Collision
{
public:
	Collision() {}
	~Collision() {}
	void Set_ColDir(COL_DIR dir) { _dir = dir; }
	COL_DIR Get_ColDir() { return _dir; }
	COL_STATE Set_Curcol(_bool curcol) {
		if (false == _bPreCol && true == curcol)
		{
			_CurState = COLSTATE_ENTER;
		}
		else if (true == _bPreCol && true == curcol)
		{
			_CurState = COLSTATE_STAY;
		}
		else if (true == _bPreCol && false == curcol)
		{
			_CurState = COLSTATE_EXIT;
		}
		_bCurCol = curcol;
		return _CurState;
	}
	// 다음프레임 가기전 현재 상태를 이전값에 저장
	void Set_PreCol() { _bPreCol = _bCurCol; }
	_bool Get_PreCol() { return _bPreCol; }
	//		이전프레임에 충돌중이였는지 확인
	_bool	_bPreCol = false;
	_bool	_bCurCol = false;

	// 충돌한 방향
	COL_DIR		_dir;
	// 현재 enter, stay, exit 판단
	COL_STATE	_CurState;
	class CCollider*	otherCol;
	class CGameObject* otherObj;
};

END