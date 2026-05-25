#include<DxLib.h>
#include "Enemy.h"
#include"Application.h"
#include"Player.h"


Enemy::Enemy(GameScene* gs)
{
	gInst = gs;
}

Enemy::~Enemy(void)
{

}

bool Enemy::SystemInit(void)
{

	int result = LoadDivGraph("image/enemy1.png",8, 4, 2, 480,440, img);

	return true;
}

void Enemy::GameInit(void)
{
	Pos.x = 1700;
	Pos.y = 600;
	hp = 300;
	state = 0;
	motionTimer = 0;
	animCounter = 0;
	aliveFlg = true;
	isGroundPounded = false;
	currentImg = -1;
	nextAttackDelay = 0;

	nextAttackDelay = 120 + rand() % 180;
}

void Enemy::Update(void)
{
	motionTimer++;
	animeTimer++;
	isGroundPounded = false; // フラグは毎フレームリセット

	if (state == 0) {
		if (animeTimer % 30 < 15) {
			currentImg = img[0];
		}
		else {
			currentImg = img[1];
		}
		if (motionTimer >= nextAttackDelay) {
			state = 1;
			motionTimer = 0;
			animeTimer = 0;
		}
	}
	//state1 =攻撃
	else if (state == 1) {
		//叩きモーション中約一秒経ったら、地面をたたく
		if (motionTimer < 15) {
			currentImg = img[2];   //足を上げる動作
		}
		else if (motionTimer < 30) {
			currentImg = img[3];   //前足を振り下ろす
		}
		else if (motionTimer < 45) {
			if (motionTimer == 30) {
				isGroundPounded = true;    //地面をたたいた瞬間にフラグをオンにする
			}
			currentImg = img[6];
		}
		else if (motionTimer < 60) {
			currentImg = img[7]; //態勢を戻す
		}
		else {
			state = 0;
			motionTimer = 0;
			animeTimer = 0;
			nextAttackDelay = 170 + rand() % 180;    //次の待ち時間を再設定
		}
	
	}

}

void Enemy::Draw(void)
{
	if (currentImg != -1) {

		bool isLeft = true;   //左を向くフラグ

		double Rate = 1.5f;   //2倍に拡大する

		int cx = 240; // 480の半分
		int cy = 220; // 440の半分

		if (isLeft) {
			// 左右反転 ＋ 拡大して描画（左を向く）
			// 最後の引数を「TRUE」にすると左右反転になります
			DrawRotaGraph2(Pos.x, Pos.y , cx, cy, Rate, 0.0, currentImg, true, true);
		}
		else {
			// 通常向き ＋ 拡大して描画（右を向く）
			// 最後の引数を「FALSE」にすると反転なしになります
			DrawRotaGraph2(Pos.x, Pos.y , cx, cy, Rate, 0.0, currentImg, true, false);
		}
	}

	//座標をint　型に変換する
	Vector2 ePos;
	ePos = AsoUtility::Round(Pos);

}


bool Enemy::Release(void)
{
	for (int i = 0; i < 8;i++) {
		if (img[i] != -1) {
			DeleteGraph(img[i]);
			img[i] = -1;
		}
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

void Enemy::SetDamage(int dp)
{
	hp -= dp;
	if (hp <= 0) {
		hp = 0;
		aliveFlg = false;
	}
}

bool Enemy::CheckAndResetPoundFlag() {
	if (isGroundPounded) {
		isGroundPounded = false;
		return true;
	}
	return false;
}