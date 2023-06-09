#pragma once

#include "Management.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "CollisionMgr.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "Collider.h"
#include "Collision.h"
#include "BulletPool.h"

#include "GameObject.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "CubeTex.h"
#include "Rigidbody.h"
#include "Line.h"
#include "Animation.h"
#include "Shadow.h"
#include "BulletPool.h"
#include "RcGradation.h"
#include "RcAlpha.h"

#include "TexParticle.h"
#include "SparkParticle.h"
#include "CircularParticle.h"
#include "JumpParticle.h"
#include "LandingParticle.h"
#include "SlerpParticle.h"
#include "WindParticle.h"
#include "LeafParticle.h"
#include "RainParticle.h"
#include "CircleParticle.h"
#include "WaterParticle.h"
#include "BoxParticle.h"
#include "SkyParticle.h"
#include "SmokeParticle.h"
#include "WaveParticle.h"
#include "SuperLandingParticle.h"
#include "StarParticle.h"
#include "PortalParticle.h"
BEGIN(Engine)

inline HRESULT			Create_Management(CManagement** ppManagement);

inline CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline CLayer*			Get_Layer(const _tchar* pLayerTag);
inline HRESULT			Set_Scene(CScene* pScene);
inline CScene*			Get_Scene();
inline CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);

inline _int			Update_Management(const _float& fTimeDelta);
inline void			LateUpdate_Management();
inline void			Render_Management(LPDIRECT3DDEVICE9 pGraphicDev);

inline HRESULT		Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
inline CComponent*	Clone_Proto(const _tchar* pProtoTag, CGameObject* pGameObject);


inline void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void		Clear_RenderGroup(void);

inline void		Add_Collider(CCollider* pCollider);
inline void		Check_Collision(COLGROUP eGroup1, COLGROUP eGroup2);
inline void		Clear_Collision();
inline void		Set_Collider(COLGROUP eGroup, CCollider* pCollider);
inline void		Delete_Collider(CGameObject* pGameObject);
inline vector<RayCollision>		Check_Collision_Ray(RAYCAST ray, CCollider* shootObj, vector<_tchar*> tagname = {});

inline CBullet*		Reuse_Bullet(LPDIRECT3DDEVICE9& pGraphicDev, _vec3& vPos, BULLETTYPE eType, _vec3& vDir = _vec3(0,1,0));
inline void			Release_Bullet(CBullet* pBullet);
inline void			Reserve_Bullet(LPDIRECT3DDEVICE9& pGraphicDev, BULLETTYPE eType);

inline void						Release_Utility(void);


#include "Export_Utility.inl"

END