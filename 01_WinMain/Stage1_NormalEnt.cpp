#include "pch.h"
#include "Animation.h"
#include "Image.h"
#include "Stage1_NormalEnt.h"
Stage1_NormalEnt::Stage1_NormalEnt(int indexX, int indexY)
	:Enemy(indexX, indexY)
{
	mHp = 60;
	mSizeX = 30.f;
	mSizeY = 30.f;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mResources = L"Monster/stage1/NormalEnt/";
	IMAGEMANAGER->LoadFromFile(L"Stage1_NormalEnt" + mStateType[(int)StateType::Attack],
		Resources(mResources + mStateType[(int)StateType::Attack] + L".bmp"), 260, 128, 4, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_NormalEnt" + mStateType[(int)StateType::Hit],
		Resources(mResources + mStateType[(int)StateType::Hit] + L".bmp"), 116, 134, 2, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_NormalEnt" + mStateType[(int)StateType::Idle],
		Resources(mResources + mStateType[(int)StateType::Idle] + L".bmp"), 285, 132, 5, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_NormalEnt" + mStateType[(int)StateType::Walk],
		Resources(mResources + mStateType[(int)StateType::Walk] + L".bmp"), 324, 130, 6, 2, true);

	AnimationSet();
	mDirection = Direction::right;
	CurrentSet(StateType::Idle, mDirection);
	mAttackDelay = 1.f; // 공격 간격 1초6
}

void Stage1_NormalEnt::Init()
{

}

void Stage1_NormalEnt::Update()
{
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
	mTargetSkulTile = TILE[mCurrentSkul->GetIndexY()][mCurrentSkul->GetIndexX()];
	//기본 적으로 idle 상태에서만 다음 이벤트가 일어난다.
	if (mType == StateType::Idle) {
		if (mType != StateType::Walk) {
			if (mType != StateType::Attack && AttackCheck(1)) {
				//근접으로 한칸
				Attack();
			}
			else if (mType != StateType::Attack) {
				if (WalkCheck()) {
					Walk();
				}
				else {
					MoveReset();
				}

			}
		}
	}
	if (mType == StateType::Walk) {
		float mAngle = Math::GetAngle(mX, mY, mCurrentSkul->GetX(), mCurrentSkul->GetY());
		if (LEFT && mDirection == Direction::right) {
			mDirection = Direction::left;
			Walk();
		}
		else if (RIGHT && mDirection == Direction::left) {
			mDirection = Direction::right;
			Walk();
		}
		Move(150);
	}
	if (mType == StateType::Attack) {
		if (mCurrentAnimation->GetNowFrameX() == 0) {
			mCurrentAnimation->SetFrameUpdateTime(1.f);
		}
		if (mCurrentAnimation->GetNowFrameX() != 0) {
			mCurrentAnimation->SetFrameUpdateTime(0.1f);
		}
		if (mCurrentAnimation->GetNowFrameX() == 2) {
			if (mAttackEnd) {
				AttackDamage();
			}
		}
		if (!mCurrentAnimation->GetIsPlay()) {
			mCurrentAnimation->Stop();
			CurrentSet(StateType::Idle, mDirection);
		}
	}
	mCurrentAnimation->Update();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}

void Stage1_NormalEnt::Release()
{
	// 좌우 Animation 삭제
	for (map<StateType, AnimationPair>::iterator itr = mAnimationMap[0].begin(); itr != mAnimationMap[0].end(); itr++) {
		SafeDelete(itr->second.animation);
	}
	for (map<StateType, AnimationPair>::iterator itr = mAnimationMap[1].begin(); itr != mAnimationMap[1].end(); itr++) {
		SafeDelete(itr->second.animation);
	}
}

void Stage1_NormalEnt::Render(HDC hdc)
{
	//CAMERA->RenderRect(hdc, mRect);
	if (mCurrentImage) {
		CAMERA->CenterBottomFrameRender(hdc, mCurrentImage, mX, mY, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY());
	}
}

void Stage1_NormalEnt::Walk()
{
	//int skulX = SkulManager::GetInstance()->GetCurrentSkul()->GetX();
	//int skulY = SkulManager::GetInstance()->GetCurrentSkul()->GetY();
	//float angle = Math::GetAngle(mX,mY,skulX,skulY);
	int dumpIndex = 0;
	if (mType == StateType::Walk) {
		dumpIndex = mCurrentAnimation->GetCurrentFrameIndex();
	}
	CurrentSet(StateType::Walk, mDirection);
	mCurrentAnimation->SetCurrentFrameIndex(dumpIndex);
	mCurrentAnimation->SetIsLoop(true);
}

void Stage1_NormalEnt::Attack()
{
	//0번 대기 후 공격
	float x = mCurrentSkul->GetX();
	float y = mCurrentSkul->GetY();
	float mAngle = Math::GetAngle(mX, mY, x, y);

	if (LEFT) {
		mDirection = Direction::left;
	}
	else {
		mDirection = Direction::right;
	}
	CurrentSet(StateType::Attack, mDirection);
}

void Stage1_NormalEnt::Idle()
{
	CurrentSet(StateType::Idle, mDirection);
	mCurrentAnimation->SetIsLoop(true);
}

