#include "pch.h"
#include "Animation.h"
#include "Stage1_Wizard.h"
#include "FixedSysFont.h"
#include "FirePillar.h"

Stage1_Wizard::Stage1_Wizard(int indexX, int indexY)
	:Enemy(indexX, indexY), mNowTeleport(false), mNextAttackTime(0), mNextTeleportTime(0)
{
	mHp = 20;
	mSizeX = 30.f;
	mSizeY = 30.f;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mResources = L"Monster/stage1/Wizard/";
	IMAGEMANAGER->LoadFromFile(L"Stage1_Wizard" + mStateType[(int)StateType::Attack],
		Resources(mResources + mStateType[(int)StateType::Attack] + L".bmp"), 258, 134, 6, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_Wizard" + mStateType[(int)StateType::Hit],
		Resources(mResources + mStateType[(int)StateType::Hit] + L".bmp"), 116, 98, 2, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_Wizard" + mStateType[(int)StateType::Idle],
		Resources(mResources + mStateType[(int)StateType::Idle] + L".bmp"), 215, 112, 5, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_Wizard" + mStateType[(int)StateType::Teleport_In],
		Resources(mResources + mStateType[(int)StateType::Teleport_In] + L".bmp"), 344, 130, 8, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_Wizard" + mStateType[(int)StateType::Teleport_Out],
		Resources(mResources + mStateType[(int)StateType::Teleport_Out] + L".bmp"), 215, 110, 5, 2, true);

	AnimationSet();
	mDirection = Direction::right;
	CurrentSet(StateType::Idle, mDirection);
	mAttackDelay = 1.f; // 공격 간격 1초
	TILE[mIndexY][mIndexY]->SetObject(this);
}

void Stage1_Wizard::Init()
{

}

void Stage1_Wizard::Update()
{
	mNextAttackTime -= dTime;
	mNextTeleportTime -= dTime;
	if (mNextAttackTime < 0) {
		mNextAttackTime = 0;
	}
	if (mNextTeleportTime < 0) {
		mNextTeleportTime = 0;
	}
	if (mHp <= 0) {
		mIsDestroy = true;
		return;
	}
	if (mType == StateType::Hit) {
		if (mIsKnockBack) {
			KnockBackMove();
		}
		mHitTime -= dTime;
		if (mHitTime < 0 && !mIsKnockBack) {
			mHitTime = 0;
			mPath.clear();
			mPathIndex = 1;
			mTargetTile = nullptr;
			mTargetSkulTile = nullptr;
			CurrentSet(StateType::Idle, mDirection);
		}
		else {
			mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
			return;
		}
	}
	mCurrentSkul = SKUL->GetCurrentSkul();
	if (mTargetSkulTile != nullptr) {
		mCurrentSkulTileCheckTime += dTime;
	}
	else {
		mCurrentSkulTileCheckTime = 0;
	}
	if (mType == StateType::Teleport_In) {
		if (!mCurrentAnimation->GetIsPlay()) {
			Teleport();
		}
	}
	else if (mType == StateType::Teleport_Out) {
		if (!mCurrentAnimation->GetIsPlay()) {
			mNowTeleport = false;
			Idle();
		}
	}
	else if (mType == StateType::Attack) {
		if (mCurrentAnimation->GetIsPlay()) {
			mAttackTarget = RectMakeBottom(mTargetTile->GetX()+30,mTargetTile->GetY()+50,80,100);
			//추후 불기둥에 해당 Rect값을 보내 렉트 출돌 체크 후 데미지
		}
		else if (!mCurrentAnimation->GetIsPlay()) {
			Attack();
			mTargetTile = nullptr;
			CurrentSet(StateType::Idle, mDirection);
		}
	}
	//기본 적으로 idle 상태에서만 다음 이벤트가 일어난다.
	else if (mType == StateType::Idle) {
		if (mType != StateType::Attack && AttackCheck(5)) {
			int indexX = SKUL->GetCurrentSkul()->GetIndexX();
			int indexY = SKUL->GetCurrentSkul()->GetIndexY();
			mTargetTile = TILE[indexY][indexX];
			float mAngle = Math::GetAngle(mX, mY, mTargetTile->GetX(), mTargetTile->GetY());
			if (LEFT)
				mDirection = Direction::left;
			else
				mDirection = Direction::right;

			CurrentSet(StateType::Attack, mDirection);
			mCurrentAnimation->SetFrameUpdateTime(0.2f);
			mAttackEnd = true;
		}
		else if (mType != StateType::Attack) {
			if (TeleportCheck()) {
				mNowTeleport = true;
				float mAngle = Math::GetAngle(mX,mY, mTargetTile->GetX(),mTargetTile->GetY());
				if (LEFT)
					mDirection = Direction::left;
				else
					mDirection = Direction::right;

				CurrentSet(StateType::Teleport_In, mDirection);
			}
		}
	}
	mCurrentAnimation->Update();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}

