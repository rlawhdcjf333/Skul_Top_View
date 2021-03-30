#include "pch.h"
#include "Stage1_Hunter.h"
#include "Animation.h"
#include "FixedSysFont.h"
#include "EnemyArrow.h"

Stage1_Hunter::Stage1_Hunter(int indexX, int indexY)
	:Enemy(indexX, indexY), mShotTime(2.f)
{
	mHp = 60;
	mSizeX = 30.f;
	mSizeY = 30.f;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mResources = L"Monster/stage1/Hunter/";
	IMAGEMANAGER->LoadFromFile(L"Stage1_Hunter" + mStateType[(int)StateType::Attack],
		Resources(mResources + mStateType[(int)StateType::Attack] + L".bmp"), 228, 98, 4, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_Hunter" + mStateType[(int)StateType::Hit],
		Resources(mResources + mStateType[(int)StateType::Hit] + L".bmp"), 102, 96, 2, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_Hunter" + mStateType[(int)StateType::Idle],
		Resources(mResources + mStateType[(int)StateType::Idle] + L".bmp"), 190, 104, 5, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Stage1_Hunter" + mStateType[(int)StateType::Walk],
		Resources(mResources + mStateType[(int)StateType::Walk] + L".bmp"), 228, 98, 6, 2, true);

	AnimationSet();
	mDirection = Direction::right;
	CurrentSet(StateType::Idle, mDirection);
	mAttackDelay = 1.f; // ���� ���� 1��
}

void Stage1_Hunter::Init()
{

}

void Stage1_Hunter::Update()
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
	if (mTargetSkulTile != nullptr) {
		mCurrentSkulTileCheckTime += dTime;
	}
	else {
		mCurrentSkulTileCheckTime = 0;
	}
	if (mType == StateType::Walk) {
		if (mTargetSkulTile == nullptr) {
			if (WalkCheck()) {
				Walk();
			}
			else {
				MoveReset();
			}
		}
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
		if (mCurrentAnimation->GetNowFrameX() != 3) {
			mCurrentAnimation->SetFrameUpdateTime(0.1f);
		}
		else if (mCurrentAnimation->GetNowFrameX() == 3) {
			mCurrentAnimation->SetFrameUpdateTime(2.5f);
		}
		if (!mCurrentAnimation->GetIsPlay()) {
			//mCurrentAnimation->Stop();
			Shot();
			CurrentSet(StateType::Idle, mDirection);
		}
	}
	//�⺻ ������ idle ���¿����� ���� �̺�Ʈ�� �Ͼ��.
	if (mType == StateType::Idle) {
		if (mType != StateType::Walk) {
			if (mType != StateType::Attack && AttackCheck(5)) {
				//�������� ��ĭ
				Attack();
				mAttackEnd = true;
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
	mCurrentAnimation->Update();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}

void Stage1_Hunter::Release()
{
	// �¿� Animation ����
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

void Stage1_Hunter::Render(HDC hdc)
{

	while (mDamages.size() > 0) {
		new FixedSysFont(mX, mY, 100, 100, to_wstring(mDamages.top()), FontColor::Blue);
		mDamages.pop();
	}
	//CAMERA->RenderRect(hdc, mRect);
	if (mCurrentImage) {
		CAMERA->CenterBottomFrameRender(hdc, mCurrentImage, mX, mY, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY());
	}
}

void Stage1_Hunter::Walk()
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

void Stage1_Hunter::Attack()
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

	CurrentSet(StateType::Attack, mDirection);
}

void Stage1_Hunter::Idle()
{
	CurrentSet(StateType::Idle, mDirection);
	mCurrentAnimation->SetIsLoop(true);
}

void Stage1_Hunter::Hit()
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
		mHitTime = 0.6f; //�� Ÿ�̹����� �ֵ� �ǰ� ���� üũ�� �ϰ� �����Ƿ� ����Ǹ� �˷��ֽñ� �ٶ�
	}
	KnockBack();
}

void Stage1_Hunter::Shot()
{
	float angle = Math::GetAngle(mX,mY,SKUL->GetCurrentSkul()->GetX(), SKUL->GetCurrentSkul()->GetY());
	new EnemyArrow(mX,mY, angle,500.f);
}

void Stage1_Hunter::Damage(int Damage)
{
	mHp -= Damage;
	mDamages.emplace(Damage);
	Hit();
}

