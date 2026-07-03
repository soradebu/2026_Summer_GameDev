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
	mino_img[1] = LoadGraph("image/Boss_2/Pose_1.png");

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
	Pos.x = 1200;
	Pos.y = 570;
	hp = 20;
	animCounter = 0;
	aliveFlg = true;
	currentImg = -1;

	currentstate = state::IDLE;
}

void EnemyMino::Update(void)
{
	animeTimer++;

	if (animeTimer >= 10) 
	{
		animeTimer = 0;
		animNo++;

		// 現在の状態に応じた最大コマ数を設定する
		int maxFrames = 2; // デフォルト（待機は2コマ）

		if (currentstate == state::RUN)   maxFrames = 4; // 移動は4コマ
		if (currentstate == state::ATTACK) maxFrames = 5; // 攻撃は5コマ

		// コマ数が最大値に達したら 0 に戻してループさせる
		if (animNo >= maxFrames)
		{
			animNo = 0;
		}
	}

	currentstate = state::IDLE;
}

void EnemyMino::Draw(void)
{
	// -------------------------------------------------------------
	// 現在の画像ハンドルを取得
	int currentHandle = minoImages[static_cast<int>(currentstate)][animNo];

	DrawTurnGraph(Pos.x, Pos.y, currentHandle, TRUE);

}


bool EnemyMino::Release(void)
{
	for (int a = 0; a < static_cast<int>(state::MAX); a++)
	{
		for (int b = 0; b < MAX_ANIMS; b++)
		{
			if (minoImages[a][b] > 0)
			{
				DeleteGraph(minoImages[a][b]);
				minoImages[a][b] = 0;
			}
		}
	}

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

