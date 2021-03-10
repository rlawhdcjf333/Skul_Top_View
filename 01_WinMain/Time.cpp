#include "pch.h"
#include "Time.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

Time::Time()
{
	//고성능 하드웨어를 지원하는지 검사한다. 
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&this->mPeriodFrequency))
	{
		//고성능 하드웨어 여부 true
		this->mIsHightHardware = true;
		//마지막 타임 받아옴 
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
		//mWorldTime 버그 때문에 추가
		if (isFirstChecking == false)
		{
			this->mWorldTime = 0.0f;
			this->mFrameRate = this->mFPSFrameCount = 0;
			this->mFPSTimeElapsed = this->mWorldTime = 0.0f;
			isFirstChecking = true;
		}

		//고성능 하드웨어를 지원한다면 밀리세컨드 이상의 단위로 받아온다.
		if (this->mIsHightHardware)
			QueryPerformanceCounter((LARGE_INTEGER*)&this->mCurrentTime);
		//아니라면 밀리세컨드 단위로 현제타임을 받아온다.
		else
			this->mCurrentTime = timeGetTime();
		//deltaTimedms = (현재 시간 - 지난 프레임에 체킹한 시간) * mTimeScale;
		this->mDeltaTime = (this->mCurrentTime - this->mLastTime) * mTimeScale;
		//프레임 락이 설정 되어 있다면
		if (lockFPS > 0.0f)
		{
			//deltaTime이 제한 프레임시간 보다 작을 때까지 루프
			while (this->mDeltaTime < (1.0f / lockFPS))
			{
				//고성능 하드웨어 지원하면 밀리세컨드 이상의 단위로 받아온다. 
				if (this->mIsHightHardware)
					QueryPerformanceCounter((LARGE_INTEGER*)&this->mCurrentTime);
				//아니라면 현재 시간은 밀리세컨드 단위로 현재 시간받아온다. 
				else
					this->mCurrentTime = timeGetTime();
				//deltaTime다시 설정
				this->mDeltaTime = (this->mCurrentTime - this->mLastTime) * this->mTimeScale;
			}
		}
		//마지막 프레임 시간 현재시간으로 초기화 
		this->mLastTime = this->mCurrentTime;
		//프레임 증가
		this->mFPSFrameCount++;
		//프레임 델타 타임 증가 
		this->mFPSTimeElapsed += this->mDeltaTime;
		//프로세스 실행 후 시간 증감
		this->mWorldTime += this->mDeltaTime;
		//초당 프레임 타임이 1초가 넘었다면 요소들 다시 초기화 
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
	//고성능 하드웨어를 지원한다면 밀리세컨드 이상의 단위로 받아온다.
	if (this->mIsHightHardware)
		QueryPerformanceCounter((LARGE_INTEGER*)&this->mCurrentTime);
	//아니라면 밀리세컨드 단위로 현제타임을 받아온다.
	else
		this->mCurrentTime = timeGetTime();
	//deltaTimedms = (현재 시간 - 지난 프레임에 체킹한 시간) * mTimeScale;
	this->mDeltaTime = (this->mCurrentTime - this->mLastTime) * mTimeScale;
	//프레임 락이 설정 되어 있다면
	//if (LOCK_FPS > 0.0f)
	{
		//deltaTime이 제한 프레임시간 보다 작을 때까지 루프
		while (this->mDeltaTime < (1.0f / 60.f))
		{
			//고성능 하드웨어 지원하면 밀리세컨드 이상의 단위로 받아온다. 
			if (this->mIsHightHardware)
				QueryPerformanceCounter((LARGE_INTEGER*)&this->mCurrentTime);
			//아니라면 현재 시간은 밀리세컨드 단위로 현재 시간받아온다. 
			else
				this->mCurrentTime = timeGetTime();
			//deltaTime다시 설정
			this->mDeltaTime = (this->mCurrentTime - this->mLastTime) * this->mTimeScale;
		}
	}
	//마지막 프레임 시간 현재시간으로 초기화 
	this->mLastTime = this->mCurrentTime;
	//프레임 증가
	this->mFPSFrameCount++;
	//프레임 델타 타임 증가 
	this->mFPSTimeElapsed += this->mDeltaTime;
	//프로세스 실행 후 시간 증감
	this->mWorldTime += this->mDeltaTime;
	//초당 프레임 타임이 1초가 넘었다면 요소들 다시 초기화 
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