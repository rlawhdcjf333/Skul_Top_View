#include "pch.h"
#include "Animation.h"

Animation::Animation(int startX, int startY, int endX, int endY, bool isReverse, bool isLoop, float updateTime, function <void(void)> func)
	:mIsPlay(false), mIsLoop(false), mCurrentFrameIndex(0),
	mCurrentFrameTime(0.f), mFrameUpdateTime(0.f), mCallbackFunc(nullptr)
{
	InitFrameByStartEnd(startX, startY, endX, endY, isReverse);
	SetIsLoop(isLoop);
	SetFrameUpdateTime(updateTime);
	SetCallbackFunc(func);
}

Animation::Animation()
	:mIsPlay(false), mIsLoop(false), mCurrentFrameIndex(0),
	mCurrentFrameTime(0.f), mFrameUpdateTime(0.f), mCallbackFunc(nullptr) {}


void Animation::Update()
{
	if (mIsPlay == false)
		return;	

	mCurrentFrameTime += dTime;

	if (mCurrentFrameTime >= mFrameUpdateTime)
	{
		//�ս� �� �����ش�
		while (mCurrentFrameTime >= mFrameUpdateTime)
		{
			mCurrentFrameTime -=mFrameUpdateTime;
		}

		mCurrentFrameIndex++;
		//�������� ������ �����Ӻ��� Ŀ������ �Ѵٸ�
		if (mCurrentFrameIndex >= mFrameList.size())
		{
			if (mIsLoop == false)
			{
				mIsPlay = false;
				mCurrentFrameIndex = 0;
			}
			else
			{
				mCurrentFrameIndex = 0;
			}
			//�ݹ��Լ��� ���ε� �Ǿ� �ִٸ� �� �Լ� ����
			if (mCallbackFunc != nullptr)
			{
				mCallbackFunc();
			}
		}
	}
}

void Animation::Play()
{	
	mIsPlay = true;
}

void Animation::Stop()
{
	mIsPlay = false;
	mCurrentFrameIndex = 0;
	mCurrentFrameTime = 0.f;
}

void Animation::Pause()
{
	mIsPlay = false;
}

void Animation::InitFrameByVector(const vector<pair<int, int>>& frameList)
{
	mFrameList.clear();
	mFrameList = frameList;
}

void Animation::InitFrameByStartEnd(int startX, int startY, int endX, int endY, bool isReverse)
{
	if (!isReverse)
	{
		for (int y = startY; y <= endY; ++y)
		{
			for (int x = startX; x <= endX; ++x)
			{
				mFrameList.push_back(make_pair(x, y));
			}
		}
	}

	else if (isReverse)
	{
		for (int y = endY; y >= startY; --y)
		{
			for (int x = endX; x >= startX; --x)
			{
				mFrameList.push_back(make_pair(x, y));
			}
		}
	}
}

void Animation::SetCallbackFunc(const function<void(void)>& func)
{
	mCallbackFunc = func;
}
