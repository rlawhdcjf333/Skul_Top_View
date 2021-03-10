#include "pch.h"
#include "SoundPlayer.h"

using namespace FMOD;

SoundPlayer::SoundPlayer()
{
	//��κ��� ���̺귯���� �Ժη� ���� ��Ҹ� ����,���� ���� ���ϰ� �Ǿ��ִ�. 
	//�׷��� ���丮�����̶�� ������ ������ ����ؼ� �����ϰ� �� ������
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
		//���̺귯���� Ŭ�������� ��κ� ������ �Ҹ��� ��� private�Ǿ� ����
		//�ܺο��� �����Ϸ��� release�Լ��� ȣ���ϰԲ� ¥���� �ִ�.
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
	// {{ �̹� �ҷ������� �ִ� �������� �˻�
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

	//�Ͻ����� �Ǿ� �ִ� ���� ���� �Ǵ�
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

	//�ش� �̸��� ���尡 ���ٸ� return 
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