void Stage1_Wizard::Release()
{
	// 좌우 Animation 삭제
	for (map<StateType, AnimationPair>::iterator itr = mAnimationMap[0].begin(); itr != mAnimationMap[0].end(); itr++) {
		SafeDelete(itr->second.animation);
	}
	for (map<StateType, AnimationPair>::iterator itr = mAnimationMap[1].begin(); itr != mAnimationMap[1].end(); itr++) {
		SafeDelete(itr->second.animation);
	}
	while (mDamages.size() > 0) {
		new FixedSysFont(mX, mY, 100, 100, to_wstring(mDamages.top()), FontColor::Blue);
		mDamages.pop();
	}
}

void Stage1_Wizard::Render(HDC hdc)
{
	//if (mType == StateType::Attack) {
	//	CAMERA->RenderRect(hdc, mAttackTarget);
	//}
	while (mDamages.size() > 0) {
		new FixedSysFont(mX, mY, 100, 100, to_wstring(mDamages.top()), FontColor::Blue);
		mDamages.pop();
	}
	//CAMERA->RenderRect(hdc, mRect);
	if (mCurrentImage) {
		CAMERA->CenterBottomFrameRender(hdc, mCurrentImage, mX, mY, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY());
	}
}

void Stage1_Wizard::Attack()
{
	//0번 대기 후 공격
	//float x = mCurrentSkul->GetX();
	//float y = mCurrentSkul->GetY();
	//float mAngle = Math::GetAngle(mX, mY, x, y);
	//if (LEFT) {
	//	mDirection = Direction::left;
	//}
	//else {
	//	mDirection = Direction::right;
	//}
	//CurrentSet(StateType::Attack, mDirection);
	new FirePillar(mTargetTile->GetX(),mTargetTile->GetY(),mAttackTarget);
	mNextAttackTime = 3.f;
}

void Stage1_Wizard::Idle()
{
	CurrentSet(StateType::Idle, mDirection);
	mCurrentAnimation->SetIsLoop(true);
}

void Stage1_Wizard::Hit()
{
	if (mHitTime > 0) {
		mCurrentAnimation->Play();
		mCurrentAnimation->SetFrameUpdateTime(0.01f);
		mCurrentAnimation->Update();
		mCurrentAnimation->Pause();
		if (mHitTime < 0.6f) mHitTime = 0.6f;
	}
	else {
		CurrentSet(StateType::Hit, mDirection);
		mCurrentAnimation->Pause();
		mCurrentAnimation->SetFrameUpdateTime(0.01f);
		mHitTime = 0.6f; //이 타이밍으로 애들 피격 순간 체크를 하고 있으므로 변경되면 알려주시길 바람
	}
	KnockBack();
}
void Stage1_Wizard::Teleport()
{
	mIndexX = mTargetTile->GetIndexX();
	mIndexY = mTargetTile->GetIndexY();
	EnemyInTileCheck();
	TILE[mIndexY][mIndexX]->SetObject(this);
	CurrentSet(StateType::Teleport_Out,mDirection);
	mTargetTile = nullptr;
	mNextTeleportTime = 8.f;
}
void Stage1_Wizard::Damage(int Damage)
{
	if (mNowTeleport) {
		return;
	}
	mHp -= Damage;
	mDamages.emplace(Damage);
	Hit();
}
void Stage1_Wizard::AttackDamage()
{

	mAttackEnd = false;
}

