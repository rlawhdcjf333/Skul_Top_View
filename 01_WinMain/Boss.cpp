#include "pch.h"
#include "Boss.h"
#include "Animation.h"
#include "FixedSysFont.h"
#include "Effect.h"
#include "BossFireBall.h"

Boss::Boss(int x, int y)
	:GameObject(), mHp(300), mInvincibility(false), mNextAnimation(0), mDeadCheck(false), mSkillTime1(0), mSkillTime2(0), mShotCheck(true)
{
	IMAGEMANAGER->LoadFromFile(L"Boss_Attack1",Resources(L"Monster/Boss/Attack1.bmp"), 416, 138, 8, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Attack2(End)", Resources(L"Monster/Boss/Attack2(End).bmp"), 781, 152, 11, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Attack2(Ready)", Resources(L"Monster/Boss/Attack2(Ready).bmp"), 715, 192, 13, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Attack2(Ready2)", Resources(L"Monster/Boss/Attack2(Ready2).bmp"), 171, 182, 3, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Attack3(Landing)", Resources(L"Monster/Boss/Attack3(Landing).bmp"), 384, 108, 8, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Attack3(Ready)", Resources(L"Monster/Boss/Attack3(Ready).bmp"), 174, 116, 6, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Attack4(re)", Resources(L"Monster/Boss/Attack4(re).bmp"), 186, 178, 6, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Attack4(Ready)", Resources(L"Monster/Boss/Attack4(Ready).bmp"), 396, 160, 9, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Attack4", Resources(L"Monster/Boss/Attack4.bmp"), 183, 162, 6, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Die", Resources(L"Monster/Boss/Die.bmp"), 350, 82, 5, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Groggy", Resources(L"Monster/Boss/Groggy.bmp"), 236,82, 4, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Hit", Resources(L"Monster/Boss/Hit.bmp"), 102, 154, 3, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Idle", Resources(L"Monster/Boss/Idle.bmp"), 246, 142, 6, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Intro1(left)", Resources(L"Monster/Boss/Intro1(left).bmp"), 330, 324, 10, 4, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Intro1(right)", Resources(L"Monster/Boss/Intro1(right).bmp"), 330, 324, 10, 4, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Intro2", Resources(L"Monster/Boss/Intro2.bmp"), 420, 192, 10, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Landing_Outro", Resources(L"Monster/Boss/Landing_Outro.bmp"), 451, 132, 11, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Landing_Ready", Resources(L"Monster/Boss/Landing_Ready.bmp"), 174, 116, 3, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Landing1(left)", Resources(L"Monster/Boss/Landing1(left).bmp"), 7347, 211, 31, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Landing1(right)", Resources(L"Monster/Boss/Landing1(right).bmp"), 7347, 211, 31, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Landing2(left)", Resources(L"Monster/Boss/Landing1(left).bmp"), 1200, 69, 12, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Landing2(right)", Resources(L"Monster/Boss/Landing1(right).bmp"), 1200, 69, 12, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_LandingEnd(left)", Resources(L"Monster/Boss/LandingEnd(left).bmp"), 8400, 216, 28, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_LandingEnd(right)", Resources(L"Monster/Boss/LandingEnd(right).bmp"), 8400, 216, 28, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Potion", Resources(L"Monster/Boss/Potion.bmp"), 246, 138, 6, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Walk1(Front)", Resources(L"Monster/Boss/Walk1(Front).bmp"), 342, 110, 6, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_Walk2(Back)", Resources(L"Monster/Boss/Walk2(Back).bmp"), 360, 120, 6, 2, true);
	IMAGEMANAGER->LoadFromFile(L"Boss_BossAttackEffect", Resources(L"Monster/Boss/BossAttackEffect.bmp"), 2400, 400, 12, 2, true);

	mIndexX = x;
	mIndexY = y;
	TILE[mIndexY][mIndexX]->SetObject(this);
	mX = TILE[mIndexY][mIndexX]->GetCenterX();
	mY = TILE[mIndexY][mIndexX]->GetCenterY();
	mRect = RectMakeBottom(mX,mY,60,100);
	mCurentPattern = BossPattern::StartRanding;
	mAnimation = new Animation();
	mAngle = Math::GetAngle(mX,mY,SKUL->GetCurrentSkul()->GetX(),SKUL->GetCurrentSkul()->GetY());
}
void Boss::Init()
{
}

