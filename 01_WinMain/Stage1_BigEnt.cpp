#include "pch.h"
#include "Stage1_BigEnt.h"
#include "Animation.h"


Stage1_BigEnt::Stage1_BigEnt(int indexX, int indexY)
	:Enemy(indexX, indexY)
{
	mHp = 60;
	mSizeX = 30.f;
	mSizeY = 30.f;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mResources = L"Monster/stage1/BigEnt/";
	IMAGEMANAGER->LoadFromFile(L"Stage1_BigEnt" + mStateType[(int)StateType::Attack1],
		Resources(mResources + mStateType[(int)StateType::Attack1] + L".bmp"), 980, 112, 7, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_BigEnt" + mStateType[(int)StateType::Attack2],
		Resources(mResources + mStateType[(int)StateType::Attack2] + L".bmp"), 536, 115, 4, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_BigEnt" + mStateType[(int)StateType::Dead],
		Resources(mResources + mStateType[(int)StateType::Dead] + L".bmp"), 100, 77, 1, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_BigEnt" + mStateType[(int)StateType::Idle],
		Resources(mResources + mStateType[(int)StateType::Idle] + L".bmp"), 575, 114, 5, 1, true);

	AnimationSet();
	mDirection = Direction::right;
	CurrentSet(StateType::Idle, mDirection);
	mAttackDelay = 1.f; // ���� ���� 1��
}

void Stage1_BigEnt::Init()
{

}

void Stage1_BigEnt::Update()
{
	if (mHp <= 0) {
		mIsDestroy = true;
		return;
	}
	//if (mType == StateType::Hit) {
	//	if (mIsKnockBack) {
	//		KnockBackMove();
	//	}
	//	mHitTime -= dTime;
	//	if (mHitTime < 0 && !mIsKnockBack) {
	//		mHitTime = 0;
	//		mPath.clear();
	//		mPathIndex = 1;
	//		mTargetTile = nullptr;
	//		mTargetSkulTile = nullptr;
	//		CurrentSet(StateType::Idle, mDirection);
	//	}
	//	else {
	//		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	//		return;
	//	}
	//}
	mCurrentSkul = SKUL->GetCurrentSkul();
	mTargetSkulTile = TILE[mCurrentSkul->GetIndexY()][mCurrentSkul->GetIndexX()];
	//�⺻ ������ idle ���¿����� ���� �̺�Ʈ�� �Ͼ��.
	//if (mType == StateType::Idle) {
	//	if (mType != StateType::Walk) {
	//		if (mType != StateType::Attack && AttackCheck(2)) {
	//			//�������� ��ĭ
	//			Attack();
	//		}
	//		else if (mType != StateType::Attack) {
	//			if (WalkCheck()) {
	//				Walk();
	//			}
	//			else {
	//				MoveReset();
	//			}
	//
	//		}
	//	}
	//}
	//if (mType == StateType::Walk) {
	//	float mAngle = Math::GetAngle(mX, mY, mCurrentSkul->GetX(), mCurrentSkul->GetY());
	//	if (LEFT && mDirection == Direction::right) {
	//		mDirection = Direction::left;
	//		Walk();
	//	}
	//	else if (RIGHT && mDirection == Direction::left) {
	//		mDirection = Direction::right;
	//		Walk();
	//	}
	//	Move(150);
	//}
	//if (mType == StateType::Attack) {
	//	if (mCurrentAnimation->GetNowFrameX() == 0) {
	//		mCurrentAnimation->SetFrameUpdateTime(1.f);
	//	}
	//	if (mCurrentAnimation->GetNowFrameX() != 0) {
	//		mCurrentAnimation->SetFrameUpdateTime(0.1f);
	//	}
	//	if (mCurrentAnimation->GetNowFrameX() == 2) {
	//		if (mAttackEnd) {
	//			AttackDamage();
	//		}
	//	}
	//	if (!mCurrentAnimation->GetIsPlay()) {
	//		mCurrentAnimation->Stop();
	//		CurrentSet(StateType::Idle, mDirection);
	//	}
	//}
	mCurrentAnimation->Update();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}

void Stage1_BigEnt::Release()
{
	// �¿� Animation ����
	for (map<StateType, AnimationPair>::iterator itr = mAnimationMap[0].begin(); itr != mAnimationMap[0].end(); itr++) {
		SafeDelete(itr->second.animation);
	}
	for (map<StateType, AnimationPair>::iterator itr = mAnimationMap[1].begin(); itr != mAnimationMap[1].end(); itr++) {
		SafeDelete(itr->second.animation);
	}
}

