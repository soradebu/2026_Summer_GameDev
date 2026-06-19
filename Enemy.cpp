#include<DxLib.h>
#include"Fire.h"
#include "Enemy.h"
#include"Application.h"
#include"Player.h"


Enemy::Enemy(GameScene* gs)
{
	gInst = gs;

	for (int i = 0; i < 12; i++) {
		img[i] = -1;
	}
}

Enemy::~Enemy(void)
{

}

bool Enemy::SystemInit(void)
{

	int result = LoadDivGraph("image/enemy5.png",12, 4, 3, 480,400, img);

	return true;
}

void Enemy::GameInit(void)
{
	Pos.x = 1700;
	Pos.y = 800;
	hp = 20;
	state = 0;
	motionTimer = 0;
	animCounter = 0;
	aliveFlg = true;
	isGroundPounded = false;
	isFireBreathing = false;
	currentImg = -1;
	nextAttackDelay = 0;

	nextAttackDelay = 120 + rand() % 180;
}

void Enemy::Update(void)
{
	motionTimer++;
	animeTimer++;

	if (state == 0) {
		isFireBreathing = false;
		isStoneRaining = false;
		isGroundPounded = false;

		// 待機は1行目の0と1をなめらかに交互ループ
		if (animeTimer % 20 < 10) {
			currentImg = img[0];
		}
		else {
			currentImg = img[1];
		}

		// 待機タイマーが来たら攻撃へ
		if (motionTimer >= nextAttackDelay) {
			state = 1;
			motionTimer = 0;
			animeTimer = 0;
			// ランダムに攻撃パターン決定(0が火.1が石)
			attack = rand() % 2;
		}
	}
	//攻撃モーション
	else if (state == 1) {
		// 足上げポーズ身構え
		if (motionTimer < 5) {
			currentImg = img[7];
		}
		// 攻撃パターンによってルートが分岐
		else if (attack == 0) {
			//火を噴くモーション
			if (motionTimer < 11) {
				currentImg = img[10]; // 口を開けて激しくブレス放射
				if (motionTimer == 5) {
					isFireBreathing = true; // 火の攻撃判定
				}
			}
			else if (motionTimer < 17) {
				isFireBreathing = false; // 火の攻撃判定
				currentImg = img[11]; //後隙
			}
			else {
				goto ATTACK_END; //攻撃終了処理
			}
		}
		else {
			if (motionTimer < 6) {
				currentImg = img[7];
				if (motionTimer == 2) {
					isStoneRaining = true; //石が降る攻撃判定
				}
			}
			else if (motionTimer < 10) {
				isStoneRaining = false; // 石が降る判定を消す

				if (motionTimer == 6) {
					isGroundPounded = true; //地面に叩き付けた瞬間に衝撃波判定ON
				}
				currentImg = img[7];
			}
			else {
				goto ATTACK_END; // 攻撃終了処理
			}
			if (Pos.x < 360) {
				Pos.x = 360; // これ以上左に行かせない
			}
		}
		return;

		//共通の終了処理
		ATTACK_END:
		state = 0;
		motionTimer = 0;
		animeTimer = 0;
		//次の攻撃までのクールダウン時間を再設定
		nextAttackDelay = 170 + rand() % 180;
	}

}

void Enemy::Draw(void)
{
	if (!aliveFlg) return;

	if (currentImg != -1) {
		bool isLeft = true;
		double Rate = 1.5;

		int cx = 240;
		int cy = 200;

		// 描画用のX座標を一時変数に受ける
		double drawX = Pos.x;

		// 石の攻撃モーション中、かつ画面左側（例えば座標500以下）に見切れそうな時
		if ((currentImg == img[8] || currentImg == img[9]) && Pos.x < 500) {
			// 左を向いているなら、描画位置を右に200ピクセルほど押し戻す
			if (isLeft) {
				drawX += 200;
			}
		}

		if (isLeft) {
			DrawRotaGraph2(drawX, Pos.y, cx, cy, Rate, 0.0, currentImg, true, true);
		}
		else {
			DrawRotaGraph2(drawX, Pos.y, cx, cy, Rate, 0.0, currentImg, true, false);
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
	if (isGroundPounded) { isGroundPounded = false; return true; }
	return false;
}

bool Enemy::CheckAndResetFireFlag(void) {
	if (isFireBreathing) {
		isFireBreathing = false; // フラグをリセット！
		return true;
	}
	return false;
}

bool Enemy::CheckResetStoneFlag() {
	if (isStoneRaining) { isStoneRaining = false; return true; }
	return false;
}

float Enemy::GetEnemyRadius(void) {
	return 220.0f;
}