void Boss::Update()
{
	mSkillTime1 -= dTime;
	mSkillTime2 -= dTime;
	if (mSkillTime1 < 0) {
		mSkillTime1 = 0;
	}
	if (mSkillTime2 < 0) {
		mSkillTime2 = 0;
	}
	if (mHp <= 0) {
		mCurentPattern = BossPattern::Dead;
	}
	mAngle = Math::GetAngle(mX, mY, SKUL->GetCurrentSkul()->GetX(), SKUL->GetCurrentSkul()->GetY());
	if (mCurentPattern == BossPattern::StartRanding) {
		StartRanding();
	}
	else if (mCurentPattern == BossPattern::Normal) {
		Normal();
	}
	else if (mCurentPattern == BossPattern::RandingAttack) {
		RandingAttack();
	}
	else if (mCurentPattern == BossPattern::Dead) {
		Dead();
	}
	if (mCurentPattern == BossPattern::Idle)
	{
		if (mSkillTime1 == 0) {
			mCurentPattern = BossPattern::Normal;
			Normal();
		}
		else if (mSkillTime2 ==0) {
			mCurentPattern = BossPattern::RandingAttack;
			RandingAttack();
		}
		else {
			Idle();
		}
	}
	
	if (mAnimation) {
		mAnimation->Update();
	}
}

void Boss::Render(HDC hdc)
{
	while (mDamages.size() > 0) {
		new FixedSysFont(mX, mY, 100, 100, to_wstring(mDamages.front()), FontColor::Blue);
		mDamages.pop();
	}
	CAMERA->CenterBottomFrameRender(hdc,mImage,mX,mY,mAnimation->GetNowFrameX(),mAnimation->GetNowFrameY());
}

void Boss::Release()
{

}

void Boss::StartRanding()
{
	if (mNextAnimation == 0) {
		mInvincibility = true;
		if (LEFT) {
			mImage = IMAGEMANAGER->FindImage(L"Boss_Landing1(left)");
		}
		else {
			mImage = IMAGEMANAGER->FindImage(L"Boss_Landing1(right)");
		}
		mAnimation->InitFrameByStartEnd(0,0, mImage->GetFrameX() - 1,0,false);
		mAnimation->SetFrameUpdateTime(0.1f);
		mAnimation->Play();
		++mNextAnimation;
	}
	else if (mNextAnimation == 1 && mAnimation->GetIsPlay()) {
		CAMERA->PanningOn(2);
		CAMERA->Panning();
	}
	else if (mNextAnimation == 1 && !mAnimation->GetIsPlay()) {
		mImage = IMAGEMANAGER->FindImage(L"Boss_Attack1");
		mAnimation->SetAnimationClear();
		if (LEFT) {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 0, false);
		}
		else {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 1, false);
		}
		new Effect(L"Boss_BossAttackEffect", mX, mY, EffectType::Normal);
		mAnimation->SetFrameUpdateTime(0.1f);
		mAnimation->Play();
		++mNextAnimation;
	}
	else if (mNextAnimation == 2 && !mAnimation->GetIsPlay()) {
		mAnimation->SetAnimationClear();
		mCurentPattern = BossPattern::Idle;
		mInvincibility = false;
		mNextAnimation = 0;
	}
}

void Boss::Idle()
{
	int Index = NULL;
	if (mImage == IMAGEMANAGER->FindImage(L"Boss_Idle")) {
		Index = mAnimation->GetCurrentFrameIndex();
	}
	else {
		mImage = IMAGEMANAGER->FindImage(L"Boss_Idle");
	}
	if (LEFT) {
		mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 0, false);
	}
	else {
		mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 1, false);
	}
	mAnimation->SetFrameUpdateTime(0.1f);
	if (Index) {
		mAnimation->SetCurrentFrameIndex(Index);
	}
	mShotCheck = true;
	mAnimation->SetIsLoop(true);
	mAnimation->Play();
}

