#pragma once

#include "Typedef.h"

BEGIN(Engine)

typedef	 struct tagVertexColor
{
	_vec3			vPos;		// ��ġ
	_ulong			dwColor;	// ����

}VTXCOL;

const	_ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef	 struct tagVertexTexture
{
	_vec3			vPos;		// ��ġ
	_ulong			dwColor;	// ����
	_vec2			vTexUV;		// ����

}VTXTEX;

const	_ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

typedef	 struct tagVertexCube
{
	_vec3			vPos;		// ��ġ
	_vec3			vTexUV;		// ����

}VTXCUBE;

const	_ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


typedef VTXCOL PTCCOL;
typedef VTXTEX PTCTEX;

const	_ulong		FVF_PTC = D3DFVF_XYZ | D3DFVF_DIFFUSE |	D3DFVF_TEX1;

// D3DFVF_TEXCOORDSIZE3 : �ؽ�ó�� UV���� FLOAT�� 3���� ũ�� ��ŭ�̸� ��ȣ �� ������ �ǹ̴� ���� ���ؽ��� UV ���� ������ ���� �� �մµ� �� �� ù ��° ���� ����ϰڴٴ� �ǹ�

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

	// ������ ��ġ�� �޾ƿͼ� offsetMin �̶� offsetMax��ŭ ������ �� ������ ���ӿ�����Ʈ�� ���������� ������ �޶����� �ϱ⶧��.
	void Offset(const _vec3& origin)
	{
		_min = origin + _offsetMin + _offPos;
		_max = origin + _offsetMax + _offPos;
	}

	// origin + ť���� ũ�� (ť�� ���� ��ǥ)
	_vec3 _min;
	_vec3 _max;

	// ť���� ũ�� (ť�� ���� ��ǥ)
	_vec3 _offsetMin;
	_vec3 _offsetMax;

	// ���� ��ü �������� �ݶ��̴� ���������� ������
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

// ��ġ���� ������ �����ϱ� ���� ����ü
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