void Stage1_BigEnt::Render(HDC hdc)
{
	//CAMERA->RenderRect(hdc, mRect);
	if (mCurrentImage) {
		CAMERA->CenterBottomFrameRender(hdc, mCurrentImage, mX, mY, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY());
	}
}

void Stage1_BigEnt::Walk()
{
	//int skulX = SkulManager::GetInstance()->GetCurrentSkul()->GetX();
	//int skulY = SkulManager::GetInstance()->GetCurrentSkul()->GetY();
	//float angle = Math::GetAngle(mX,mY,skulX,skulY);
	//int dumpIndex = 0;
	//if (mType == StateType::Walk) {
	//	dumpIndex = mCurrentAnimation->GetCurrentFrameIndex();
	//}
	//CurrentSet(StateType::Walk, mDirection);
	//mCurrentAnimation->SetCurrentFrameIndex(dumpIndex);
	//mCurrentAnimation->SetIsLoop(true);
}

void Stage1_BigEnt::Attack()
{
	//0�� ��� �� ����
	float x = mCurrentSkul->GetX();
	float y = mCurrentSkul->GetY();
	float mAngle = Math::GetAngle(mX, mY, x, y);

	if (LEFT) {
		mDirection = Direction::left;
	}
	else {
		mDirection = Direction::right;
	}
	//CurrentSet(StateType::Attack, mDirection);
}

void Stage1_BigEnt::Idle()
{
	CurrentSet(StateType::Idle, mDirection);
	mCurrentAnimation->SetIsLoop(true);
}

void Stage1_BigEnt::Hit()
{
	if (mHitTime > 0) {
		mCurrentAnimation->Play();
		mCurrentAnimation->SetFrameUpdateTime(0.01f);
		mCurrentAnimation->Update();
		mCurrentAnimation->Pause();
		mHitTime = 0.6f;
	}
	else {
		//CurrentSet(StateType::Hit, mDirection);
		mCurrentAnimation->Pause();
		mCurrentAnimation->SetFrameUpdateTime(0.01f);
		mHitTime = 0.6f;
	}
	KnockBack();
}

void Stage1_BigEnt::Damage(int Damage)
{
	mHp -= Damage;
	Hit();
}

