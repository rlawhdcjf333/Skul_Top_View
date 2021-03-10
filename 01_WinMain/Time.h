#pragma once
class Time
{
	Singleton(Time)
private:
	bool mIsHightHardware;			//고성능 하드웨어를 지원하느냐
	float mTimeScale;				//타이머의 스케일()
	float mDeltaTime;				//tick 카운트

	__int64 mCurrentTime;				//현재 시간
	__int64 mLastTime;				//이전 프레임 시간
	__int64 mPeriodFrequency;		//고성능 하드웨어 검사용 변수

	unsigned long mFrameRate;
	unsigned long mFPSFrameCount;
	float mFPSTimeElapsed;
	float mWorldTime;

	bool mIsStart;
private:
	Time();

	void Tick(float lockFPS = 60.0f);
	void StartClock();
public:
	void Update();

	inline void Start() { mIsStart = true; StartClock(); }
	inline void Stop() { mIsStart = false; }
	inline unsigned long GetmFrameRate() const { return mFrameRate; }
	inline float DeltaTime()const { return this->mDeltaTime; }
	inline float GetWorldTime()const { return this->mWorldTime; }
	inline float GetTimeScale()const { return this->mTimeScale; }
	inline void SetTimeScale(float scale) { this->mTimeScale = scale; }
};

