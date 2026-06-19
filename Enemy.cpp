#include<DxLib.h>
#include "Enemy.h"
#include"Application.h"
#include"Player.h"


Enemy::Enemy(GameScene* gs)
{
	img = -1;
	gInst = gs;
}

Enemy::~Enemy(void)
{

}

bool Enemy::SystemInit(void)
{
	img = LoadGraph("image/enemy.png");
	if (img == -1)return false;

	return true;
}

void Enemy::GameInit(void)
{
	Pos.x = 1400;
	Pos.y = 50;

	animCounter = 0;
	aliveFlg = true;
}

void Enemy::Update(void)
{
	animCounter++;
	if (animCounter > (ANIM_NUMS * ANIM_INTERVAL) * 10)animCounter = 0;
}

void Enemy::Draw(void)
{
	int animNo = (animCounter / ANIM_INTERVAL) % ANIM_NUMS;

	if (aliveFlg) {
		DrawGraph(Pos.x, Pos.y, img, true);
	}

	//座標をint　型に変換する
	Vector2 ePos;
	ePos = AsoUtility::Round(Pos);

}


bool Enemy::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

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

void Enemy::SetDamage(int dp)
{
	hp -= dp;
	if (hp <= 0) {
		hp = 0;
		aliveFlg = false;
	}
}