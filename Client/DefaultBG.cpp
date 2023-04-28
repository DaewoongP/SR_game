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

	// ����Ʈ ����
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Default_BG_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Default_BG_Texture", pComponent });
	return S_OK;
}

void CDefaultBG::Mouse_Move()
{
	// ���콺 ��ǥ�� z = 10.f ��ġ�� �����Ͽ� ��ŷ�ϴ¹��
	// ���콺 ��ǥ ���� �̰ǹ� �پ˰�
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	// ����Ʈ = ���콺 ��ǥ ���� ����Ʈ -> ���� -> �� -> ���� �������� �浹ó��? ��ŷó���ؾ���
	// �׷��� ����ķ� ��� ������
	// ����Ʈ -> ����
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	// ����Ʈ �������� �Լ�
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3	vMouse;
	// ���콺 ��ġ ���� -> �̰Ŵ� ���� ���� �����ϸ鼭 ����غ��� �̰��� �� �Ǵ��� �˼�����
	// directX ���� ������ǥ�� x -1~ 1, y -1~1, z 0 ~1 �̹Ƿ� �̷����� ������ ����
	vMouse.x = pt.x / (ViewPort.Width * 0.5f) - 1.f;
	vMouse.y = pt.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMouse.z = 0.f;

	//  ���� -> �� �����̽�
	// �������� ����� ����
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	// ���콺�� ����� ��������
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	// �� �����̽� -> ����
	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	// �佺���̽���ȯ�� ��ģ���¿����� ī�޶� ��ġ���� ��𿡿�
	// -> ī�޶���ġ���� ������ 0,0,0�̶�������? �׷��� �ʱ� RayPos ���� 0,0,0�ΰ���
	_vec3	vRayPos, vRayDir;

	vRayPos = { 0.f,0.f,0.f };
	// ���콺 ��ġ������ ���������ǰ������� Pos -> Mouse�� ���ϴ� 2���� ���Ͱ� ����.
	vRayDir = vMouse - vRayPos;
	// ������ ���� Dir���Ϳ� Pos ���͸� ������� ����ķ� �������� -> �ΰ��� ���Ͱ� ��->����� ��ȯ�� ����?
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// ���⼭ RayDir, RayPos�� ������� ��ġ�� ����.
	// z�� 1�� ����ȭ �ϴ� ������ z���� 10���� ���⶧ x,y�� ���� �������ֱ� ����
	// ex) _vec3(1, 0.7, -0.5) ��� ��ǥ���� z�� 10�� ������ ��ġ�� �����ַ���
	// ��ü�� -0.5 (z��ǥ�� ������) -> _vec3(1 / -0.5 , 0.7 / -0.5, 1) => z�� 1�� �ʱ�ȭ ��.
	// �̻��¿��� 10 - matView._43�� �����־� (���� matView�� ����� �����̹Ƿ� ī�޶��� ������ǥ��) -> ī�޶��� z��ǥ�� ����
	// ex) ī�޶��� z��ǥ�� -21�� ��� 10 - (-21) �� �ǹǷ� z�� 31�� ����°ſ� ����.

	vRayDir /= vRayDir.z; // z�� 1�� ����ȭ
	// �̰� vPos��� �ϱ��ߴµ� ����� ���̸����� ���⺤���� �����Ǻ��;ƴ�. (RayPos = ī�޶���ġ) ���� RayDir * (10 - ī�޶�z) �� ���ϴ� ����
	// �� vPos�� ǥ����.
	_vec3 vPos = vRayPos + vRayDir * (10 - matView._43);

	// �ε��� ĭ���߱�
	// ������ �����ǰ��� �ø��� ���� 0�ϰ�� Ȧ�������� ��ȯ���� ex) 2.1 �ϰ�� 3���� ��ȯ
	if ((_int)ceil(vPos.x) % 2 == 0)
		vPos.x += 1;

	if ((_int)ceil(vPos.y) % 2 == 0)
		vPos.y += 1;
	// ��ȯ�� x,y ��ǥ �־��ְ�
	// ������ ��ȯ�� z��ǥ �����Ͽ� z�� 10�� x,y��ǥ�� ��ȯ��.
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
