#include <string>
#include <DxLib.h>
#include "Application.h"
#include "SceneManager.h"
#include "AsoUtility.h"
#include "Vector2.h"
#include"Player.h"

Player::Player(GameScene* gs)
{
	gInst = gs;
}

Player::~Player(void)
{

}

bool Player::SystemInit(void)
{
	player_img = LoadGraph("image/sheet3.png");

	return true;
}

void Player::GameInit(void)
{
	playerPos.x = PLAYER_WID ;
	playerPos.y = PLAYER_HIG + 425;

	animCounter = 0;
	frame = 0;
	aliveFlg = true;
	hp = PLAYER_HP_MAX;

}

void Player::Update(void)
{
	animCounter++;
	if (animCounter > (ANIM_NUMS * ANIM_INTERVAL) * 1)animCounter = 0;
	
	for (int i = 0; i < 8; i++) {
		// DerivationGraph(切り出し開始X, 開始Y, 幅, 高さ, 元画像ハンドル)
		walkImages[i] = DerivationGraph(i * 25,10, 115, 200, player_img);
	}

	if (CheckHitKey(KEY_INPUT_A)) {
		playerPos.x -= MOVE_SPEED;
		if (playerPos.x < (PLAYER_WID / 5))playerPos.x = PLAYER_WID / 5;
		playerDir = static_cast<int>(AsoUtility::DIR::LEFT);
	}

	if (CheckHitKey(KEY_INPUT_D)) {
		playerPos.x += MOVE_SPEED;
		if (playerPos.x > (Application::SCREEN_SIZE_WID /2.5 - PLAYER_WID)) {
			playerPos.x = Application::SCREEN_SIZE_WID /2.5 - PLAYER_WID;
		}
		playerDir = static_cast<int>(AsoUtility::DIR::RIGHT);
	}
}

void Player::Draw(void)
{
	int animNo = (animCounter / ANIM_INTERVAL) % ANIM_NUMS;

	// アニメーション描画（30フレームごとにコマ送り）
	frame = (GetNowCount() / 100) % 2;
	DrawGraph(playerPos.x, playerPos.y, walkImages[frame], true);
}

bool Player::Release(void)
{
	for (int yy = 0;yy < static_cast<int>(AsoUtility::DIR::MAX);yy++) {
		for (int xx = 0; xx < ANIM_NUMS;xx++) {
			if (DeleteGraph(player_img) == -1)return false;
		}
	}

	return true;
}

// プレイヤーにダメージを与える
// Input:
//    int dp : ダメージ
// Ountput:
//    無し
void Player::SetDamage(int dp)
{
	hp -= dp;
	if (hp <= 0) {
		hp = 0;
		aliveFlg = false;
	}
}