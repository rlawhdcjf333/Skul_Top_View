#include "pch.h"
#include "Animation.h"
#include "Stage1_SwordMan.h"

Stage1_SwordMan::Stage1_SwordMan(int indexX, int indexY)
	:Enemy(indexX,indexY)
{
	mSizeX = 30.f;
	mSizeY = 30.f;
	mRect = RectMakeBottom(mX,mY,mSizeX, mSizeY);
	mResources = L"/Monster/stage1/SwordMan/";
	AnimationSet();
}

void Stage1_SwordMan::Init()
{

}

void Stage1_SwordMan::Update()
{
}

void Stage1_SwordMan::Release()
{
	// 谅快 Animation 昏力
	for (map<StateType, AnimationPair>::iterator itr = mAnimationMap[0].begin(); itr != mAnimationMap[0].end(); itr++) {
		SafeDelete(itr->second.animation);
	}
	for (map<StateType, AnimationPair>::iterator itr = mAnimationMap[1].begin(); itr != mAnimationMap[1].end(); itr++) {
		SafeDelete(itr->second.animation);
	}
}

void Stage1_SwordMan::Render(HDC hdc)
{
	CAMERA->RenderRect(hdc, mRect);
}

void Stage1_SwordMan::Walk()
{
	int skulX = SkulManager::GetInstance()->GetCurrentSkul()->GetX();
	int skulY = SkulManager::GetInstance()->GetCurrentSkul()->GetY();
}

void Stage1_SwordMan::Attack()
{
}

void Stage1_SwordMan::Idle()
{
	CurrentSet(StateType::Attack,Direction::left);
}

void Stage1_SwordMan::Hit()
{
}

void Stage1_SwordMan::Damage(int Damage)
{

}

void Stage1_SwordMan::CurrentSet(StateType type, Direction direction)
{
	mCurrentAnimation = mAnimationMap[(int)direction].find(type)->second.animation;
	mCurrentImage = mAnimationMap[(int)direction].find(type)->second.image;
}
void Stage1_SwordMan::AnimationSet() {
	//哭率
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(Resources(mResources + mStateType[a]));
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::left, image->GetFrameX(), image->GetFrameY(), false);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::left].insert(make_pair(type, aPair));
	}
	//坷弗率
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(Resources(mResources + mStateType[a]));
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::right, image->GetFrameX(), image->GetFrameY(), false);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::right].insert(make_pair(type, aPair));
	}
}