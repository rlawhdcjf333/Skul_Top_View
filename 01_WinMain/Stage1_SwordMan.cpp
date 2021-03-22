#include "pch.h"
#include "Animation.h"
#include "Stage1_SwordMan.h"

Stage1_SwordMan::Stage1_SwordMan(int indexX, int indexY)
	:Enemy(indexX,indexY)
{
	mHp = 10;
	mSizeX = 30.f;
	mSizeY = 30.f;
	mRect = RectMakeBottom(mX,mY,mSizeX, mSizeY);
	mResources = L"/Monster/stage1/SwordMan/";
	IMAGEMANAGER->LoadFromFile(L"Stage1_SwordMan"+ mStateType[(int)StateType::Attack],
		Resources(mResources+mStateType[(int)StateType::Attack]),312,128,4,2,true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_SwordMan" + mStateType[(int)StateType::Hit], 
		Resources(mResources + mStateType[(int)StateType::Hit]), 92, 102, 2, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_SwordMan" + mStateType[(int)StateType::Idle],
		Resources(mResources + mStateType[(int)StateType::Idle]), 204, 112, 6, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_SwordMan" + mStateType[(int)StateType::Walk], 
		Resources(mResources + mStateType[(int)StateType::Walk]), 416, 100, 8, 2, true);
	AnimationSet();
	mDirection = Direction::right;
	CurrentSet(StateType::Idle,mDirection);
}

void Stage1_SwordMan::Init()
{

}

void Stage1_SwordMan::Update()
{
	mCurrentAnimation->Update();
}

void Stage1_SwordMan::Release()
{
	// 좌우 Animation 삭제
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
	if (mCurrentImage) {
		CAMERA->FrameRender(hdc,mCurrentImage,mRect.left,mRect.top,mCurrentAnimation->GetNowFrameX(),mCurrentAnimation->GetNowFrameY());
	}
}

void Stage1_SwordMan::Walk()
{
	int skulX = SkulManager::GetInstance()->GetCurrentSkul()->GetX();
	int skulY = SkulManager::GetInstance()->GetCurrentSkul()->GetY();
	float angle = Math::GetAngle(mX,mY,skulX,skulY);
	
	CurrentSet(StateType::Walk, mDirection);
}

void Stage1_SwordMan::Attack()
{
	if (mIndexY - 1 >= 0 ) {
		TILE[mIndexY-1][mIndexX];
	}else if (mIndexX - 1 >=0 && mIndexY - 1 >= 0) {
		TILE[mIndexY-1][mIndexX-1];
	}
	else if (mIndexX + 1 < TILESizeY && mIndexY-1 >= 0) {
		TILE[mIndexY-1][mIndexX+1];
	}
	else if (mIndexX - 1 >= 0 ) {
		TILE[mIndexY][mIndexX-1];
	}
	else if (mIndexX + 1 < TILESizeX) {
		TILE[mIndexY][mIndexX+1];
	}
	else if (mIndexY + 1 < TILESizeY) {
		TILE[mIndexY+1][mIndexX];
	}
	else if (mIndexX - 1 >= 0 && mIndexY + 1 < TILESizeY) {
		TILE[mIndexY+1][mIndexX-1];
	}
	else if (mIndexX + 1 < TILESizeX && mIndexY + 1 < TILESizeY) {
		TILE[mIndexY+1][mIndexX+1];
	}
}

void Stage1_SwordMan::Idle()
{
	CurrentSet(StateType::Idle, mDirection);
}

void Stage1_SwordMan::Hit()
{
	CurrentSet(StateType::Hit, mDirection);
}

void Stage1_SwordMan::Damage(int Damage)
{
	
}

void Stage1_SwordMan::move()
{
	//int pathX = mPath[mPathIndex]->GetX() + TileSizeX / 2;
	//int pathY = mPath[mPathIndex]->GetY() + TileSizeY / 2;
	//mAngle = Math::GetAngle(mX, mY, pathX, pathY); //앵글 거리 계산이 0이 나올때 리턴 0으로 막음
	//mX += 15.f * cosf(mAngle) * dTime;
	//mY -= 15.f * sinf(mAngle) * dTime;
	//
	//if (abs(mX - pathX) < 15.f * dTime and abs(mY - pathY) < 15.f * dTime) //오차 보정
	//{
	//	mX = pathX;
	//	mY = pathY;
	//	mIndexX = mPath[mPathIndex]->GetIndexX();
	//	mIndexY = mPath[mPathIndex]->GetIndexY();
	//	TILE[mIndexY][mIndexX]->SetObject(this);
	//	mPathIndex++;
	//}
}

void Stage1_SwordMan::CurrentSet(StateType type, Direction direction)
{
	if (mDirection != direction)
		mDirection = direction;
	if (mCurrentAnimation) {
		mCurrentAnimation->Stop();
	}
	mCurrentAnimation = mAnimationMap[(int)direction].find(type)->second.animation;
	mCurrentImage = mAnimationMap[(int)direction].find(type)->second.image;
}
void Stage1_SwordMan::AnimationSet() {
	//왼쪽
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_SwordMan" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::left, image->GetFrameX(), image->GetFrameY(), false);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::left].insert(make_pair(type, aPair));
	}
	//오른쪽
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_SwordMan" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::right, image->GetFrameX(), image->GetFrameY(), false);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::right].insert(make_pair(type, aPair));
	}
}