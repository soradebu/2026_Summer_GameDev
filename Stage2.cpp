#include  <DxLib.h>
#include "InputManager.h"
#include "EnemyMino.h"
#include "Knife.h"
#include "Stage2.h"
#include "Application.h"
#include "Player.h"



Stage2::Stage2(void)
{
	img = -1;

	pauseimg = -1;

	pl = -1;

	bgm = -1;
	player = nullptr;
	mino = nullptr;
	knife = nullptr;
}

Stage2::~Stage2(void)
{

}

//初期化処理(最初の一回のみ実行)
bool Stage2::SystemInit(void)
{
	player = new Player(this);
	if (player == nullptr)return false;

	mino = new EnemyMino(this);
	if (mino == nullptr)return false;

	knife = new Knife(this);
	if (knife == nullptr)return false;

	if (player->SystemInit() == false)return false;

	if (mino->SystemInit() == false)return false;

	if (knife->SystemInit() == false)return false;

	// ゲーム背景画像の読み込み
	img = LoadGraph("image/Stage1.jpg");
	if (img == -1)return false;
	
	pauseimg = LoadGraph("image/Pause.png");
	if (pauseimg == -1)return false;

	pl = LoadGraph("image/play_2.png");
	if (pl == -1)return false;

	sound = LoadSoundMem("sound/Stone.mp3");
	if (sound == -1)return false;

	sound1 = LoadSoundMem("sound/doragon.mp3");
	if (sound1 == -1)return false;

	bgm = LoadSoundMem("sound/stage.mp3");
	if (bgm == -1)return false;

	attackSE = LoadSoundMem("sound/damage.mp3");
	if (attackSE == -1)return false;

	hidanSE = LoadSoundMem("sound/hidan.mp3");
	if (hidanSE == -1)return false;

	hidanSE1 = LoadSoundMem("sound/hidan1.mp3");
	if (hidanSE1 == -1)return false;

	zimenSE = LoadSoundMem("sound/zimen.mp3");
	if (zimenSE == -1)return false;

	return true;

}

//ゲーム起動・再開時に必ず呼び出す処理
void Stage2::GameInit(void)
{
	player->GameInit();
	mino->GameInit();
	knife->GameInit();

	enCounter = 0;
	prevShotKey = nowShotKey = 0;
	knifeDelayTimer = 0;
	scrollX = 0;
	pauseMenuCursor = 0;

	sState = SceneState::PLAYING; // 最初は普通に遊べる状態

	nextSceneID = E_SCENE_STAGE2;

	StopSoundMem(bgm);

	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, true);

}