void Boss::Randing()
{
	
}

void Boss::Normal()
{
	if (mNextAnimation == 0) {
		mImage = IMAGEMANAGER->FindImage(L"Boss_Attack4(Ready)");
		mAnimation->SetAnimationClear();
		if (LEFT) {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 0, false);
		}
		else {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 1, false);
		}
		new Effect(L"Boss_BossAttackEffect", mX, mY, EffectType::Normal);
		mAnimation->SetFrameUpdateTime(0.1f);
		mAnimation->Play();
		mNextAnimation++;
	}
	else if (mNextAnimation == 1 && !mAnimation->GetIsPlay()) {
		mImage = IMAGEMANAGER->FindImage(L"Boss_Attack1");
		mAnimation->SetAnimationClear();
		if (LEFT) {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 0, false);
		}
		else {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 1, false);
		}
		mAnimation->SetFrameUpdateTime(0.1f);
		mAnimation->Play();
		mNextAnimation++;
	}
	else if (mNextAnimation == 2 && mAnimation->GetIsPlay()) {
		if (mAnimation->GetCurrentFrameIndex() == 1) {
			if (mShotCheck) {
				new BossFireBall(mX, mY, mAngle, 300.f);
				mShotCheck = false;
			}
			
		}
		else if (mAnimation->GetCurrentFrameIndex() == 4) {
			if (mShotCheck) {
				new BossFireBall(mX, mY, mAngle, 300.f);
				mShotCheck = false;
			}
		}
		else {
			mShotCheck = true;
		}
	}
	else if (mNextAnimation == 2 && !mAnimation->GetIsPlay()) {
		mImage = IMAGEMANAGER->FindImage(L"Boss_Attack1");
		mAnimation->SetAnimationClear();
		if (LEFT) {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 0, false);
		}
		else {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 1, false);
		}
		mAnimation->SetFrameUpdateTime(0.1f);
		mAnimation->Play();
		mNextAnimation++;
	}
	else if (mNextAnimation == 3 && mAnimation->GetIsPlay()) {
		if (mAnimation->GetCurrentFrameIndex() == 1) {
			if (mShotCheck) {
				new BossFireBall(mX, mY, mAngle, 300.f);
				mShotCheck = false;
			}

		}
		else if (mAnimation->GetCurrentFrameIndex() == 4) {
			if (mShotCheck) {
				new BossFireBall(mX, mY, mAngle, 300.f);
				mShotCheck = false;
			}
		}
		else {
			mShotCheck = true;
		}
	}
	else if (mNextAnimation == 3 && !mAnimation->GetIsPlay()) {
		mAnimation->SetAnimationClear();
		mCurentPattern = BossPattern::Idle;
		mInvincibility = false;
		mNextAnimation = 0;
		mSkillTime1 = 7.f;
	}
}

void Boss::SideCut()
{

}

