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

	 //���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;
	FMOD_SYSTEM* m_pSystem_Effect;


private:
	void LoadSoundFile();
	void LoadSoundFile_Effect();

public:
    virtual void Free();
};

END