//更新処理
void Stage2::Update(void)
{
	static int prevPadInput = 0;

	InputManager& inputIns = InputManager::GetInstance();

	InputManager::JOYPAD_IN_STATE state =
		inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	padInput = GetJoypadInputState(DX_INPUT_PAD1);

	isPadBtnPressed1 = ((padInput & PAD_INPUT_1) && !(prevPadInput & PAD_INPUT_1));
	isPadBtnPressed2 = ((padInput & PAD_INPUT_8) && !(prevPadInput & PAD_INPUT_8));

	prevPadInput = padInput;

	// 左アナログキーのX値
	int analogKeyY = state.AKeyLY;

	int analogKeyZ = 0;

	XINPUT_STATE xinputState;
	if (GetJoypadXInputState(DX_INPUT_PAD1, &xinputState) == 0)
	{
		analogKeyZ = xinputState.RightTrigger;
	}

	bool nowStickUp = (analogKeyY < 0);
	bool nowStickDown = (analogKeyY > 0);
	bool nowRtPressed = (analogKeyZ > 128);

	prevUpkey = nowUpkey;
	nowUpkey = CheckHitKey(KEY_INPUT_UP);

	prevDownkey = nowDownkey;
	nowDownkey = CheckHitKey(KEY_INPUT_DOWN);

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	prevEscapeKey = nowEscapeKey;
	nowEscapeKey = CheckHitKey(KEY_INPUT_ESCAPE);

	static bool prevStickUp = false;
	static bool prevStickDown = false;
	static bool prevRtPressed = false;

	bool isPadRtPressed = (nowRtPressed && !prevRtPressed); // 押した瞬間だけ true
	prevRtPressed = nowRtPressed;

	bool stickUpReleased = (prevStickUp && !nowStickUp);
	bool stickDownReleased = (prevStickDown && !nowStickDown);

	prevStickUp = nowStickUp;		// 現在の位置保存
	prevStickDown = nowStickDown;	// 現在の位置保存


	if (sState == SceneState::PAUSE)
	{
		// ESCAPEキーでプレイに戻る
		if ((prevEscapeKey == 0 && nowEscapeKey == 1) || isPadBtnPressed2)
		{
			sState = SceneState::PLAYING;
			return;
		}

		if ((prevUpkey == 0 && nowUpkey == 1) || stickUpReleased)
		{
			pauseMenuCursor = 0; // RETRYを選択
		}

		if ((prevDownkey == 0 && nowDownkey == 1) || stickDownReleased)
		{
			pauseMenuCursor = 1; // EXITを選択
		}

		// スペースキーで決定
		if ((prevSpaceKey == 0 && nowSpaceKey == 1) || isPadBtnPressed1)
		{
			if (pauseMenuCursor == 0)
			{
				// 今のシーンを最初からやり直す
				this->GameInit();
				sState = SceneState::PLAYING; // プレイ状態に戻す
				return;
			}
			else if (pauseMenuCursor == 1)
			{
				// EXITが選ばれた時タイトルシーン（TITLE）へ遷移
				nextSceneID = E_SCENE_TITLE;
				return;
			}
		}

		return;
	}

	if (sState == SceneState::PLAYING)
	{
		player->Update();
		mino->Update();

		if (prevEscapeKey == 0 && nowEscapeKey == 1 || isPadBtnPressed2)
		{
			sState = SceneState::PAUSE;
			pauseMenuCursor = 0;
			return;
		}
	}

	if (knifeDelayTimer > 59)
	{
		knifeDelayTimer = 0;
	}

	if (knife->GetCutFlg() == false && knifeDelayTimer == 0)
	{
		prevShotKey = nowShotKey;
		nowShotKey = CheckHitKey(KEY_INPUT_G);

		if (prevShotKey == 0 && nowShotKey == 1 || isPadRtPressed)
		{
			if (player != nullptr && player->GetIsJumping() == false)
			{
				// プレイヤーの向きを取得
				bool isRight = (player->GetPlayerDir() == static_cast<int>(AsoUtility::DIR::RIGHT));

				if (isRight)
				{
					Vector2 pPos = player->GetPlayerPos();
					pPos.x += 32;
					pPos.y -= 32;

					knife->KnifeCreate(pPos);
					knifeDelayTimer = KNIFE_DELAY_TIME;
				}

			}
		}
	}

	if (player != nullptr && player->GetAlive() <= 0)
	{
		StopSoundMem(bgm);
		nextSceneID = E_SCENE_GAMEOVER;
	}

	if (mino != nullptr && mino->GetAlive() == false)
	{
		StopSoundMem(bgm);
		nextSceneID = E_SCENE_GAMECLEAR;
	}

	Collision();
}


