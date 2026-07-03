#include <string>
#include <DxLib.h>
#include "Application.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "AsoUtility.h"
#include "Vector2.h"
#include "Enemy.h"
#include "Player.h"

Player::Player(SceneBase* scene)
{
	m_pScene = scene;
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

	// 岩ダメージ画像の読み込み
	player_hit_stone = LoadGraph("image/Damage_A.png");
	if (player_hit_stone == -1) return false;

	// 火ダメージ画像の読み込み
	player_hit_fire = LoadGraph("image/Damage_B.png");
	if (player_hit_fire == -1) return false;

	for (int i = 0; i < 6; i++)
	{
		// 待機アニメーション
		playerImages[static_cast<int>(state::IDLE)][i] = DerivationGraph(i * 100, 0, 100, 100, player_img);

		// 走行アニメーション
		playerImages[static_cast<int>(state::RUN)][i] = DerivationGraph(i * 100, 0, 100, 100, player_run);

		// ジャンプアニメーション
		playerImages[static_cast<int>(state::JUMP)][i] = DerivationGraph(i * 100, 0, 100, 100, player_jump);

		// 岩ダメージアニメーション
		playerImages[static_cast<int>(state::STONE)][i] = DerivationGraph(i * 100, 0, 100, 100, player_hit_stone);

		// ダメージアニメーション
		playerImages[static_cast<int>(state::FIRE)][i] = DerivationGraph(i * 100, 0, 100, 100, player_hit_fire);
	}

	for (int i = 0; i < 3; i++)
	{
		// 岩ダメージアニメーション
		playerImages[static_cast<int>(state::STONE)][i] = DerivationGraph(i * 100, 0, 100, 100, player_hit_stone);
	}

	for (int i = 0; i < 5; i++)
	{
		// ダメージアニメーション
		playerImages[static_cast<int>(state::FIRE)][i] = DerivationGraph(i * 100, 0, 100, 100, player_hit_fire);
	}

	return true;
}

void Player::GameInit(void)
{
	pPos.x = PLAYER_WID;
	pPos.y = PLAYER_HIG + 750;

	isKnockback = false;   

	knockbackTimer = 0;     

	vx = 0.0f;            // プレイヤーの現在のX速度
	vy = 0.0f;			  // プレイヤーの現在のY速度

	jumpSpeed = 0.0f;
	gravity = 0.8f;
	isJumping = false;

	animCounter = 0;
	frame = 0;
	aliveFlg = true;
	hp = 8;

	damageTimer = 0;

	currentstate = state::IDLE;
}

void Player::Update(void)
{
	if (invincibleTime > 0.0f) 
	{
		invincibleTime -= (1.0f / 60.0f);
		if (invincibleTime < 0.0f) invincibleTime = 0.0f;
	}

	animCounter++;
	if (animCounter >= 60) animCounter = 0;

	if (damageTimer > 0)
	{
		damageTimer--;
		if (damageTimer == 0)
		{
			currentstate = state::IDLE; // タイマーが切れたら通常状態に戻る
		}
		return; 
	}

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
		pPos.x -= MOVE_SPEED;
		if (pPos.x < (PLAYER_WID / 5))pPos.x = PLAYER_WID / 5;
		playerDir = static_cast<int>(AsoUtility::DIR::LEFT);

		currentstate = state::RUN;
	}

	if (CheckHitKey(KEY_INPUT_D)
		|| analogKeyX > 0)
	{
		pPos.x += MOVE_SPEED;
		if (pPos.x > 1800 )pPos.x = 1800;
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
		pPos.y += jumpSpeed;
		jumpSpeed += gravity;


		// 地面に戻ってきたか判定
		float groundY = PLAYER_HIG + 750;
		if (pPos.y >= groundY)
		{
			pPos.y = groundY; // 地面にめり込まないように位置調整
			isJumping = false;     // 着地
			jumpSpeed = 0.0f;
		}
	}

	if (isKnockback)
	{
		pPos.x += vx;
		pPos.y += vy; 

		vx *= 0.95f;

		knockbackTimer--;

		if (knockbackTimer <= 0)
		{
			isKnockback = false;
			vx = 0.0f;
			vy = 0.0f;
		}
		return;
	}
}

void Player::Draw(void)
{
	int animNo;

	if (currentstate == state::IDLE)
	{
		animNo = 0;
	}
	else if (currentstate == state::STONE)
	{
		animNo = (animCounter / ANIM_INTERVAL) % 2;
	}
	else if (currentstate == state::JUMP)
	{
		animNo = (animCounter / ANIM_INTERVAL) % 3;
	}
	else if (currentstate == state::FIRE)
	{
		//ダメージアニメーションは6コマでパラパラさせる
		animNo = (animCounter / ANIM_INTERVAL) % 4;
	}
	else
	{
		animNo = (animCounter / ANIM_INTERVAL) % 6;
	}

	// 現在の画像ハンドルを取得
	int currentHandle = playerImages[static_cast<int>(currentstate)][animNo];


	if (damageTimer > 0)
	{
		if ((damageTimer % 4) < 2) return; // 2フレームに1回、描画をスキップして点滅
	}

	if (playerDir == static_cast<int>(AsoUtility::DIR::LEFT))
	{
		//左右反転して描画する
		DrawTurnGraph(pPos.x, pPos.y, currentHandle, true);
	}
	else
	{
		// 右向き時（RIGHT）通常の向きで描画する
		DrawGraph(pPos.x, pPos.y, currentHandle, true);
	}


}

bool Player::Release(void)
{
	if (sound != -1) DeleteSoundMem(sound);

	if (player_img != -1) DeleteGraph(player_img);

	if (player_run != -1) DeleteGraph(player_run);

	if (player_jump != -1) DeleteGraph(player_jump);

	if (player_hit_stone != -1) DeleteGraph(player_hit_stone);

	if (player_hit_fire != -1) DeleteGraph(player_hit_fire);   

	return true;
}

// プレイヤーにダメージを与える
// Input:
//    int dp : ダメージ
// Ountput:
//    無し
void Player::SetDamage(int dp, state damageState)
{
	invincibleTime = 1.0f;

	hp -= dp;
	if (hp <= 0)
	{
		hp = 0;
		aliveFlg = false;
	}
	else
	{
		damageTimer = 30;
		currentstate = damageState;
		animCounter = 0;    
	}
}

void Player::TriggerKnockback(float dir)
{
	if (isKnockback) return; // 連続で当たり判定が起きない

	isKnockback = true;
	knockbackTimer = 20; 

	vx = dir * 8.0f;    
	vy = -4.0f;
}
