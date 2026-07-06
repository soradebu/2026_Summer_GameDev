#include <DxLib.h>
#include "Fire.h"
#include "EnemyMino.h"
#include "Application.h"
#include "Player.h"


EnemyMino::EnemyMino(SceneBase* scene)
{
	m_pScene = scene;
}

EnemyMino::~EnemyMino(void)
{

}

bool EnemyMino::SystemInit(void)
{
	mino_img[0] = LoadGraph("image/Boss_2/Pose_1.png");
	mino_img[1] = LoadGraph("image/Boss_2/Pose_2.png");

	mino_run[0] = LoadGraph("image/Boss_2/Run_1.png");
	mino_run[1] = LoadGraph("image/Boss_2/Run_2.png");
	mino_run[2] = LoadGraph("image/Boss_2/Run_3.png");
	mino_run[3] = LoadGraph("image/Boss_2/Run_4.png");

	mino_attack[0] = LoadGraph("image/Boss_2/Attack_1.png");
	mino_attack[1] = LoadGraph("image/Boss_2/Attack_2.png");
	mino_attack[2] = LoadGraph("image/Boss_2/Attack_3.png");
	mino_attack[3] = LoadGraph("image/Boss_2/Attack_4.png");
	mino_attack[4] = LoadGraph("image/Boss_2/Attack_5.png");

	for (int i = 0; i < 2; i++)
	{
		if (mino_img[i] == -1)
		{

			return false;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (mino_run[i] == -1)
		{
			return false;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (mino_attack[i] == -1)
		{
			return false;
		}
	}

	// 1コマのサイズ
	int WID = ENEMY_SIZE_WID;
	int HIG = ENEMY_SIZE_HIG;

	// 待機モーション
	for (int i = 0; i < 2; i++)
	{
		minoImages[static_cast<int>(state::IDLE)][i] = mino_img[i];
	}

	// 移動モーション
	for (int i = 0; i < 4; i++)
	{
		minoImages[static_cast<int>(state::RUN)][i] =mino_run[i];
	}

	// 攻撃モーション
	for (int i = 0; i < 5; i++)
	{
		minoImages[static_cast<int>(state::ATTACK)][i] = mino_attack[i];
	}

	return true;
}

void EnemyMino::GameInit(void)
{
	Pos.x = 1440;
	Pos.y = 570;
	hp = 20;
	animCounter = 0;
	animeTimer = 0;
	aliveFlg = true;
	aTimer = 200;
	goTimer = 30;
	moveSpeedX = 0;
	currentImg = -1;

	isCharging = false;

	currentstate = state::IDLE;
}

void EnemyMino::Update(void)
{
	if (!aliveFlg) return;
	animeTimer++;

	if (currentstate != state::RUN)
	{
		aTimer--;
	}

	if (aTimer <= 0)
	{
		int pat = GetRand(3);

		if (pat == 3)
		{
			currentstate = state::RUN;
			aTimer = 2000;
		}
		else
		{
			currentstate = state::ATTACK;
			aTimer = 10000;

			isCharging = false;
		}

		animNo = 0;
		animeTimer = 0;

		if (currentstate == state::IDLE)
		{
			animNo = 0;
			animeTimer = 0;

			isCharging = false; 
		}
	}

	if (currentstate == state::RUN)
	{
		if (goTimer >= 0)
		{
			goTimer--;

			isCharging = true;
		}
		else
		{
			Pos.x += moveSpeedX;
			isCharging = false;

			if (Pos.x <= 320.0f)
			{
				Pos.x = 320.0f;
				moveSpeedX = 20.0f;
			}

			if (Pos.x >= 1440.0f)
			{
				Pos.x = 1440.0f;
				moveSpeedX = -100.0f;

				currentstate = state::IDLE;
				aTimer = 120;

				goTimer = 30;
			}
		}

	}

	animeTimer++;

	int animSpeed = 15; // デフォルト
	if (currentstate == state::RUN)    animSpeed = 6;  
	if (currentstate == state::ATTACK) animSpeed = 20; 

	if (animeTimer >= animSpeed)
	{
		animeTimer = 0;
		animNo++;

		int maxFrames = 2;
		if (currentstate == state::RUN)    maxFrames = 4;
		if (currentstate == state::ATTACK) maxFrames = 5;

		if (animNo >= maxFrames)
		{
			if (currentstate == state::ATTACK)
			{
				currentstate = state::IDLE;
				aTimer = 60 + GetRand(30);
			}

			animNo = 0;
		}
	}
}

void EnemyMino::Draw(void)
{
	if (!aliveFlg) return;

	// 現在の画像ハンドルを取得
	int currentHandle = minoImages[static_cast<int>(currentstate)][animNo];

	// 走行前は赤くそれ以外は通常
	if (currentHandle > 0 && currentHandle != -1)
	{
		if (isCharging)
		{
			SetDrawBright(255, 100, 100);
		}

		DrawTurnGraph(static_cast<int>(Pos.x), static_cast<int>(Pos.y), currentHandle, TRUE);

		if (isCharging)
		{
			SetDrawBright(255, 255, 255); 
		}
	}
}


bool EnemyMino::Release(void)
{

	for (int i = 0; i < 2; i++)
	{
		if (mino_img[i] != -1) DeleteGraph(mino_img[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		if (mino_img[i] != -1) DeleteGraph(mino_run[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		if (mino_img[i] != -1) DeleteGraph(mino_attack[i]);
	}
	

	return true;
}

/*
* --------------------------------
* 敵にダメージを与える
* Input;
*     int dp : 与えるダメージ
* OutPut:
*     なし
* -------------------------------
*/
void EnemyMino::SetDamage(int dp)
{
	hp -= dp;
	if (hp <= 0)
	{
		hp = 0;
		aliveFlg = false;
	}
}

