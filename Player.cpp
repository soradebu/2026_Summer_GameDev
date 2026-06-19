#include <string>
#include <DxLib.h>
#include "Application.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "AsoUtility.h"
#include "Vector2.h"
#include "Enemy.h"
#include "Player.h"

Player::Player(GameScene* gs)
{
	gInst = gs;

}

Player::~Player(void)
{

}

bool Player::SystemInit(void)
{
	player_img = LoadGraph("image/idle.png");
	if (player_img == -1) return false;

	player_run = LoadGraph("image/Run.png");
	if (player_run == -1) return false;

	sound = LoadSoundMem("sound/jump.wav");
	if (sound == -1)return false;

	player_jump = LoadGraph("image/Jump.png");
	if (player_jump == -1) return false;

	for (int i = 0; i < 6; i++)
	{
		// 待機アニメーション
		playerImages[static_cast<int>(state::IDLE)][i] = DerivationGraph(i * 100, 0, 100, 100, player_img);

		// 走行アニメーション
		playerImages[static_cast<int>(state::RUN)][i] = DerivationGraph(i * 100, 0, 100, 100, player_run);

		// ジャンプアニメーション
		playerImages[static_cast<int>(state::JUMP)][i] = DerivationGraph(i * 100, 0, 100, 100, player_jump);
	}
	return true;
}

void Player::GameInit(void)
{
	playerPos.x = PLAYER_WID;
	playerPos.y = PLAYER_HIG + 720;

	jumpSpeed = 0.0f;
	gravity = 0.8f;
	isJumping = false;

	animCounter = 0;
	frame = 0;
	aliveFlg = true;
	hp = 8;

	currentstate = state::IDLE;
}

void Player::Update(void)
{
	if (invincibleTime > 0.0f) {
		invincibleTime -= (1.0f / 60.0f); 
		if (invincibleTime < 0.0f) invincibleTime = 0.0f;
	}

	animCounter++;

	if (animCounter >= 60) animCounter = 0;

	currentstate = state::IDLE;

	InputManager& inputIns = InputManager::GetInstance();

	InputManager::JOYPAD_IN_STATE state =
		inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	padInput = GetJoypadInputState(DX_INPUT_PAD1);

	isPadBtnPressed = (padInput & PAD_INPUT_1);

	// 左アナログキーのX値
	int analogKeyX = state.AKeyLX;

	if (CheckHitKey(KEY_INPUT_A)
		|| analogKeyX < 0)
	{
		playerPos.x -= MOVE_SPEED;
		if (playerPos.x < (PLAYER_WID / 5))playerPos.x = PLAYER_WID / 5;
		playerDir = static_cast<int>(AsoUtility::DIR::LEFT);

		currentstate = state::RUN;
	}

	if (CheckHitKey(KEY_INPUT_D)
		|| analogKeyX > 0)
	{
		playerPos.x += MOVE_SPEED;
		if (playerPos.x > (Application::SCREEN_SIZE_WID - 800))
		{
			playerPos.x = Application::SCREEN_SIZE_WID - 800;
		}
		playerDir = static_cast<int>(AsoUtility::DIR::RIGHT);

		currentstate = state::RUN;
	}

	if (!isJumping &&
		(CheckHitKey(KEY_INPUT_SPACE)
			|| isPadBtnPressed))
	{
		PlaySoundMem(sound, DX_PLAYTYPE_BACK);
		jumpSpeed = -18.0f; // 上方向への速度
		isJumping = true;
		
	}

	if (isJumping)
	{
		currentstate = state::JUMP;
		playerPos.y += jumpSpeed;
		jumpSpeed += gravity;


		// 地面に戻ってきたか判定
		float groundY = PLAYER_HIG + 720;
		if (playerPos.y >= groundY)
		{
			playerPos.y = groundY; // 地面にめり込まないように位置調整
			isJumping = false;     // 着地
			jumpSpeed = 0.0f;
		}
	}
}

void Player::Draw(void)
{
	bool isVisible = true;
	if (invincibleTime > 0.0f) {
		if (((int)(invincibleTime * 10)) % 2 == 0) {
			isVisible = false;
		}
	}

	// 非表示フラグが立っていたら描画をスキップ
	if (!isVisible) return;

	int animNo;

	if (currentstate == state::IDLE)
	{
		animNo = 0;
	}
	else if (currentstate == state::JUMP)
	{
		animNo = (animCounter / ANIM_INTERVAL) % 3;
	}
	else {
		animNo = (animCounter / ANIM_INTERVAL) % 6;
	}

	// 現在の画像ハンドルを取得
	int currentHandle = playerImages[static_cast<int>(currentstate)][animNo];

	if (playerDir == static_cast<int>(AsoUtility::DIR::LEFT))
	{
		//左右反転して描画する
		DrawTurnGraph(playerPos.x, playerPos.y, currentHandle, true);
	}
	else
	{
		// 右向き時（RIGHT）通常の向きで描画する
		DrawGraph(playerPos.x, playerPos.y, currentHandle, true);
	}


}

bool Player::Release(void)
{
	if (sound != -1)
	{
		DeleteSoundMem(sound);
	}

	if (player_img != -1) {
		if (DeleteGraph(player_img) == -1) return false;
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
	invincibleTime = 1.0f;

	hp -= dp;
	if (hp <= 0) {
		hp = 0;
		aliveFlg = false;
	}
}
