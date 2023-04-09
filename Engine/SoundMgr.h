#pragma once
#include "Base.h"
#include"Include.h"

BEGIN(Engine)

class CSoundMgr :  public CBase
{
    DECLARE_SINGLETON(CSoundMgr)
private:
    explicit CSoundMgr();
    virtual ~CSoundMgr();

public:
    void Ready_Sound();
	void Ready_Sound_Effect();
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:

	 //사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;
	FMOD_SYSTEM* m_pSystem_Effect;


private:
	void LoadSoundFile();
	void LoadSoundFile_Effect();

public:
    virtual void Free();
};

END