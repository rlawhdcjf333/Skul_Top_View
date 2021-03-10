#pragma once
/*
FMOD 라이브라리!!!!!
System : FMOD관련된 장치들을 생성하거나, 사운드를 재생하는 등, 전반적인 시스템 처리를 전부 담당
Sound : 사운드 파일
Channel : 사운드 재생해주는 녀석
*/

#define MaxChannelBuffer 50	//최대 채널 수

class SoundPlayer final
{
	Singleton(SoundPlayer)
private:
	struct ChannelInfo
	{
		wstring SoundName;
		FMOD::Channel* Channel;
	};
	typedef map<wstring, FMOD::Sound*>::iterator SoundIter;
private:
	map<wstring, FMOD::Sound*> mSoundList;
	vector<ChannelInfo> mActiveChannels;		//현재 활성화 되어 있는 채널
	FMOD::System* mSystem;
private:
	SoundPlayer();
	~SoundPlayer();
public:
	void Update();
	void LoadFromFile(const wstring& keyName, const wstring& fileName, bool isLoop);
	void Play(const wstring& keyName, float volume);
	void Pause(const wstring& keyName);
	void Stop(const wstring& keyName);

	//과제~ MP3플레이어 만들어오기
	//void SetVolume(const wstring& keyName, float volume);
	//float GetVolume(const wstring& keyName);  
	//void SetPosition(const wstring& keyName, float time);
	//float GetPosition(const wstring& keyName);
};