void Stage1_Wizard::CurrentSet(StateType type, Direction direction)
{
	if (mDirection != direction)
		mDirection = direction;
	if (mCurrentAnimation) {
		mCurrentAnimation->Stop();
	}
	mCurrentAnimation = mAnimationMap[(int)direction].find(type)->second.animation;
	mCurrentImage = mAnimationMap[(int)direction].find(type)->second.image;
	mCurrentAnimation->Play();
	mType = type;
}
void Stage1_Wizard::MoveReset()
{
	mTargetTile = nullptr;
	mPathIndex = 1;
	mPath.clear();
	EnemyInTileCheck();
}
void Stage1_Wizard::EnemyInTileCheck()
{
	mX = TILE[mIndexY][mIndexX]->GetX() + TileSizeX / 2;
	mY = TILE[mIndexY][mIndexX]->GetY() + TileSizeY / 2;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Stage1_Wizard::KnockBack()
{
	if (mIsKnockBack) {
		return;
	}
	float mAngle = Math::GetAngle(mCurrentSkul->GetX(), mCurrentSkul->GetY(), mX, mY);

	if (mAngle >= PI2 - (PI / 8) or mAngle < (PI / 8)) //우향
	{
		if (mIndexX + 1 < TILESizeX and mIndexY - 1 >= 0)
		{
			mKnockTile = TILE[mIndexY - 1][mIndexX + 1];
		}
	}
	else if (mAngle >= (PI / 8) and mAngle < (3 * PI / 8)) //우상향
	{

		if (mIndexY - 1 >= 0 and mIndexY - 1 < TILESizeY)
		{
			mKnockTile = TILE[mIndexY - 1][mIndexX];
		}
	}
	else if (mAngle >= (3 * PI / 8) and mAngle < (5 * PI / 8)) //상향
	{
		if (mIndexX - 1 >= 0 and mIndexY - 1 >= 0)
		{
			mKnockTile = TILE[mIndexY - 1][mIndexX - 1];
		}
	}
	else if (mAngle >= (5 * PI / 8) and mAngle < (7 * PI / 8)) //좌상향
	{
		if (mIndexX - 1 >= 0)
		{
			mKnockTile = TILE[mIndexY][mIndexX - 1];
		}
	}
	else if (mAngle >= (7 * PI / 8) and mAngle < (9 * PI / 8)) //좌향
	{
		if (mIndexX - 1 >= 0 and mIndexY + 1 < TILESizeY)
		{
			mKnockTile = TILE[mIndexY + 1][mIndexX - 1];
		}
	}
	else if (mAngle >= (9 * PI / 8) and mAngle < (11 * PI / 8)) //좌하향
	{
		if (mIndexY + 1 < TILESizeY)
		{
			mKnockTile = TILE[mIndexY + 1][mIndexX];
		}
	}
	else if (mAngle >= (11 * PI / 8) and mAngle < (13 * PI / 8)) //하향
	{
		if (mIndexX + 1 < TILESizeX and mIndexY + 1 < TILESizeY)
		{
			mKnockTile = TILE[mIndexY + 1][mIndexX + 1];
		}
	}
	else if (mAngle >= (13 * PI / 8) and mAngle < (15 * PI / 8)) //우하향
	{
		if (mIndexX + 1 < TILESizeX)
		{
			mKnockTile = TILE[mIndexY][mIndexX + 1];
		}
	}
	if (mKnockTile != nullptr) {
		if (mKnockTile->GetType() == TileType::Block) {
			mKnockTile = TILE[mIndexY][mIndexX];
		}
	}
	EnemyInTileCheck();
	mIsKnockBack = true; //넉백이 일어났을 때
}
void Stage1_Wizard::KnockBackMove()
{
	if (!mKnockTile) {
		return;
	}

	if (mKnockTile->GetIndexX() == mIndexX && mKnockTile->GetIndexY() == mIndexY) {
		mIsKnockBack = false;
		mKnockTile = nullptr;
		return;
	}
	else //이동 중
	{
		float speed = 200.f;
		int pathX = mKnockTile->GetX() + TileSizeX / 2;
		int pathY = mKnockTile->GetY() + TileSizeY / 2;
		float mAngle = Math::GetAngle(mX, mY, pathX, pathY); //앵글 거리 계산이 0이 나올때 리턴 0으로 막음
		mX += speed * cosf(mAngle) * dTime;
		mY -= speed * sinf(mAngle) * dTime;
		if (abs(mX - pathX) < speed * dTime and abs(mY - pathY) < speed * dTime) //오차 보정
		{
			mX = pathX;
			mY = pathY;
			mIndexX = mKnockTile->GetIndexX();
			mIndexY = mKnockTile->GetIndexY();
			mKnockTile->SetObject(this);
			mKnockTile->Update();
		}
	}
}
bool Stage1_Wizard::TeleportCheck()
{
	if (mNextTeleportTime != 0) {
		return false;
	}
	float m = Math::GetDistance(mX, mY, mCurrentSkul->GetX(), mCurrentSkul->GetY()); //거리
	if (m > 800.f) {
		return false;
	}
	int indexX = mCurrentSkul->GetIndexX();
	int indexY = mCurrentSkul->GetIndexY();
	vector<Tile*> moveTileList;
	for (int y = indexY - 4; y < indexY + 4; y++) {
		for (int x = indexX - 4; x < indexX + 4; x++) {
			if ((y >= 0 && y < TILESizeY) && (x >= 0 && x < TILESizeX)
				&& !(x == indexX && y == indexY)) {
				if (TILE[y][x]->GetType() != TileType::Block) {
					TILE[y][x]->Update();
					if (TILE[y][x]->GetTileEmpty()) {
						moveTileList.push_back(TILE[y][x]);
					}
				}
			}
		}
	}
	for (int a = 0; a < moveTileList.size(); a++) {
		if (a == 0) {
			mTargetTile = moveTileList[a];
		}
		else {
			float firstM = Math::GetDistance(mX, mY, mTargetTile->GetX(), mTargetTile->GetY());
			float secondM = Math::GetDistance(mX, mY, moveTileList[a]->GetX(), moveTileList[a]->GetY());
			if (firstM > secondM) {
				mTargetTile = moveTileList[a];
			}
		}
	}
	if (mTargetTile != nullptr) {
		return true;
	}
	mTargetTile = nullptr;
	return false;
}
bool Stage1_Wizard::AttackCheck(int area)
{
	if (mNextAttackTime !=0) {
		return false;
	}

	int indexX = mCurrentSkul->GetIndexX();
	int indexY = mCurrentSkul->GetIndexY();

	if (indexX - area <= mIndexX && indexX + area >= mIndexX) {
		if (indexY - area <= mIndexY && indexY + area >= mIndexY) {
			return true;
		}
	}
	return false;
}

void Stage1_Wizard::AnimationSet() {
	//왼쪽
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_Wizard" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::left, image->GetFrameX() - 1, (int)Direction::left, false);
		animation->SetFrameUpdateTime(0.1f);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::left].insert(make_pair(type, aPair));
	}
	//오른쪽
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_Wizard" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::right, image->GetFrameX() - 1, (int)Direction::right, false);
		animation->SetFrameUpdateTime(0.1f);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::right].insert(make_pair(type, aPair));
	}
}