#pragma once

#include "Typedef.h"

BEGIN(Engine)

typedef	 struct tagVertexColor
{
	_vec3			vPos;		// 위치
	_ulong			dwColor;	// 색상

}VTXCOL;

const	_ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef	 struct tagVertexTexture
{
	_vec3			vPos;		// 위치
	_vec2			vTexUV;		// 색상

}VTXTEX;

const	_ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

typedef	 struct tagVertexCube
{
	_vec3			vPos;		// 위치
	_vec3			vTexUV;		// 색상

}VTXCUBE;

const	_ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

typedef VTXCOL PTCCOL;
typedef VTXTEX PTCTEX;

// D3DFVF_TEXCOORDSIZE3 : 텍스처의 UV값이 FLOAT형 3개의 크기 만큼이며 괄호 안 숫자의 의미는 본래 버텍스의 UV 값이 여러개 있을 수 잇는데 그 중 첫 번째 값을 사용하겠다는 의미

typedef	struct tagIndex16
{
	_ushort		_0;
	_ushort		_1;
	_ushort		_2;

}INDEX16;

typedef	struct tagIndex32
{
	_ulong		_0;
	_ulong		_1;
	_ulong		_2;

}INDEX32;

struct Particle
{
	_vec3 vPos;
	_vec3 vVelocity;
	_vec3 vAccel;
	float fLifeTime;
	float fAge;
	_ulong dwColor;
	_ulong dwColorFade;
	bool bIsAlive;
};

struct Bound
{
	// 충돌했는지 아닌지 여부 검사
	virtual bool Intersect(const _vec3& point) PURE;
	// 게임오브젝트가 이동할때 같이 움직이게 하는 함수
	virtual void Offset(const _vec3& origin) PURE;
};



struct BoundingBox : public Bound
{
	enum COLDIR
	{
		COLDIR_TOP, BC_BOTTOM, BC_LEFT, BC_RIGHT
	};

	BoundingBox(const _vec3& offsetMin = { -1.f, -1.f, -1.f }, const _vec3& offsetMax = { 1.f, 1.f, 1.f })
	{
		_offsetMin = offsetMin;
		_offsetMax = offsetMax;
		_min = { 0.f,0.f,0.f };
		_max = { 0.f,0.f,0.f };
	}

	_vec3	Get_Center()
	{
		return (_max - _min) / 2 + _min;
	}

	_vec3	Get_Size()
	{
		return _offsetMax - _offsetMin;
	}

	// 지금은 큐브와 점의 충돌임
	virtual bool Intersect(const _vec3& point) override
	{
		if (point.x >= _min.x && point.y >= _min.y && point.z >= _min.z &&
			point.x <= _max.x && point.y <= _max.y && point.z <= _max.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// 오리진 위치를 받아와서 offsetMin 이랑 offsetMax만큼 더해줌 그 이유는 게임오브젝트가 움직였을때 범위가 달라져야 하기때문.
	virtual void Offset(const _vec3& origin) override
	{
		_min = origin + _offsetMin;
		_max = origin + _offsetMax;
	}

	// origin + 큐브의 크기 (큐브 월드 좌표)
	_vec3 _min;
	_vec3 _max;

	// 큐브의 크기 (큐브 로컬 좌표)
	_vec3 _offsetMin;
	_vec3 _offsetMax;
};

// 이게 충돌에 대한 정보를 담기위한 구조체인데 여기다가 무슨 방향으로 부딪혔는지 체크하는걸 넣으면 더 좋을거같아요
struct Collision
{
	// 어떤 게임오브젝트랑 충돌했는지, 그의 트랜스폼은 무엇인지,
	// 어떤 콜라이더랑 충돌했는지에 대한 정보에요
	class CGameObject* otherObj;
	class CTransform* otherTrans;
	class CCollider* otherCollider;
};

typedef struct tagViewParams
{
	_vec3 vEye;
	_vec3 vAt;
	_vec3 vUp;

	tagViewParams()
	{
		ZeroMemory(this, sizeof(tagViewParams));
	}
	tagViewParams(const _vec3& vEye, const _vec3& vAt, const _vec3& vUp)
	{
		this->vEye = vEye;
		this->vAt = vAt;
		this->vUp = vUp;
	}
	tagViewParams(const tagViewParams& rhs)
	{
		this->vEye = rhs.vEye;
		this->vAt = rhs.vAt;
		this->vUp = rhs.vUp;
	}
	D3DXMATRIX* LookAtLH(D3DXMATRIX* pMatView)
	{
		return D3DXMatrixLookAtLH(pMatView, &vEye, &vAt, &vUp);
	}
}VIEWPARAMS;

typedef struct tagProjParams
{
	_float fFov;
	_float fAspect;
	_float fNear;
	_float fFar;

	tagProjParams()
	{
		ZeroMemory(this, sizeof(tagProjParams));
	}
	tagProjParams(const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
	{
		this->fFov = fFov;
		this->fAspect = fAspect;
		this->fNear = fNear;
		this->fFar = fFar;
	}
	tagProjParams(const tagProjParams& rhs)
	{
		this->fFov = rhs.fFov;
		this->fAspect = rhs.fAspect;
		this->fNear = rhs.fNear;
		this->fFar = rhs.fFar;
	}
	D3DXMATRIX* PerspectiveLH(D3DXMATRIX* pMatProj)
	{
		return D3DXMatrixPerspectiveFovLH(pMatProj, fFov, fAspect, fNear, fFar);
	}
}PROJPARAMS;

END