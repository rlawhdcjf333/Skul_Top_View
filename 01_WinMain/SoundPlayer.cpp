#include "pch.h"
#include "SoundPlayer.h"

using namespace FMOD;

SoundPlayer::SoundPlayer()
{
	//대부분의 라이브러리는 함부로 내부 요소를 생성,삭제 하지 못하게 되어있다. 
	//그래서 팩토리패턴이라는 디자인 패턴을 사용해서 생성하게 끔 유도함
	FMOD::System_Create(&mSystem);
	mSystem->init(MaxChannelBuffer, FMOD_INIT_NORMAL, NULL);
}

SoundPlayer::~SoundPlayer()
{
	for (int i = 0; i < mActiveChannels.size(); ++i)
	{
		mActiveChannels[i].Channel->stop();
	}

	for (SoundIter iter = mSoundList.begin(); iter != mSoundList.end(); ++iter)
	{
		//라이브러리의 클래스들은 대부분 생성자 소멸자 모두 private되어 있음
		//외부에서 삭제하려면 release함수를 호출하게끔 짜여져 있다.
		iter->second->release();
	}

	mSystem->release();
}

void SoundPlayer::Update()
{
	mSystem->update();

	bool isPaused;
	bool isPlaying;
	for (int i = 0; i < mActiveChannels.size(); ++i)
	{
		mActiveChannels[i].Channel->getPaused(&isPaused);
		if (isPaused)return;
		mActiveChannels[i].Channel->isPlaying(&isPlaying);
		if (isPlaying == false)
		{
			mActiveChannels[i].Channel->stop();
			mActiveChannels.erase(mActiveChannels.begin() + i);
			--i;
		}
	}
}

void SoundPlayer::LoadFromFile(const wstring & keyName, const wstring & fileName, bool isLoop)
{
	// {{ 이미 불러온적이 있는 파일인지 검사
	SoundIter iter = mSoundList.find(keyName);
	if (iter != mSoundList.end())
		return;
	// }}

	string strFileName;
	strFileName.assign(fileName.begin(), fileName.end());

	Sound* sound = nullptr;

	if (isLoop)
	{
		mSystem->createStream(strFileName.c_str(), FMOD_LOOP_NORMAL, NULL, &sound);
	}
	else
	{
		mSystem->createSound(strFileName.c_str(), FMOD_DEFAULT, NULL, &sound);
	}

	if (sound != nullptr)
	{
		mSoundList.insert(make_pair(keyName, sound));
	}
}

void SoundPlayer::Play(const wstring & keyName, float volume)
{

	//일시정지 되어 있는 사운드 인지 판단
	for (int i = 0; i < mActiveChannels.size(); ++i)
	{
		if (mActiveChannels[i].SoundName == keyName)
		{
			bool isPaused; 
			mActiveChannels[i].Channel->getPaused(&isPaused);
			if (isPaused)
			{
				mActiveChannels[i].Channel->setPaused(false);
				return;
			}
		}
	}

	if (mActiveChannels.size() >= MaxChannelBuffer)
		return;

	//해당 이름의 사운드가 없다면 return 
	SoundIter iter = mSoundList.find(keyName);
	if (iter == mSoundList.end())
		return;

	Channel* channel;
	Sound* sound = iter->second;

	mSystem->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
	channel->setVolume(volume);

	ChannelInfo channelInfo;
	channelInfo.Channel = channel;
	channelInfo.SoundName = keyName;
	mActiveChannels.push_back(channelInfo);
}

void SoundPlayer::Pause(const wstring & keyName)
{
	for (int i = 0; i < mActiveChannels.size(); ++i)
	{
		if (mActiveChannels[i].SoundName == keyName)
		{
			mActiveChannels[i].Channel->setPaused(true);
			break;
		}
	}
}

void SoundPlayer::Stop(const wstring & keyName)
{
	for (int i = 0; i < mActiveChannels.size(); ++i)
	{
		if (mActiveChannels[i].SoundName == keyName)
		{
			mActiveChannels[i].Channel->stop();
			mActiveChannels.erase(mActiveChannels.begin() + i);
			break;
		}
	}
}
