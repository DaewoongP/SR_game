#include "stdafx.h"
#include "Grid.h"

#include "Export_Function.h"

CGrid::CGrid(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_bGridON(true)
{
}

CGrid::~CGrid()
{
}