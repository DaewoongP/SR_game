#pragma once

enum LOADINGID { LOADING_LOGO, LOADING_STAGE1, LOADING_STAGE2, LOADING_STAGE3, LOADING_STAGE4, LOADING_FINAL1, LOADING_BGTOOL, LOADING_END };
enum WINMODE { MODE_FULL, MODE_WIN };

namespace Engine
{
	// ID_DYNAMIC : 매 프레임마다 갱신을 해야하는 컴포넌트 집단
	enum COMPONENTID{ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO {INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };
	enum TEXTYPE { TEX_NORMAL, TEX_CUBE, TEX_END };
	enum RENDERID { RENDER_PRIORITY, RENDER_ALPHA, RENDER_NONALPHA, RENDER_UI, RENDER_END };
	enum CHANNELID { SOUND_BGM, SOUND_EFFECT,SOUND_EFFECT_GIMMICK, SOUND_EFFECT_ENEMY,SOUND_CAM,MAXCHANNEL };
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum COLGROUP
	{
		COL_ENV,
		COL_OBJ,
		COL_END
	};

	enum COL_DIR
	{
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
		DIR_FRONT,
		DIR_BACK,
		DIR_END
	};

	enum COL_STATE
	{
		COLSTATE_ENTER,
		COLSTATE_STAY,
		COLSTATE_EXIT,
		COLSTATE_NONE,
		COLSTATE_END
	};

	enum KEYSTATE
	{
		KEYDOWN,
		KEYPRESS,
		KEYUP,
		KEYNONE
	};

	enum TOPDEESTATE 
	{
		TD_IDLE,
		TD_MOVE,
		TD_FINDING,
		TD_SOMETHING
	};

	enum CUBE_HANDING
	{
		CH_NONE,
		CH_START,
		CH_ING,
		CH_END
	};

	enum COL_MOVEDIR
	{
		MD_UP,
		MD_DOWN,
		MD_LEFT,
		MD_RIGHT,
		MD_RU,
		MD_LU,
		MD_RD,
		MD_LD,
		MD_END
	};

	enum SHAKE_AXIS
	{
		SHAKE_ALL,
		SHAKE_X,
		SHAKE_Y,
		SHAKE_Z,
		SHAKE_END
	};
	enum CUBE_DIR
	{
		CD_UP,
		CD_DOWN,
		CD_LEFT,
		CD_RIGHT,
		CD_END
	};

	enum FLOATING_AXIS
	{
		FLOATING_X,
		FLOATING_Y,
		FLOATING_Z,
		FLOATING_XY,
		FLOATING_XZ,
		FLOATING_YZ,
		FLOATING_XYZ,
		FLOATING_END
	};
}
