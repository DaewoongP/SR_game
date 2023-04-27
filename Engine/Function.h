#pragma once

#include "Typedef.h"

template <typename T>			// 클래스 포인터들을 해제
DWORD Safe_Release(T& pointer)
{
	DWORD	dwRefCnt = 0;

	if (NULL != pointer)
	{
		dwRefCnt = pointer->Release();

		if (dwRefCnt == 0)
			pointer = NULL;
	}
	return dwRefCnt;
}

template <typename T>
void Safe_Single_Destory(T& pointer)
{
	if (NULL != pointer)
	{
		pointer->DestroyInstance();
		pointer = NULL;
	}
}

template <typename T>			// 원시 자료형, 구조체 포인터 해제
void Safe_Delete(T& pointer)
{
	if (NULL != pointer)
	{
		delete pointer;
		pointer = NULL;
	}
}

template <typename T>			// 동적 배열을 삭제하는 용도
void Safe_Delete_Array(T& pointer)
{
	if (NULL != pointer)
	{
		delete[] pointer;
		pointer = NULL;
	}
}

/* Float 에서 DWORD로 변환 */
static DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

/* lowBound 에서 highBound로 변환 */
static float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

/* min 벡터와 max 벡터사이의 랜덤 벡터 구해줌 */
static void GetRandomVector(
	D3DXVECTOR3* out,
	D3DXVECTOR3* min,
	D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}
/* 특정 반지름을 가진 원의 벡터를 랜덤 반환 (2차원 xy 공간 기준임.)*/
static void GetRandomVectorIncircle(D3DXVECTOR3* out, _float radius)
{
	_float randNum = (rand() % 10000) * 0.0001;
	*out = D3DXVECTOR3(randNum, 1 - randNum, 0.f);
	D3DXVec3Normalize(out, out);
	*out = *out * radius;
	switch (rand() % 4)
	{
	case 0:
		// 1사분면
		break;
	case 1:
		// 2사분면
		out->x *= -1;
		break;
	case 2:
		// 3사분면
		out->x *= -1;
		out->y *= -1;
		break;
	case 3:
		// 4사분면
		out->y *= -1;
		break;
	}
}
/* 구면선형보간 - output, 시작점, 끝점, 점간의 업벡터, 선형보간 중점과 원의 중점간 거리 k, 원하는 보간 값 s*/
static void GetVectorSlerp(D3DXVECTOR3* out, D3DXVECTOR3* v1, D3DXVECTOR3* v2, D3DXVECTOR3* vUp, _float k, _float s)
{
	D3DXVECTOR3 v3, OV1, OV2, vCenter, vNormal1, vNormal2;
	_float fRad;
	D3DXMATRIX	matRot;
	D3DXMatrixIdentity(&matRot);

	D3DXVec3Lerp(&v3, v1, v2, 0.5f);
	D3DXVec3Cross(&vCenter, &(*v1 - v3), vUp);
	D3DXVec3Normalize(&vCenter, &vCenter);
	vCenter *= k;
	vCenter += v3;

	OV1 = *v1 - vCenter;
	OV2 = *v2 - vCenter;
	
	D3DXVec3Normalize(&vNormal1, &OV1);
	D3DXVec3Normalize(&vNormal2, &OV2);

	fRad = acosf(D3DXVec3Dot(&vNormal1, &vNormal2));
	fRad *= s;

	D3DXMatrixRotationAxis(&matRot, vUp, fRad);
	D3DXVec3TransformCoord(out, &OV1, &matRot);
	*out += vCenter;
}
//////////////////////////////////////////////////////////////////
/////////////////////////////////Functor 함수객체//////////////////////////

class CTag_Finder
{
public:
	explicit CTag_Finder(const _tchar* pTag)
		: m_pTargetTag(pTag)
	{
	}
	~CTag_Finder() {		}
public:
	template<typename T>
	bool operator()(const T& pair)
	{
		if (0 == lstrcmpW(m_pTargetTag, pair.first))
		{
			return true;
		}

		return false;
	}

private:
	const _tchar*		m_pTargetTag = nullptr;
};

class CDeleteObj
{
public:
	explicit CDeleteObj(void) {}
	~CDeleteObj(void) {}
public: // operator
	template <typename T>
	void operator () (T& pInstance)
	{
		_ulong dwRefCnt = 0;

		dwRefCnt = pInstance->Release();

		if (0 == dwRefCnt)
			pInstance = nullptr;
	}
};

// 연관컨테이너 삭제용
class CDeleteMap
{
public:
	explicit CDeleteMap(void) {}
	~CDeleteMap(void) {}
public: // operator	
	template <typename T>
	void operator () (T& Pair)
	{
		_ulong dwRefCnt = 0;

		dwRefCnt = Pair.second->Release();

		if (0 == dwRefCnt)
			Pair.second = NULL;
	}
};

//릴리즈 없는 맵
class CDeleteMap_MY
{
public:
	explicit CDeleteMap_MY(void) {}
	~CDeleteMap_MY(void) {}
public: // operator	
	template <typename T>
	void operator () (T& Pair)
	{
		delete(Pair.second);
	}
};

//A부터 B까지
static float Lerp(float A, float B, float Alpha)
{
	return A * (1 - Alpha) + B * Alpha;
}

static float InvLerp(float a, float b, float v) {
	return (v - a) / (b - a);
}

static _vec3 Lerp(_vec3 A, _vec3 B, float Alpha)
{
	return _vec3(Lerp(A.x, B.x, Alpha), Lerp(A.y, B.y, Alpha), Lerp(A.z, B.z, Alpha));
}

static _float _short_angle_dist(_float from, _float to) {
	_float max_angle = D3DX_PI * 2;
	_float difference = fmod(to - from, max_angle);
	return fmod(2 * difference, max_angle) - difference;
}

static _float lerp_angle(_float from, _float to, _float weight)
{
	return from + _short_angle_dist(from, to) * weight;
}


	

template <typename T>
static void Value_Range(T Min, T Max, T* Value)
{
	NULL_CHECK(Value);
	if (Max < *Value)
	{
		*Value = Max;
	}
	else if (Min > *Value)
	{
		*Value = Min;
	}
}