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
	// ���������� ������ ���� ���¸� �������� ����
	void Set_PreCol() { _bPreCol = _bCurCol; }
	_bool Get_PreCol() { return _bPreCol; }
	//		���������ӿ� �浹���̿����� Ȯ��
	_bool	_bPreCol = false;
	_bool	_bCurCol = false;

	// �浹�� ����
	COL_DIR		_dir;
	// ���� enter, stay, exit �Ǵ�
	COL_STATE	_CurState;
	class CCollider*	otherCol;
	class CGameObject* otherObj;
};

END