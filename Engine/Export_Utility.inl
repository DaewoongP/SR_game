#include "Export_Utility.h"
HRESULT			Create_Management(CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagement = pManagement;

	return S_OK;
}

CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CLayer*				Get_Layer(const _tchar* pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}

HRESULT			Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}

CScene*			Get_Scene(void)
{
	return CManagement::GetInstance()->Get_Scene();
}

CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Get_GameObject(pLayerTag, pObjTag);
}

_int			Update_Management(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Management(fTimeDelta);
}
void			LateUpdate_Management()
{
	CManagement::GetInstance()->LateUpdate_Management();
}
void			Render_Management(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Management(pGraphicDev);
}

HRESULT		Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}
CComponent*	Clone_Proto(const _tchar* pProtoTag, CGameObject* pGameObject)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag, pGameObject);
}

void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup(void)
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

void		Add_Collider(CCollider* pCollider)
{
	CCollisionMgr::GetInstance()->Add_Collider(pCollider);
}
void		Check_Collision(COLGROUP eGroup1, COLGROUP eGroup2)
{
	CCollisionMgr::GetInstance()->Check_Collision(eGroup1, eGroup2);
}
void		Clear_Collision()
{
	CCollisionMgr::GetInstance()->Clear_Collision();
}
void		Set_Collider(COLGROUP eGroup, CCollider* pCollider)
{
	CCollisionMgr::GetInstance()->Set_Collider(eGroup, pCollider);
}
void		Delete_Collider(CGameObject* pGameObject)
{
	CCollisionMgr::GetInstance()->Delete_Collider(pGameObject);
}

inline vector<RayCollision>  Check_Collision_Ray(RAYCAST ray, CCollider* shootObj, vector<_tchar*> tagname)
{
	return CCollisionMgr::GetInstance()->Check_Collision_Ray(ray,shootObj, tagname);
}

inline CBullet*		Reuse_Bullet(LPDIRECT3DDEVICE9& pGraphicDev, _vec3& vPos, BULLETTYPE eType, _vec3& vDir)
{
	return CBulletPool::GetInstance()->Reuse_Bullet(pGraphicDev, vPos, eType, vDir);
}
inline void			Release_Bullet(CBullet* pBullet)
{
	CBulletPool::GetInstance()->Release_Bullet(pBullet);
}
inline void			Reserve_Bullet(LPDIRECT3DDEVICE9& pGraphicDev, BULLETTYPE eType)
{
	CBulletPool::GetInstance()->Reserve_Bullet(pGraphicDev, eType);
}

void						Release_Utility(void)
{
	CBulletPool::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}