void Stage1_BigEnt::Move(int speed)
{

	if (!mPath.empty()) {
		mTargetTile->Update();
		if (mPath.size() <= mPathIndex || !mTargetTile->GetTileEmpty()
			|| mTargetSkulTile != TILE[mCurrentSkul->GetIndexY()][mCurrentSkul->GetIndexX()]) //���������� �̵� �Ϸ�
		{
			mPath.clear();
			mPathIndex = 1;
			mTargetTile = nullptr;
			mTargetSkulTile = nullptr;
			Idle();
		}
		else //�̵� ��
		{
			mPath[mPathIndex]->Update();

			int pathX = mPath[mPathIndex]->GetX() + TileSizeX / 2;
			int pathY = mPath[mPathIndex]->GetY() + TileSizeY / 2;
			float mAngle = Math::GetAngle(mX, mY, pathX, pathY); //�ޱ� �Ÿ� ����� 0�� ���ö� ���� 0���� ����
			mX += speed * cosf(mAngle) * dTime;
			mY -= speed * sinf(mAngle) * dTime;
			if (abs(mX - pathX) < speed * dTime and abs(mY - pathY) < speed * dTime) //���� ����
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

void Stage1_BigEnt::AttackDamage()
{
}

void Stage1_BigEnt::ReMove()
{
	//if (WalkCheck()) {
	//	CurrentSet(StateType::Walk, mDirection);
	//}
}

void Stage1_BigEnt::CurrentSet(StateType type, Direction direction)
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
void Stage1_BigEnt::MoveReset()
{
	mTargetTile = nullptr;
	mPathIndex = 1;
	mPath.clear();
	EnemyInTileCheck();
}
void Stage1_BigEnt::EnemyInTileCheck()
{
	mX = TILE[mIndexY][mIndexX]->GetX() + TileSizeX / 2;
	mY = TILE[mIndexY][mIndexX]->GetY() + TileSizeY / 2;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Stage1_BigEnt::KnockBack()
{
	if (mIsKnockBack) {
		return;
	}
	float mAngle = Math::GetAngle(mCurrentSkul->GetX(), mCurrentSkul->GetY(), mX, mY);

	if (mAngle >= PI2 - (PI / 8) or mAngle < (PI / 8)) //����
	{
		if (mIndexX + 1 < TILESizeX and mIndexY - 1 >= 0)
		{
			mKnockTile = TILE[mIndexY - 1][mIndexX + 1];
		}
	}
	else if (mAngle >= (PI / 8) and mAngle < (3 * PI / 8)) //�����
	{

		if (mIndexY - 1 >= 0 and mIndexY - 1 < TILESizeY)
		{
			mKnockTile = TILE[mIndexY - 1][mIndexX];
		}
	}
	else if (mAngle >= (3 * PI / 8) and mAngle < (5 * PI / 8)) //����
	{
		if (mIndexX - 1 >= 0 and mIndexY - 1 >= 0)
		{
			mKnockTile = TILE[mIndexY - 1][mIndexX - 1];
		}
	}
	else if (mAngle >= (5 * PI / 8) and mAngle < (7 * PI / 8)) //�»���
	{
		if (mIndexX - 1 >= 0)
		{
			mKnockTile = TILE[mIndexY][mIndexX - 1];
		}
	}
	else if (mAngle >= (7 * PI / 8) and mAngle < (9 * PI / 8)) //����
	{
		if (mIndexX - 1 >= 0 and mIndexY + 1 < TILESizeY)
		{
			mKnockTile = TILE[mIndexY + 1][mIndexX - 1];
		}
	}
	else if (mAngle >= (9 * PI / 8) and mAngle < (11 * PI / 8)) //������
	{
		if (mIndexY + 1 < TILESizeY)
		{
			mKnockTile = TILE[mIndexY + 1][mIndexX];
		}
	}
	else if (mAngle >= (11 * PI / 8) and mAngle < (13 * PI / 8)) //����
	{
		if (mIndexX + 1 < TILESizeX and mIndexY + 1 < TILESizeY)
		{
			mKnockTile = TILE[mIndexY + 1][mIndexX + 1];
		}
	}
	else if (mAngle >= (13 * PI / 8) and mAngle < (15 * PI / 8)) //������
	{
		if (mIndexX + 1 < TILESizeX)
		{
			mKnockTile = TILE[mIndexY][mIndexX + 1];
		}
	}
	EnemyInTileCheck();
	mIsKnockBack = true; //�˹��� �Ͼ�� ��
}
void Stage1_BigEnt::KnockBackMove()
{
	if (!mKnockTile) {
		return;
	}

	if (mKnockTile->GetIndexX() == mIndexX && mKnockTile->GetIndexY() == mIndexY) {
		mIsKnockBack = false;
		mKnockTile = nullptr;
		return;
	}
	else //�̵� ��
	{
		float speed = 200.f;
		int pathX = mKnockTile->GetX() + TileSizeX / 2;
		int pathY = mKnockTile->GetY() + TileSizeY / 2;
		float mAngle = Math::GetAngle(mX, mY, pathX, pathY); //�ޱ� �Ÿ� ����� 0�� ���ö� ���� 0���� ����
		mX += speed * cosf(mAngle) * dTime;
		mY -= speed * sinf(mAngle) * dTime;
		if (abs(mX - pathX) < speed * dTime and abs(mY - pathY) < speed * dTime) //���� ����
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
bool Stage1_BigEnt::AttackCheck(int area)
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
bool Stage1_BigEnt::WalkCheck() //�� ĭ üũ �� �̵�
{
	if (mTargetTile != nullptr) {
		mTargetTile->Update();
		if (mTargetTile->GetTileEmpty()) {
			return true;
		}
	}
	float m = Math::GetDistance(mX, mY, mCurrentSkul->GetX(), mCurrentSkul->GetY()); //�Ÿ�
	if (m > 800.f) {
		return false;
	}
	int indexX = mCurrentSkul->GetIndexX();
	int indexY = mCurrentSkul->GetIndexY();
	vector<Tile*> moveTileList;
	for (int y = indexY - 2; y < indexY + 3; y++) {
		for (int x = indexX - 2; x < indexX + 3; x++) {
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
void Stage1_BigEnt::AnimationSet() {
	//����
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_BigEnt" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, 0, image->GetFrameX() - 1, 0, false);
		animation->SetFrameUpdateTime(0.1f);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::left].insert(make_pair(type, aPair));
	}
	//������
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_BigEnt" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, 0, image->GetFrameX() - 1, 0, false);
		animation->SetFrameUpdateTime(0.1f);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::right].insert(make_pair(type, aPair));
	}
}