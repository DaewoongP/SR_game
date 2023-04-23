#include "stdafx.h"
#include "DefaultBG.h"
CDefaultBG::CDefaultBG(PDIRECT3DDEVICE9 pGraphicdev):CBackGroundBase(pGraphicdev)
{
}

CDefaultBG::~CDefaultBG()
{
}

HRESULT CDefaultBG::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	return S_OK;
}

_int CDefaultBG::Update_GameObject(const _float& fTimeDelta)
{
	Mouse_Move();

	CGameObject::Update_GameObject(fTimeDelta);


	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CDefaultBG::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();

}

void CDefaultBG::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

HRESULT CDefaultBG::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	// 디폴트 몬스터
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Default_BG_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Default_BG_Texture", pComponent });
	return S_OK;
}

void CDefaultBG::Mouse_Move()
{
	// 마우스 좌표를 z = 10.f 위치로 고정하여 피킹하는방법
	// 마우스 좌표 얻어옴 이건뭐 다알고
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	// 뷰포트 = 마우스 좌표 따라서 뷰포트 -> 투영 -> 뷰 -> 월드 까지가서 충돌처리? 피킹처리해야함
	// 그래서 역행렬로 계속 곱해줌
	// 뷰포트 -> 투영
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	// 뷰포트 가져오는 함수
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3	vMouse;
	// 마우스 위치 보정 -> 이거는 직접 숫자 대입하면서 계산해보면 이값이 왜 되는지 알수있음
	// directX 에서 투영좌표는 x -1~ 1, y -1~1, z 0 ~1 이므로 이런식의 수식이 나옴
	vMouse.x = pt.x / (ViewPort.Width * 0.5f) - 1.f;
	vMouse.y = pt.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMouse.z = 0.f;

	//  투영 -> 뷰 스페이스
	// 프로젝션 역행렬 연산
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	// 마우스에 역행렬 연산해줌
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	// 뷰 스페이스 -> 월드
	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	// 뷰스페이스변환을 마친상태에서의 카메라 위치값이 어디에요
	// -> 카메라위치값이 무조건 0,0,0이란말이죠? 그래서 초기 RayPos 값이 0,0,0인거임
	_vec3	vRayPos, vRayDir;

	vRayPos = { 0.f,0.f,0.f };
	// 마우스 위치값에서 레이포지션값을빼면 Pos -> Mouse로 향하는 2차원 벡터가 나옴.
	vRayDir = vMouse - vRayPos;
	// 위에서 나온 Dir벡터와 Pos 벡터를 뷰행렬의 역행렬로 연산해줌 -> 두개의 벡터가 뷰->월드로 변환됨 ㅇㅋ?
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 여기서 RayDir, RayPos는 월드상의 위치를 뜻함.
	// z를 1로 정규화 하는 이유는 z값을 10으로 맞출때 x,y도 같이 보정해주기 위함
	// ex) _vec3(1, 0.7, -0.5) 라는 좌표에서 z가 10인 지점의 위치를 구해주려면
	// 전체에 -0.5 (z좌표로 나눠줌) -> _vec3(1 / -0.5 , 0.7 / -0.5, 1) => z가 1로 초기화 됨.
	// 이상태에서 10 - matView._43을 곱해주어 (지금 matView는 역행렬 상태이므로 카메라의 월드좌표임) -> 카메라의 z좌표를 빼줌
	// ex) 카메라의 z좌표가 -21일 경우 10 - (-21) 이 되므로 z를 31로 만드는거와 같다.

	vRayDir /= vRayDir.z; // z를 1로 정규화
	// 이거 vPos라고 하긴했는데 방향과 길이를가진 방향벡터임 포지션벡터아님. (RayPos = 카메라위치) 에서 RayDir * (10 - 카메라z) 로 향하는 벡터
	// 를 vPos로 표현함.
	_vec3 vPos = vRayPos + vRayDir * (10 - matView._43);

	// 인덱스 칸맞추기
	// 강제로 포지션값을 올림을 통해 0일경우 홀수값으로 변환해줌 ex) 2.1 일경우 3으로 변환
	if ((_int)ceil(vPos.x) % 2 == 0)
		vPos.x += 1;

	if ((_int)ceil(vPos.y) % 2 == 0)
		vPos.y += 1;
	// 변환한 x,y 좌표 넣어주고
	// 위에서 변환한 z좌표 대입하여 z가 10인 x,y좌표로 변환됨.
	m_pTransform->m_vInfo[INFO_POS].x = (_int)vPos.x;
	m_pTransform->m_vInfo[INFO_POS].y = (_int)vPos.y;
	m_pTransform->m_vInfo[INFO_POS].z = 11.1f;
}

CDefaultBG* CDefaultBG::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CDefaultBG* pInstance = new CDefaultBG(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
 
	return pInstance;
}

void CDefaultBG::Free(void)
{
	__super::Free();
}