// 描画処理
void Stage2::Draw(void)
{
	int haikeiPosX = (Application::SCREEN_SIZE_WID - HAIKEI_WID) / 2;

	int haikeiPosY = (Application::SCREEN_SIZE_HIG - HAIKEI_HIG) / 2;

	// 描画背景
	DrawGraph(0, 0, img, true);

	player->Draw();

	mino->Draw();

	knife->Draw();

	int hx = 10;
	int hy = -30;

	int currentHp = player->GetHP();

	int currentenemyHp = mino->GetHP();

#if 0

	if (currentHp > 0)
	{
		int imgIndex = currentHp - 1;

		if (imgIndex >= 0 && imgIndex < 9)
		{
			DrawGraph(hx, hy, hp_img[imgIndex], true);
		}
	}
	else
	{
		DrawGraph(hx, hy, hp_img[8], true);
	}

#endif // 0


	if (currentHp > 0)
	{
		int pBarWidth = 400;                                 // プレイヤーHPバーの横幅
		int pBarHeight = 40;                                 // プレイヤーHPバーの縦幅
		int pBarX = 50;                                     
		int pBarY = 980;                                    

		//現在のHPの割合から、残りHPバーの長さを計算
		int redBarWidth = static_cast<int>(pBarWidth * (static_cast<float>(currentHp) / 8));

		//外側の白
		DrawBox(pBarX - 3, pBarY - 3, pBarX + pBarWidth + 3, pBarY + pBarHeight + 3, GetColor(255, 255, 255), true);

		//内側の黒
		DrawBox(pBarX, pBarY, pBarX + pBarWidth, pBarY + pBarHeight, GetColor(255, 0, 0), true);

		DrawBox(pBarX, pBarY, pBarX + redBarWidth, pBarY + pBarHeight, GetColor(76, 175, 50), true);

		DrawString(pBarX, pBarY - 20, "PLAYER", GetColor(255, 255, 255));
	}


	if (currentenemyHp > 0)
	{
		int barWidth = 1000;													// バーの横幅
		int barHeight = 50;														// バーの縦幅
		int barX = (Application::SCREEN_SIZE_WID - barWidth) / 2 + 400;		    
		int barY = 80;															

		//現在のHPの割合から、緑バーの長さ計算
		int greenBarWidth = static_cast<int>(barWidth * (static_cast<float>(currentenemyHp) / 20));

		//外側の白
		DrawBox(barX - 4, barY - 4, barX + barWidth + 4, barY + barHeight + 4, GetColor(255, 255, 255), true);

		//内側の赤
		DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(255, 0, 0), true);

		DrawBox(barX, barY, barX + greenBarWidth, barY + barHeight, GetColor(76, 175, 50), true);

		DrawString(barX, barY - 22, "BOSS", GetColor(255, 255, 255));
	}

	if (sState == SceneState::PAUSE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
		DrawBox(0, 0, Application::SCREEN_SIZE_WID, Application::SCREEN_SIZE_HIG, GetColor(0, 0, 0), true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		int menuX = (Application::SCREEN_SIZE_WID - PAUSE_WID) / 2;
		int menuY = (Application::SCREEN_SIZE_HIG - PAUSE_HIG) / 2;

		DrawGraph(menuX, menuY, pauseimg, true);

		int arrowX = menuX - 15;
		int arrowY = 0;

		if (pauseMenuCursor == 0)
		{
			arrowY = menuY + 550;
		}
		else if (pauseMenuCursor == 1)
		{
			arrowY = menuY + 750;
		}

		// 矢印の描画
		DrawGraph(arrowX, arrowY, pl, true);
	}

}

//開放処理
bool Stage2::Release(void)
{
	if (DeleteGraph(img) == -1)return false;


	if (sound != -1)
	{
		DeleteSoundMem(sound);
	}

	if (sound1 != -1)
	{
		DeleteSoundMem(sound1);
	}

	if (bgm != -1)
	{
		if (DeleteSoundMem(bgm) == -1) return false;
	}

	if (attackSE != -1)
	{
		if (DeleteSoundMem(attackSE) == -1) return false;
	}



	if (hidanSE != -1)
	{
		if (DeleteSoundMem(hidanSE) == -1) return false;
	}



	if (hidanSE1 != -1)
	{
		if (DeleteSoundMem(hidanSE1) == -1) return false;
	}


	player->Release();
	delete player;
	player = nullptr;

	mino->Release();
	delete mino;
	mino = nullptr;

	knife->Release();
	delete knife;
	knife = nullptr;

	return true;
}

void Stage2::Collision(void)
{
	if (player->GetIsKnockback() == true)
	{
		return;
	}

	Vector2 pPos = player->GetPlayerPos();
	Vector2 ePos = mino->GetEnemyPos();

	float diffX = pPos.x - ePos.x;
	float diffY = pPos.y - ePos.y;

	if (abs(diffX) < 100 && abs(diffY) < 300)
	{
		player->SetDamage(1, Player::state::STONE);

		if (mino->currentstate)
		{

		}

		// 吹き飛ぶ方向を決定（敵より右にいれば右へ、左にいれば左へ)
		float knockbackDir = (diffX > 0) ? 3.0f : -3.0f;

		// プレイヤーのノックバック関数を呼び出す
		player->TriggerKnockback(knockbackDir);
	}
}

