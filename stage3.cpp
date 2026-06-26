#include  <DxLib.h>
#include "InputManager.h"
#include "Enemy.h"
#include "Knife.h"
#include "Fire.h"
#include "Stage3.h"
#include "Application.h"
#include "Player.h"
#include "Stone.h"

Stage3::Stage3(void)
{
	img = -1;

	pauseimg = -1;

	pl = -1;

	bgm = -1;
	player = nullptr;
	enemy = nullptr;
	knife = nullptr;
	fire = nullptr;
}

Stage3::~Stage3(void)
{

}

//初期化処理(最初の一回のみ実行)
bool Stage3::SystemInit(void)
{
	player = new Player(this);
	if (player == nullptr)return false;

	enemy = new Enemy(this);
	if (enemy == nullptr)return false;

	knife = new Knife(this);
	if (knife == nullptr)return false;

	fire = new Fire(this);
	if (fire == nullptr)return false;

	for (int i = 0; i < STONE_MAX; i++) {
		stones[i] = new Stone(this);
	}

	if (stones == nullptr)return false;

	if (player->SystemInit() == false)return false;

	if (enemy->SystemInit() == false)return false;

	if (knife->SystemInit() == false)return false;

	if (fire->SystemInit() == false)return false;

	for (int i = 0; i < STONE_MAX; i++)
	{
		if (stones[i]->SystemInit() == false)
		{
			return false; // 1個でも初期化に失敗したらfalseを返す
		}
	}

	// ゲーム背景画像の読み込み
	img = LoadGraph("image/doraemon.png");
	if (img == -1)return false;

	effectImg = LoadGraph("image/effect.png");
	if (effectImg == -1)return false;

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
void Stage3::GameInit(void)
{
	player->GameInit();
	enemy->GameInit();
	knife->GameInit();
	fire->GameInit();



	for (int i = 0; i < STONE_MAX; i++)
	{
		if (stones[i] != nullptr)
		{
			stones[i]->GameInit();   // 画面でエラーになっていた関数
		}
	}


	enCounter = 0;
	prevShotKey = nowShotKey = 0;
	knifeDelayTimer = 0;
	scrollX = 0;
	pauseMenuCursor = 0;

	sState = SceneState::PLAYING; // 最初は普通に遊べる状態

	nextSceneID = E_SCENE_STAGE3;

	StopSoundMem(bgm);

	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, true);

}

