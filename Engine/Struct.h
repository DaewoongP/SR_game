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
	_ulong			dwColor;	// 색상
	_vec2			vTexUV;		// 색상

}VTXTEX;

const	_ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

typedef	 struct tagVertexCube
{
	_vec3			vPos;		// 위치
	_vec3			vTexUV;		// 색상

}VTXCUBE;

const	_ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


typedef VTXCOL PTCCOL;
typedef VTXTEX PTCTEX;

const	_ulong		FVF_PTC = D3DFVF_XYZ | D3DFVF_DIFFUSE |	D3DFVF_TEX1;

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

struct Anim_Info
{
	_int iStartIdx;
	_int iEndIdx;
	_float fCycle;
	_bool bisLoop;
};

struct Particle
{
	_vec3 vPos;
	_vec3 vVelocity;
	_vec3 vAccel;
	_float fGenTime = 0;
	_float fLifeTime;
	_float fAge;
	_float fSizeoverLifetime;
	_ulong dwColor;
	_ulong dwColorFade;
	_vec2	vTexUV;
	_bool bIsAlive;
};

struct BoundingBox
{
	BoundingBox(const _vec3& offsetMin = { -1.f, -1.f, -1.f }, 
		const _vec3& offsetMax = { 1.f, 1.f, 1.f },
		const _vec3& offsetPos = {0.f,0.f,0.f})
	{
		_offsetMin = offsetMin;
		_offsetMax = offsetMax;
		_min = { 0.f,0.f,0.f };
		_max = { 0.f,0.f,0.f };
		_offPos = offsetPos;
	}
	_vec3	Get_Offset()
	{
		return _offPos;
	}

	void	Set_Offset(const _vec3& vOffset)
	{
		_offPos = vOffset;
	}

	_vec3	Get_Center()
	{
		return (_max - _min) / 2 + _min;
	}

	_vec3	Get_Size()
	{
		return _offsetMax - _offsetMin;
	}

	bool Intersect(const _vec3& point)
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
	void Offset(const _vec3& origin)
	{
		_min = origin + _offsetMin + _offPos;
		_max = origin + _offsetMax + _offPos;
	}

	// origin + 큐브의 크기 (큐브 월드 좌표)
	_vec3 _min;
	_vec3 _max;

	// 큐브의 크기 (큐브 로컬 좌표)
	_vec3 _offsetMin;
	_vec3 _offsetMax;

	// 현재 객체 중점에서 콜라이더 중점까지의 오프셋
	_vec3	_offPos;
};

struct RayCollision 
{
	_tchar* tag;
	class CCollider* col;
	float dist;
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
	tagViewParams(const _vec3& vEye, const _vec3& vAt, const _vec3& vUp = { 0.0f,1.0f,0.0f })
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

	//tagProjParams()
	//{
	//	ZeroMemory(this, sizeof(tagProjParams));
	//}
	tagProjParams(const _float& fFov = D3DXToRadian(90), const _float& fAspect = (float)WINCX / WINCY, const _float& fNear = 0.0f, const _float& fFar = 1000.0f)
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

typedef struct tagRay
{
	_vec3 _origin;
	_vec3 _direction;
	float _Length;

	tagRay(_vec3 origin, _vec3 dir)
	{
		_origin = origin;
		D3DXVec3Normalize(&_direction,&dir);
		_Length = D3DXVec3Length(&dir);
	}

	tagRay(_vec3 origin, _vec3 dir, float Length)
	{
		_origin = origin;
		_direction = dir;
		_Length = Length;
	}

}RAYCAST;

END

// 위치값과 종류를 저장하기 위한 구조체
typedef struct ObjectInfo
{
	_vec3		vObjPos;
	_int		iObjTypeNumber;
	const _tchar* pObjtag;
	_vec3 vObjScale;
	_float fAngle;
}OBJINFO;

typedef struct BGInfo
{
	_vec3		vObjPos;
	_int		iObjTypeNumber;
	const _tchar* pObjtag;
	_vec3 vObjScale;
	_float fAngle;
}BGINFO;