void Stage1_Hunter::Move(int speed)
{

	if (!mPath.empty()) {
		mTargetTile->Update();
		if (mPath.size() <= mPathIndex) //���������� �̵� �Ϸ�
		{
			mPath.clear();
			mPathIndex = 1;
			mTargetTile = nullptr;
			mTargetSkulTile = nullptr;
			Idle();
		}
		else if ((mTargetSkulTile != TILE[mCurrentSkul->GetIndexY()][mCurrentSkul->GetIndexX()] || !mTargetTile->GetTileEmpty()) && (mCurrentSkulTileCheckTime > TileCheckTime)) {
			mPath.clear();
			mPathIndex = 1;
			mTargetTile = nullptr;
			mTargetSkulTile = nullptr;
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

void Stage1_Hunter::AttackDamage(int range, int damage)
{
	for (int y = mIndexY - range; y <= mIndexY + range; y++) {
		for (int x = mIndexX - range; x <= mIndexX + range; x++) {
			if (y <= 0 || y > TILESizeY || x <= 0 || x > TILESizeX) {
				continue;
			}
			if (mDirection == Direction::right) //����
			{
				if (y - x > mIndexY - mIndexX) continue;
				TILE[y][x]->EnemyAttack(damage);
			}
			else if (mDirection == Direction::left) //����
			{
				if (y - x < mIndexY - mIndexX) continue;
				TILE[y][x]->EnemyAttack(damage);
			}
		}
	}
	mAttackEnd = false;
}

void Stage1_Hunter::ReMove()
{
	if (WalkCheck()) {
		CurrentSet(StateType::Walk, mDirection);
	}
}

void Stage1_Hunter::CurrentSet(StateType type, Direction direction)
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
void Stage1_Hunter::MoveReset()
{
	mTargetTile = nullptr;
	mPathIndex = 1;
	mPath.clear();
	EnemyInTileCheck();
}
void Stage1_Hunter::EnemyInTileCheck()
{
	mX = TILE[mIndexY][mIndexX]->GetX() + TileSizeX / 2;
	mY = TILE[mIndexY][mIndexX]->GetY() + TileSizeY / 2;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Stage1_Hunter::KnockBack()
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
	if (mKnockTile != nullptr) {
		if (mKnockTile->GetType() == TileType::Block) {
			mKnockTile = TILE[mIndexY][mIndexX];
		}
	}
	EnemyInTileCheck();
	mIsKnockBack = true; //�˹��� �Ͼ�� ��
}
void Stage1_Hunter::KnockBackMove()
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
bool Stage1_Hunter::AttackCheck(int area)
{
	float m = Math::GetDistance(mX, mY, mCurrentSkul->GetX(), mCurrentSkul->GetY());
	if (m < 500.f) {
		return true;
	}
	return false;
}
bool Stage1_Hunter::WalkCheck() //�� ĭ üũ �� �̵�
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
	for (int y = indexY - 5; y < indexY + 6; y++) {
		for (int x = indexX - 5; x < indexX + 6; x++) {
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
	//if (!(mTargetTile == nullptr))
	//{
	//	
	//}
	if (!(mTargetTile->GetIndexX() == mCurrentSkul->GetIndexX() &&
		mTargetTile->GetIndexY() == mCurrentSkul->GetIndexY())) {
		if (PathFinder::GetInstance()->FindPath(TILE, mPath, mIndexX, mIndexY, mTargetTile->GetIndexX(), mTargetTile->GetIndexY())) {
			mTargetSkulTile = TILE[SKUL->GetCurrentSkul()->GetIndexY()][SKUL->GetCurrentSkul()->GetIndexX()];
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
void Stage1_Hunter::AnimationSet() {
	//����
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_Hunter" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::left, image->GetFrameX() - 1, (int)Direction::left, false);
		animation->SetFrameUpdateTime(0.1f);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::left].insert(make_pair(type, aPair));
	}
	//������
	for (int a = 0; a < (int)StateType::End; a++) {
		StateType type = (StateType)a;
		Image* image = IMAGEMANAGER->FindImage(L"Stage1_Hunter" + mStateType[a]);
		Animation* animation = new Animation();
		animation->InitFrameByStartEnd(0, (int)Direction::right, image->GetFrameX() - 1, (int)Direction::right, false);
		animation->SetFrameUpdateTime(0.1f);
		AnimationPair aPair = { image,animation };
		mAnimationMap[(int)Direction::right].insert(make_pair(type, aPair));
	}
}