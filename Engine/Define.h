#pragma once

#define		WINCX		1366
#define		WINCY		768

#define		VTXCNTX		120
#define		VTXCNTZ		120
#define		VTXITV		1

#define		CUBEX		32
#define		CUBEY		18
// 큐브의 가장 낮은 y값
#define		LANDY		2

#define		MAX_STR		256

#define			OBJ_NOEVENT		0
#define			OBJ_DEAD		1
#define			STAGE_END		2
#define			STAGE_FAILED	3
#define			STAGE_FADEIN	4
#define			STAGE_FADEOUT	5

#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;
//

#ifdef	ENGINE_EXPORTS
#define 		_declspec(dllexport)
#else
#define 		_declspec(dllimport)
#endif

#define NULL_CHECK( _ptr)	\
	{if( _ptr == 0){__asm { int 3 };return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( _ptr == 0){__asm { int 3 };return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}


#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return _return;}



#define NO_COPY(CLASSNAME)							\
		private:											\
		CLASSNAME(const CLASSNAME&) = delete;					\
		CLASSNAME& operator = (const CLASSNAME&) = delete;		

#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(NULL == m_pInstance) {					\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ) {		\
			if(NULL != m_pInstance)	{					\
				delete m_pInstance;						\
				m_pInstance = NULL;						\
			}											\
		}

#define TOODEEDIE CToodee* toodee = dynamic_cast<CToodee*>(collision->otherObj);\
if (toodee != nullptr&&g_Is2D&&!g_IsInvin)\
toodee->Set_AnimDead();

#define TOOKEEDIE CTookee* tookee = dynamic_cast<CTookee*>(collision->otherObj);\
if (tookee != nullptr&&!g_IsInvin)\
tookee->Set_Dead();

#define TOPDEEDIE CTopdee* topdee = dynamic_cast<CTopdee*>(collision->otherObj);\
if (topdee != nullptr&&!g_Is2D&&!g_IsInvin)\
topdee->SetDie();

#define THIRDDEEDIE CThirddee* thirddee = dynamic_cast<CThirddee*>(collision->otherObj);\
if (thirddee != nullptr&&!g_IsInvin)\
thirddee->Set_Die();

#define TOODEEDIE_RAY if (_detectedCOL.size() >= 1&&!lstrcmp(_detectedCOL[0].tag, L"Toodee")&&g_Is2D&&!g_IsInvin)\
	dynamic_cast<CToodee*>(_detectedCOL[0].col->m_pGameObject)->Set_AnimDead();

#define TOOKEEDIE_RAY if (_detectedCOL.size() >= 1&&!lstrcmp(_detectedCOL[0].tag, L"Tookee")&&!g_IsInvin)\
	dynamic_cast<CTookee*>(_detectedCOL[0].col->m_pGameObject)->Set_Dead();

#define TOPDEEDIE_RAY if (_detectedCOL.size() >= 1&&!lstrcmp(_detectedCOL[0].tag, L"Topdee")&&!g_Is2D&&!g_IsInvin)\
	dynamic_cast<CTopdee*>(_detectedCOL[0].col->m_pGameObject)->SetDie();

#define THIRDDEEDIE_RAY if (_detectedCOL.size() >= 1&&!lstrcmp(_detectedCOL[0].tag, L"Thirddee")&&!g_IsInvin)\
	dynamic_cast<CThirddee*>(_detectedCOL[0].col->m_pGameObject)->Set_Die();
/*
#include "Tookee.h"
#include "Toodee.h"
#include "Topdee.h"
#include "Thirddee.h"

TOOKEEDIE;
TOODEEDIE;
TOPDEEDIE;
THIRDDEEDIE;

TOODEEDIE_RAY
TOOKEEDIE_RAY
TOPDEEDIE_RAY
THIRDDEEDIE_RAY
*/