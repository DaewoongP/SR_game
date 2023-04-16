#pragma once
#include "Include.h"
#include "GameObject.h"

//						ë°˜í™˜?€?? ë³´ìŠ¤?ˆì˜ ?¨ìˆ˜ ?¬ìš©?˜ê¸° ?„í•¨, ë§¤ê°œë³€??
#define BOSS2_STATE_FUNC vector<void(CBoss2::*)(const _float& fTimeDelta)>

BEGIN(Engine)
class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
class CAnimation;
END

enum BOSS2STATE
{
	B2_IDLE, //? ë‹ˆë©”ì´??ë¦¬ê¹…/joint
	B2_JUMPING,
	B2_SCREAM,
	B2_PUNCH,
	B2_STUMP,
	B2_END
};

//ë³´ìŠ¤??ë¨¸ë¦¬?µì„. ëª¨ë“  ?ê°/?‰ë™??ì¤‘ì¶”ë¥??´ë‹¹??
//ë³´ìŠ¤??ì¤‘ë ¥???°ì? ?Šì„ê±°ì„. ë³´ë‹ˆê¹?ì¤‘ë ¥ ?ˆì?.
class CBoss2 :
	public CGameObject
{
private:
	explicit CBoss2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

private:
	HRESULT		Add_Component(void);
	HRESULT		Find_PlayerBoth();
	void		CheckZFloor();
	//?í”„ ?¨ìˆ˜ ê¸°ë³¸ê°??¤ì •
	void		Do_Jump_Ready(const _float& fTimeDelta);
	//?„ë¡œ ?ìŠ¹?œë‹¤
	void		Do_Jump_01(const _float& fTimeDelta);
	//ì°©ì??œë‹¤.
	void		Do_Jump_02(const _float& fTimeDelta);
	//?´ì‹?¨í„´
	void		Do_Rest(const _float& fTimeDelta);
	//ë²¨ë¡œ?œí‹° ì´ˆê¸°???¨í„´
	void		Do_ResetVelocity(const _float& fTimeDelta) { m_pRigid->m_Velocity = _vec3(0, 0, 0); m_pRigid->m_AngularVelocity = _vec3(0, 0, 0); CheckIsLastActionIdx(); }

	void		Do_Idle(const _float& fTimeDelta) { m_pAnimation_Body->SetAnimation(L"Idle"); m_dwRestTime = 2.0f; CheckIsLastActionIdx();}
	
	//?¤í????¨ìˆ˜ ê¸°ë³¸ê°??¤ì •
	void		Do_Stump_Ready(const _float& fTimeDelta);
	//toodee ?¹ì? topdeeë¥??¥í•´ nì´ˆê°„ ê°€?ˆëŠ” ?¨í„´
	void		Do_Chase_Player(const _float& fTimeDelta);
	//yë¡??´ì§ ?¬ë¼ê°€ë©? ?Œì „??ì£¼ëŠ” ?¨í„´
	void		Do_LittleUp_Turn(const _float& fTimeDelta);
	//ì°©ì??œë‹¤.
	void		Do_Stump_02(const _float& fTimeDelta);
	//?Œì „ ê°ì†Œ ?¨í„´
	void		Do_Turn_Minus(const _float& fTimeDelta);
	//ë§Œì„¸!
	void        Do_Hurray(const _float& fTimeDelta);
	//ì£¼ë¨¹ ?Œí™˜
	void        Do_SummonFist(const _float& fTimeDelta);
	void        Do_Scream(const _float& fTimeDelta);
	void		Do_ScreamEnd(const _float& fTimeDelta);

	void		DoFlip();
	void		SetPartten();
	void		ReadyPartten();
	void		CheckIsLastActionIdx();

	//´øÁö±â
	void		Do_Standing(const _float& fTimeDelta);
	//½¬±â
	//¹ÙÀ§ ¼ÒÈ¯ 
	void		Do_SummonRock(const _float& fTimeDelta);
	//½¬±â
	//¼Õ¸¸ ³»¸®±â
	void		Do_Throw(const _float& fTimeDelta);
	//´Ù½Ã ÀçÀ§Ä¡
	void		Do_ThrowEnd(const _float& fTimeDelta);
private:	
	BOSS2STATE m_eCurrentState;
	BOSS2STATE m_ePreState;
	_int		m_iCurrentActionIdx;
	_float		m_dwRestTime;
	_float		m_dwActionTime;
	_bool		m_bFlip_Y;

	//state, ?¨ìˆ˜?¬ì¸??vecë¥?ê°€ì§€??vector
	vector<BOSS2_STATE_FUNC> funcAction;

	//?í”„???´ë™??x ê°?
	_vec3	   m_fJumpPos[3];
	_int	   m_iJumpPosidx;

	//ì°ê¸°???¬ìš©??player???„ì¹˜ë¥?ë°›ì•„?¤ê¸° ?„í•¨.
	CTransform* m_pPlayer01_trans;
	CTransform* m_pPlayer02_trans;

	_bool		m_bIsOnGround;
	_bool		m_bInit;

private:
	Engine::CCollider*		m_pCollider;
	Engine::CRigidbody*		m_pRigid;
	Engine::CAnimation*		m_pAnimation_Face;
	Engine::CAnimation*		m_pAnimation_Body;
public:
	static CBoss2*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