void Stage1_NormalEnt::Hit()
{
	if (mHitTime > 0) {
		mCurrentAnimation->Play();
		mCurrentAnimation->SetFrameUpdateTime(0.01f);
		mCurrentAnimation->Update();
		mCurrentAnimation->Pause();
		mHitTime = 0.6f;
	}
	else {
		CurrentSet(StateType::Hit, mDirection);
		mCurrentAnimation->Pause();
		mCurrentAnimation->SetFrameUpdateTime(0.01f);
		mHitTime = 0.6f;
	}
	KnockBack();
}

void Stage1_NormalEnt::Damage(int Damage)
{
	mHp -= Damage;
	Hit();
}

void Stage1_NormalEnt::Move(int speed)
{
	if (!mPath.empty()) {
		mTargetTile->Update();
		if (mPath.size() <= mPathIndex || !mTargetTile->GetTileEmpty()
			|| mTargetSkulTile != TILE[mCurrentSkul->GetIndexY()][mCurrentSkul->GetIndexX()]) //목적지까지 이동 완료
		{
			mPath.clear();
			mPathIndex = 1;
			mTargetTile = nullptr;
			mTargetSkulTile = nullptr;
			Idle();
		}
		else //이동 중
		{
			mPath[mPathIndex]->Update();

			int pathX = mPath[mPathIndex]->GetX() + TileSizeX / 2;
			int pathY = mPath[mPathIndex]->GetY() + TileSizeY / 2;
			float mAngle = Math::GetAngle(mX, mY, pathX, pathY); //앵글 거리 계산이 0이 나올때 리턴 0으로 막음
			mX += speed * cosf(mAngle) * dTime;
			mY -= speed * sinf(mAngle) * dTime;
			if (abs(mX - pathX) < speed * dTime and abs(mY - pathY) < speed * dTime) //오차 보정
			{
				mX = pathX;
				mY = pathY;
				mIndexX = mPath[mPathIndex]->GetIndexX();
				mIndexY = mPath[mPathIndex]->GetIndexY();
				TILE[mIndexY][mIndexX]->SetObject(this);
				TILE[mIndexY][mIndexX]->Update();
				mPathIndex++;
			}

		}
	}
}

void Stage1_NormalEnt::AttackDamage()
{
}

void Stage1_NormalEnt::ReMove()
{
	if (WalkCheck()) {
		CurrentSet(StateType::Walk, mDirection);
	}
}

void Stage1_NormalEnt::CurrentSet(StateType type, Direction direction)
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
void Stage1_NormalEnt::MoveReset()
{
	mTargetTile = nullptr;
	mPathIndex = 1;
	mPath.clear();
	EnemyInTileCheck();
}
void Stage1_NormalEnt::EnemyInTileCheck()
{
	mX = TILE[mIndexY][mIndexX]->GetX() + TileSizeX / 2;
	mY = TILE[mIndexY][mIndexX]->GetY() + TileSizeY / 2;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Stage1_NormalEnt::KnockBack()
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
	EnemyInTileCheck();
	mIsKnockBack = true; //넉백이 일어났을 때
}
void Stage1_NormalEnt::KnockBackMove()
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
bool Stage1_NormalEnt::AttackCheck(int area)
{
	int indexX = mCurrentSkul->GetIndexX();
	int indexY = mCurrentSkul->GetIndexY();

	if (indexX - area <= mIndexX && indexX + area >= mIndexX) {
		if (indexY - area <= mIndexY && indexY + area >= mIndexY) {
			return true;
		}
	}
	return false;
}
bool Stage1_NormalEnt::WalkCheck() //빈 칸 체크 후 이동
{
	if (mTargetTile != nullptr) {
		mTargetTile->Update();
		if (mTargetTile->GetTileEmpty()) {
			return true;
		}
	}
	float m = Math::GetDistance(mX, mY, mCurrentSkul->GetX(), mCurrentSkul->GetY()); //거리
	if (m > 800.f) {
		return false;
	}
	int indexX = mCurrentSkul->GetIndexX();
	int indexY = mCurrentSkul->GetIndexY();
	vector<Tile*> moveTileList;
	for (int y = indexY - 1; y < indexY + 2; y++) {
		for (int x = indexX - 1; x < indexX + 2; x++) {
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
	moveTileList.clear();
	if (!(mTargetTile->GetIndexX() == mCurrentSkul->GetIndexX() &&
		mTargetTile->GetIndexY() == mCurrentSkul->GetIndexY())) {
		if (PathFinder::GetInstance()->FindPath(TILE, mPath, mIndexX, mIndexY, mTargetTile->GetIndexX(), mTargetTile->GetIndexY())) {
			return true;
		}
		else {
			mTargetTile = nullptr;
			return false;
		}
	}
	mTargetTile = nullptr;
	return false;
}
void Stage1_NormalEnt::AnimationSet() {
	//왼쪽
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_NormalEnt" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::left, image->GetFrameX() - 1, (int)Direction::left, false);
		animation->SetFrameUpdateTime(0.1f);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::left].insert(make_pair(type, aPair));
	}
	//오른쪽
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_NormalEnt" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::right, image->GetFrameX() - 1, (int)Direction::right, false);
		animation->SetFrameUpdateTime(0.1f);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::right].insert(make_pair(type, aPair));
	}
}