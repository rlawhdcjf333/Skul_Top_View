#pragma once
class Time
{
	Singleton(Time)
private:
	bool mIsHightHardware;			//���� �ϵ��� �����ϴ���
	float mTimeScale;				//Ÿ�̸��� ������()
	float mDeltaTime;				//tick ī��Ʈ

	__int64 mCurrentTime;				//���� �ð�
	__int64 mLastTime;				//���� ������ �ð�
	__int64 mPeriodFrequency;		//���� �ϵ���� �˻�� ����

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