//更新処理
void Stage3::Update(void)
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

	if (knife != nullptr && player != nullptr)
	{
		knife->Update(player->GetPlayerPos());
	}

	for (int i = 0; i < STONE_MAX; i++)
	{
		stones[i]->Update();
	}

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
		enemy->Update();
		fire->Update();


		if (prevEscapeKey == 0 && nowEscapeKey == 1 || isPadBtnPressed2)
		{
			sState = SceneState::PAUSE;
			pauseMenuCursor = 0;
			return;
		}

	}

	//-----------------------------------------
	//数フレームごとに石を1個ずつ落とす
	//-----------------------------------------
	if (isSpawnActive)
	{
		spawnTimer++;

		// 最初（1フレーム目）または、12フレーム経つごとに石を落とす
		// ※ 12 の数字を小さくするとさらに高速でパラパラ落ちてきます
		if (spawnedCount == 0 || spawnTimer >= 12)
		{
			// まだ落とすべき石が残っていれば
			if (spawnedCount < STONE_MAX)
			{
				if (stones[spawnedCount] != nullptr)
				{
					// ランダムな位置から落とす
					float spawnX = (float)(120 + rand() % 1081);
					stones[spawnedCount]->activate(spawnX, 0.0f);
				}

				spawnedCount++; // 落とした数をカウントアップ
				spawnTimer = 0; // 次の石へのタイマーをリセット
			}
		}

		// ５個をすべて落とし終えたらルーチンを終了
		if (spawnedCount >= STONE_MAX)
		{
			isSpawnActive = false;
		}

	}

	//-----------------------------------------
	// 敵の叩きつけを検知したら連続落下モードを起動
	//-----------------------------------------

	if (enemy != nullptr)
	{
		if (enemy->CheckAndResetPoundFlag())
		{
			PlaySoundMem(zimenSE, DX_PLAYTYPE_BACK);

			// すでに落下中のときは重ならないようにする
			if (!isSpawnActive)
			{
				isSpawnActive = true;        // 時間差落下開始
				spawnTimer = 0;              // タイマーリセット
				spawnedCount = 0;            // 落としたカウントをゼロに
			}
		}
	}

	//敵が吠えるのを確認したら火を出す
	if (enemy != nullptr)
	{
		// 火の処理
		if (enemy->CheckAndResetFireFlag()) {
			fire->Activate(enemy->GetEnemyPos().x - 50.0f, enemy->GetEnemyPos().y + 50.0f);

			PlaySoundMem(sound1, DX_PLAYTYPE_BACK);

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

	if (hitEffect.active) {
		hitEffect.timer--;
		if (hitEffect.timer <= 0) hitEffect.active = false;
	}

	Collision();

	if (player != nullptr && player->GetAlive() <= 0)
	{
		StopSoundMem(bgm);
		nextSceneID = E_SCENE_GAMEOVER;
	}

	if (enemy != nullptr && enemy->GetAlive() == false)
	{
		StopSoundMem(bgm);
		nextSceneID = E_SCENE_GAMECLEAR;
	}

	if (hitEffect.active) {
		hitEffect.timer--;
		if (hitEffect.timer <= 0) hitEffect.active = false;
	}

}


//描画処理
void Stage3::Draw(void)
{
	int haikeiPosX = (Application::SCREEN_SIZE_WID - HAIKEI_WID) / 2;

	int haikeiPosY = (Application::SCREEN_SIZE_HIG - HAIKEI_HIG) / 2;

	// 1枚目の背景
	DrawGraph(haikeiPosX, haikeiPosY, img, true);

	player->Draw();

	enemy->Draw();

	knife->Draw();

	fire->Draw();

	for (int i = 0; i < STONE_MAX; i++)
	{
		stones[i]->Draw();
	}

	int hx = 10;
	int hy = -30;

	int currentHp = player->GetHP();

	int currentenemyHp = enemy->GetHP();

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
		//画面の左上に配置するサイズと座標
		int pBarWidth = 400;                                 // プレイヤーHPバーの横幅（ボスの半分くらい）
		int pBarHeight = 40;                                 // プレイヤーHPバーの縦幅（少しスマートに）
		int pBarX = 50;                                      // 画面左端から20ピクセル右
		int pBarY = 980;                                     // 画面上端から40ピクセル下

		//現在のHPの割合から、残りHPバーの長さを計算
		int redBarWidth = static_cast<int>(pBarWidth * (static_cast<float>(currentHp) / 8));

		//外側の白枠
		DrawBox(pBarX - 3, pBarY - 3, pBarX + pBarWidth + 3, pBarY + pBarHeight + 3, GetColor(255, 255, 255), true);

		//内側の黒背
		DrawBox(pBarX, pBarY, pBarX + pBarWidth, pBarY + pBarHeight, GetColor(255, 0, 0), true);

		DrawBox(pBarX, pBarY, pBarX + redBarWidth, pBarY + pBarHeight, GetColor(76, 175, 50), true);

		DrawString(pBarX, pBarY - 20, "PLAYER", GetColor(255, 255, 255));
	}


	if (currentenemyHp > 0)
	{
		//バーの表示位置とサイズ
		int barWidth = 1000;													// バー全体の横幅
		int barHeight = 50;														// バーの縦幅
		int barX = (Application::SCREEN_SIZE_WID - barWidth) / 2 + 400;		    // 画面中央のX座標
		int barY = 80;															// 画面上端からのY座標

		//現在のHPの割合から、緑色のバーの長さを計算
		int greenBarWidth = static_cast<int>(barWidth * (static_cast<float>(currentenemyHp) / 20));

		//外側の白枠
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

	if (hitEffect.active) {
		// 透過度やサイズをタイマーに合わせて調整
		DrawGraph(hitEffect.x - 32, hitEffect.y - 32, effectImg, true);
	}

}

//開放処理
bool Stage3::Release(void)
{
	if (DeleteGraph(img) == -1)return false;
	if (DeleteGraph(effectImg) == -1)return false;

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

	enemy->Release();
	delete enemy;
	enemy = nullptr;

	knife->Release();
	delete knife;
	knife = nullptr;

	fire->Release();
	delete fire;
	fire = nullptr;

	for (int i = 0; i < STONE_MAX; i++)
	{
		if (stones[i] != nullptr)
		{
			stones[i]->Release();
			delete stones[i];
			stones[i] = nullptr;
		}
	}

	return true;
}

void Stage3::Collision(void)
{
	Vector2 bpos;

	if (knife != nullptr && knife->GetCutFlg() == true)
	{
		Vector2 knifePos = knife->GetKnifePos();
		float knifeRadius = knife->GetRadius();

		for (int i = 0; i < STONE_MAX; i++)
		{
			if (stones[i] != nullptr && stones[i]->IsStoneActive())
			{
				Vector2 stonePos = stones[i]->GetStonePos();

				float diffX = stonePos.x - knifePos.x;
				float diffY = stonePos.y - knifePos.y;
				float distanceSq = (diffX * diffX) + (diffY * diffY);

				float combinedRadius = knifeRadius + stones[i]->GetStoneRadius();
				float combinedRadiusSq = combinedRadius * combinedRadius;

				if (distanceSq < combinedRadiusSq)
				{
					stones[i]->OnHit();
					PlaySoundMem(sound, DX_PLAYTYPE_BACK);
				}
			}
		}
	}

	// すべての石に対して、敵かプレイヤーのどちらか片方だけに当たるようにループを綺麗にまとめる
	for (int i = 0; i < STONE_MAX; i++)
	{
		if (stones[i] == nullptr || !stones[i]->IsStoneActive()) continue;

		Vector2 stonePos = stones[i]->GetStonePos();

		Vector2 firePos = fire->GetFirePos();

		if (enemy != nullptr && enemy->GetAlive())
		{
			Vector2 enemyPos = enemy->GetEnemyPos();
			float enemyRadius = 180.0f;

			float diffX = stonePos.x - enemyPos.x;
			float diffY = stonePos.y - enemyPos.y;
			float distanceSq = (diffX * diffX) + (diffY * diffY);

			float combinedRadius = enemyRadius + stones[i]->GetStoneRadius();
			float combinedRadiusSq = combinedRadius * combinedRadius;

			if (distanceSq < combinedRadiusSq)
			{
				hitEffect.x = stonePos.x;
				hitEffect.y = stonePos.y;
				hitEffect.timer = 10; // 10フレームで消える設定
				hitEffect.active = true;

				enemy->SetDamage(1);	// 敵にダメージ
				stones[i]->GameInit();  // 石を消す

				PlaySoundMem(attackSE, DX_PLAYTYPE_BACK);

				continue;              // この石は処理終了、次の石のループへ！

			}
		}

		if (player != nullptr && player->GetHP() > 0)
		{
			Vector2 playerPos = player->GetPlayerPos();
			float playerRadius = 37.0f;

			float diffX = stonePos.x - playerPos.x;
			float diffY = stonePos.y - playerPos.y;
			float distanceSq = (diffX * diffX) + (diffY * diffY);

			float combinedRadius = playerRadius + stones[i]->GetStoneRadius();
			float combinedRadiusSq = combinedRadius * combinedRadius;

			if (distanceSq < combinedRadiusSq)
			{
				if (player->GetDamageTimer() > 0)
				{
					continue;
				}

				if (player->GetInvincibleTime() > 0.0f)
				{
					// 無敵時間中は何もせずスキップ
					continue;
				}

				player->SetDamage(1, Player::state::STONE); // 岩ダメージ画像を再生

				stones[i]->GameInit(); // 石を消す

				PlaySoundMem(hidanSE, DX_PLAYTYPE_BACK);


				continue;           //この石は処理終了
			}
		}
	}

	if (fire != nullptr && fire->GetActiveFlg())
	{
		Vector2 firePos = fire->GetFirePos();
		Vector2 playerPos = player->GetPlayerPos();

		float diffX = firePos.x - playerPos.x;
		float diffY = firePos.y - playerPos.y;
		float distanceSq = (diffX * diffX) + (diffY * diffY);

		float combinedRadius = 100.0f + fire->GetFireRadius();

		if (distanceSq < (combinedRadius * combinedRadius))
		{

			if (player->GetInvincibleTime() > 0.0f)
			{
				// 無敵時間中は何もせずスキップ
				return;
			}

			if (player != nullptr && player->GetDamageTimer() == 0)
			{
				player->SetDamage(1, Player::state::FIRE); // 火ダメージ画像を再生

				PlaySoundMem(hidanSE1, DX_PLAYTYPE_BACK);

				fire->GameInit(); // 炎を消す処理
			}
		}
	}
}

