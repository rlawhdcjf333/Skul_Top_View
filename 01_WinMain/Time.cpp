#include "pch.h"
#include "Time.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

Time::Time()
{
	//���� �ϵ��� �����ϴ��� �˻��Ѵ�. 
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&this->mPeriodFrequency))
	{
		//���� �ϵ���� ���� true
		this->mIsHightHardware = true;
		//������ Ÿ�� �޾ƿ� 
		QueryPerformanceFrequency((LARGE_INTEGER*)&mLastTime);

		this->mTimeScale = 1.0f / this->mPeriodFrequency;
	}
	else
	{
		this->mIsHightHardware = false;
		this->mLastTime = timeGetTime();
		this->mTimeScale = 0.001f;
	}

	this->mFrameRate = this->mFPSFrameCount = 0;
	this->mFPSTimeElapsed = this->mWorldTime = 0.0f;
	this->mIsStart = true;
}


void Time::Tick(float lockFPS)
{
	if (mIsStart)
	{
		static bool isFirstChecking = false;
		//mWorldTime ���� ������ �߰�
		if (isFirstChecking == false)
		{
			this->mWorldTime = 0.0f;
			this->mFrameRate = this->mFPSFrameCount = 0;
			this->mFPSTimeElapsed = this->mWorldTime = 0.0f;
			isFirstChecking = true;
		}

		//���� �ϵ��� �����Ѵٸ� �и������� �̻��� ������ �޾ƿ´�.
		if (this->mIsHightHardware)
			QueryPerformanceCounter((LARGE_INTEGER*)&this->mCurrentTime);
		//�ƴ϶�� �и������� ������ ����Ÿ���� �޾ƿ´�.
		else
			this->mCurrentTime = timeGetTime();
		//deltaTimedms = (���� �ð� - ���� �����ӿ� üŷ�� �ð�) * mTimeScale;
		this->mDeltaTime = (this->mCurrentTime - this->mLastTime) * mTimeScale;
		//������ ���� ���� �Ǿ� �ִٸ�
		if (lockFPS > 0.0f)
		{
			//deltaTime�� ���� �����ӽð� ���� ���� ������ ����
			while (this->mDeltaTime < (1.0f / lockFPS))
			{
				//���� �ϵ���� �����ϸ� �и������� �̻��� ������ �޾ƿ´�. 
				if (this->mIsHightHardware)
					QueryPerformanceCounter((LARGE_INTEGER*)&this->mCurrentTime);
				//�ƴ϶�� ���� �ð��� �и������� ������ ���� �ð��޾ƿ´�. 
				else
					this->mCurrentTime = timeGetTime();
				//deltaTime�ٽ� ����
				this->mDeltaTime = (this->mCurrentTime - this->mLastTime) * this->mTimeScale;
			}
		}
		//������ ������ �ð� ����ð����� �ʱ�ȭ 
		this->mLastTime = this->mCurrentTime;
		//������ ����
		this->mFPSFrameCount++;
		//������ ��Ÿ Ÿ�� ���� 
		this->mFPSTimeElapsed += this->mDeltaTime;
		//���μ��� ���� �� �ð� ����
		this->mWorldTime += this->mDeltaTime;
		//�ʴ� ������ Ÿ���� 1�ʰ� �Ѿ��ٸ� ��ҵ� �ٽ� �ʱ�ȭ 
		if (this->mFPSTimeElapsed > 1.0f)
		{
			this->mFrameRate = this->mFPSFrameCount;
			this->mFPSFrameCount = 0;
			this->mFPSTimeElapsed = 0.0f;
		}
	}
	else
	{
		mDeltaTime = 0.f;
	}
}

void Time::StartClock()
{
	//���� �ϵ��� �����Ѵٸ� �и������� �̻��� ������ �޾ƿ´�.
	if (this->mIsHightHardware)
		QueryPerformanceCounter((LARGE_INTEGER*)&this->mCurrentTime);
	//�ƴ϶�� �и������� ������ ����Ÿ���� �޾ƿ´�.
	else
		this->mCurrentTime = timeGetTime();
	//deltaTimedms = (���� �ð� - ���� �����ӿ� üŷ�� �ð�) * mTimeScale;
	this->mDeltaTime = (this->mCurrentTime - this->mLastTime) * mTimeScale;
	//������ ���� ���� �Ǿ� �ִٸ�
	//if (LOCK_FPS > 0.0f)
	{
		//deltaTime�� ���� �����ӽð� ���� ���� ������ ����
		while (this->mDeltaTime < (1.0f / 60.f))
		{
			//���� �ϵ���� �����ϸ� �и������� �̻��� ������ �޾ƿ´�. 
			if (this->mIsHightHardware)
				QueryPerformanceCounter((LARGE_INTEGER*)&this->mCurrentTime);
			//�ƴ϶�� ���� �ð��� �и������� ������ ���� �ð��޾ƿ´�. 
			else
				this->mCurrentTime = timeGetTime();
			//deltaTime�ٽ� ����
			this->mDeltaTime = (this->mCurrentTime - this->mLastTime) * this->mTimeScale;
		}
	}
	//������ ������ �ð� ����ð����� �ʱ�ȭ 
	this->mLastTime = this->mCurrentTime;
	//������ ����
	this->mFPSFrameCount++;
	//������ ��Ÿ Ÿ�� ���� 
	this->mFPSTimeElapsed += this->mDeltaTime;
	//���μ��� ���� �� �ð� ����
	this->mWorldTime += this->mDeltaTime;
	//�ʴ� ������ Ÿ���� 1�ʰ� �Ѿ��ٸ� ��ҵ� �ٽ� �ʱ�ȭ 
	if (this->mFPSTimeElapsed > 1.0f)
	{
		this->mFrameRate = this->mFPSFrameCount;
		this->mFPSFrameCount = 0;
		this->mFPSTimeElapsed = 0.0f;
	}
	mDeltaTime = 0.f;
}

void Time::Update()
{
	this->Tick(60.f);
}