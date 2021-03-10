#pragma once

//std::pair�� �䷱ �������� �Ǿ� �ִ�.
//template<typename First,typename Second>
//struct Pair
//{
//	First first;
//	Second second;
//};

//������ ���� ��
class Animation
{
	bool mIsPlay;	//�÷������̳�
	bool mIsLoop;	//������

	int mCurrentFrameIndex;	//���� ������ �ε��� 

	float mCurrentFrameTime;	//���� ������ ���� �ð�
	float mFrameUpdateTime;		//������ ����(����) �ؾ��ϴ� �ð�
	
	vector<pair<int, int>> mFrameList;		//������ ����
	function<void(void)> mCallbackFunc;		//�ִϸ��̼��� ���� �÷��̵ǰ� ���� �� ������ �Լ�
public:
	Animation();

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
	inline float GetCurrentFrameTIme()const { return mCurrentFrameTime; }
	inline bool GetIsLoop()const { return mIsLoop; }
	inline bool GetIsPlay()const { return mIsPlay; }
	inline pair<int, int> GetNowFrame()const { return mFrameList[mCurrentFrameIndex]; }
	inline int GetNowFrameX()const { return mFrameList[mCurrentFrameIndex].first; }
	inline int GetNowFrameY()const { return mFrameList[mCurrentFrameIndex].second; }

};