void Boss::RandingAttack()
{
	if (mNextAnimation == 0) {
		mInvincibility = true;
		mTargetTile = TILE[SKUL->GetCurrentSkul()->GetIndexY()][SKUL->GetCurrentSkul()->GetIndexX()];
		mImage = IMAGEMANAGER->FindImage(L"Boss_Landing_Outro");
		mAnimation->SetAnimationClear();
		if (LEFT) {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 0, false);
		}
		else {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 1, false);
		}
		mAnimation->SetFrameUpdateTime(0.1f);
		mAnimation->Play();
		mNextAnimation++;
	}
	else if (mNextAnimation == 1 && !mAnimation->GetIsPlay()) {
		mIndexX = mTargetTile->GetIndexX();
		mIndexY = mTargetTile->GetIndexY();
		TILE[mIndexY][mIndexX]->SetObject(this);
		mX = TILE[mIndexY][mIndexX]->GetCenterX();
		mY = TILE[mIndexY][mIndexX]->GetCenterY();
		mRect = RectMakeBottom(mX, mY, 60, 100);
		mAnimation->SetAnimationClear();
		if (LEFT) {
			mImage = IMAGEMANAGER->FindImage(L"Boss_LandingEnd(left)");
		}
		else {
			mImage = IMAGEMANAGER->FindImage(L"Boss_LandingEnd(right)");
		}
		mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 0, false);
		mAnimation->SetFrameUpdateTime(0.1f);
		mAnimation->Play();
		mNextAnimation++;
	}
	else if (mNextAnimation == 2 && mAnimation->GetIsPlay()) {
		if (mAnimation->GetCurrentFrameIndex() == 10) {
			for (int y = mIndexY - 2; y <= mIndexY + 2; y++) {
				for (int x = mIndexX - 2; x <= mIndexX + 2; x++) {
					if (y <= 0 || y > TILESizeY || x <= 0 || x > TILESizeX) {
						continue;
					}
					TILE[y][x]->EnemyAttack(5.f);
				}
			}
		}
	}
	else if (mNextAnimation == 2 && !mAnimation->GetIsPlay()) {
		mAnimation->SetAnimationClear();
		mCurentPattern = BossPattern::Idle;
		mInvincibility = false;
		mNextAnimation = 0;
		mSkillTime2 = 15.f;
	}
}

void Boss::ChargingAttack()
{

}

void Boss::PhaseUp()
{

}

void Boss::WaveAttack()
{

}

void Boss::DoubleWaveAttack()
{

}

void Boss::Dead()
{
	mDeadCheck = true;
	if (mNextAnimation == 0) {
		mAnimation->SetAnimationClear();
		mImage = IMAGEMANAGER->FindImage(L"Boss_Die");
		if (LEFT) {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 0, false);
		}
		else {
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, 1, false);
		}
		mAnimation->Play();
		mNextAnimation++;
	}
	else if (mNextAnimation ==1 && mAnimation->GetIsPlay()) {
		if (mAnimation->GetCurrentFrameIndex() == 4) {
			mAnimation->Pause();
		}
	}
	
}

void Boss::Move()
{
	//if (!mPath.empty()) {
	//	mTargetTile->Update();
	//	if (mPath.size() <= mPathIndex) //목적지까지 이동 완료
	//	{
	//		mPath.clear();
	//		mPathIndex = 1;
	//		mTargetTile = nullptr;
	//		mTargetSkulTile = nullptr;
	//		Idle();
	//	}
	//	else if ((mTargetSkulTile != TILE[mCurrentSkul->GetIndexY()][mCurrentSkul->GetIndexX()] || !mTargetTile->GetTileEmpty()) && (mCurrentSkulTileCheckTime > TileCheckTime)) {
	//		mPath.clear();
	//		mPathIndex = 1;
	//		mTargetTile = nullptr;
	//		mTargetSkulTile = nullptr;
	//	}
	//	else //이동 중
	//	{
	//		mPath[mPathIndex]->Update();
	//
	//		int pathX = mPath[mPathIndex]->GetX() + TileSizeX / 2;
	//		int pathY = mPath[mPathIndex]->GetY() + TileSizeY / 2;
	//		float mAngle = Math::GetAngle(mX, mY, pathX, pathY); //앵글 거리 계산이 0이 나올때 리턴 0으로 막음
	//		mX += speed * cosf(mAngle) * dTime;
	//		mY -= speed * sinf(mAngle) * dTime;
	//		if (abs(mX - pathX) < speed * dTime and abs(mY - pathY) < speed * dTime) //오차 보정
	//		{
	//			mX = pathX;
	//			mY = pathY;
	//			mIndexX = mPath[mPathIndex]->GetIndexX();
	//			mIndexY = mPath[mPathIndex]->GetIndexY();
	//			TILE[mIndexY][mIndexX]->SetObject(this);
	//			TILE[mIndexY][mIndexX]->Update();
	//			mPathIndex++;
	//		}
	//
	//	}
	//}
}

void Boss::Damage(int damage)
{
	if (!mInvincibility && !mDeadCheck) {
		mHp -= damage;
		mDamages.emplace(damage);
	}
}
