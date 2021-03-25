#pragma once

//std::pair가 요런 느낌으로 되어 있다.
//template<typename First,typename Second>
//struct Pair
//{
//	First first;
//	Second second;
//};

//프레임 돌릴 때
class Animation
{
	bool mIsPlay;	//플레이중이냐
	bool mIsLoop;	//루프냐

	int mCurrentFrameIndex;	//현재 프레임 인덱스 

	float mCurrentFrameTime;	//현재 프레임 감는 시간
	float mFrameUpdateTime;		//프레임 갱신(증가) 해야하는 시간
	
	vector<pair<int, int>> mFrameList;		//프레임 정보
	function<void(void)> mCallbackFunc;		//애니메이션이 전부 플레이되고 끝날 때 실행할 함수
public:
	Animation();
	Animation(int startX, int startY, int endX, int endY, bool isReverse, bool isLoop, float updateTime, function <void(void)> func = []() {});

	void Update();

	void Play();
	void Stop();
	void Pause();

	void InitFrameByVector(const vector<pair<int, int>>& frameList);
	void InitFrameByStartEnd(int startX, int startY, int endX, int endY, bool isReverse);

	void SetCallbackFunc(const function<void(void)>& func);
	inline void SetIsLoop(bool isLoop) { mIsLoop = isLoop; }
	inline void SetFrameUpdateTime(float updateTime) { mFrameUpdateTime = updateTime; }
	inline void SetCurrentFrameIndex(int index) { mCurrentFrameIndex = index; }

	inline float GetFrameUpdateTime()const { return mFrameUpdateTime; }
	inline int GetCurrentFrameIndex()const { return mCurrentFrameIndex; }
	inline float GetCurrentFrameTime()const { return mCurrentFrameTime; }
	inline int GetFrameSize()const { return mFrameList.size(); }
	inline bool GetIsLoop()const { return mIsLoop; }
	inline bool GetIsPlay()const { return mIsPlay; }
	inline pair<int, int> GetNowFrame()const { return mFrameList[mCurrentFrameIndex]; }
	inline int GetNowFrameX()const { return mFrameList[mCurrentFrameIndex].first; }
	inline int GetNowFrameY()const { return mFrameList[mCurrentFrameIndex].second; }

};

