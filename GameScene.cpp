#include  <DxLib.h>
#include"InputManager.h"
#include"Enemy.h"
#include"Knife.h"
#include"Fire.h"
#include "GameScene.h"
#include "Application.h"
#include "Player.h"
#include"Stone.h"


GameScene::GameScene(void)
{
	img = -1;

	bgm = -1;
	player = nullptr;
	enemy = nullptr;
	knife = nullptr;
	fire = nullptr;
}

GameScene::~GameScene(void)
{

}

//初期化処理(最初の一回のみ実行)
bool GameScene::SystemInit(void)
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
	img = LoadGraph("image/mori1.jpg");
	if (img == -1)return false;

	sound = LoadSoundMem("sound/Stone.mp3");
	if (sound == -1)return false;

	sound1 = LoadSoundMem("sound/doragon.mp3");
	if (sound1 == -1)return false;

	bgm = LoadSoundMem("sound/stage.mp3");
	if (bgm == -1)return false;

	attackSE = LoadSoundMem("sound/damege.mp3");
	if (attackSE == -1)return false;

	hidanSE = LoadSoundMem("sound/hidan.mp3");
	if (attackSE == -1)return false;

	hidanSE1 = LoadSoundMem("sound/hidan1.mp3");
	if (attackSE == -1)return false;

	hp_img[0] = LoadGraph("image/HP/hp_1.png");
	hp_img[1] = LoadGraph("image/HP/hp_2.png");
	hp_img[2] = LoadGraph("image/HP/hp_3.png");
	hp_img[3] = LoadGraph("image/HP/hp_4.png");
	hp_img[4] = LoadGraph("image/HP/hp_5.png");
	hp_img[5] = LoadGraph("image/HP/hp_6.png");
	hp_img[6] = LoadGraph("image/HP/hp_7.png");
	hp_img[7] = LoadGraph("image/HP/hp_8.png");

	hp_img[8] = LoadGraph("image/HP/hp_0.png");

	for (int i = 8; i > 0; i--)
	{
		if (hp_img[i] == -1)
		{
			return false;
		}
	}


	return true;

}

//ゲーム起動・再開時に必ず呼び出す処理
void GameScene::GameInit(void)
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
	nextSceneID = E_SCENE_GAME;

	StopSoundMem(bgm);

	PlaySoundMem(bgm, DX_PLAYTYPE_BACK, true);

}
//更新処理
void GameScene::Update(void)
{
	InputManager& inputIns = InputManager::GetInstance();

	InputManager::JOYPAD_IN_STATE state =
		inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	padInput = GetJoypadInputState(DX_INPUT_PAD1);

	isPadBtnPressed = (padInput & PAD_INPUT_6);

	player->Update();
	enemy->Update();
	fire->Update();

	if (knife != nullptr && player != nullptr)
	{
		knife->Update(player->GetPlayerPos());
	}

	for (int i = 0; i < STONE_MAX; i++)
	{
		stones[i]->Update();
	}

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);
	// 背景を左へ動かす
	scrollX -= HAIKEI_MOVE_SPEED;

	if (scrollX <= -HAIKEI_WID)
	{
		scrollX = 0;
	}

	//-----------------------------------------
	// 敵の叩きつけを検知したら連続落下モードを起動
	//-----------------------------------------

	if (enemy != nullptr)
	{
		if (enemy->CheckAndResetPoundFlag())
		{
			// すでに落下中のときは重ならないように安全弁をかける
			if (!isSpawnActive)
			{
				isSpawnActive = true; // 時間差落下ルーチン開始！
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
			fire->activate(enemy->GetEnemyPos().x - 50.0f, enemy->GetEnemyPos().y + 50.0);

			PlaySoundMem(sound1, DX_PLAYTYPE_BACK);

		}
	}


	//-----------------------------------------
	//数フレームごとに石を1個ずつ落とす
	//-----------------------------------------
	if (isSpawnActive)
	{
		spawnTimer++;

		// 最初（1フレーム目）または、12フレーム（約200ミリ秒）経つごとに石を落とす
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

	if (knifeDelayTimer > 59)
	{
		knifeDelayTimer = 0;
	}

	if (knife->GetCutFlg() == false && knifeDelayTimer == 0)
	{
		prevShotKey = nowShotKey;
		nowShotKey = CheckHitKey(KEY_INPUT_G);

		if (prevShotKey == 0 && nowShotKey == 1 || isPadBtnPressed) {
			// プレイヤーの向きを取得（前回の正面限定の処理）
			bool isRight = (player->GetPlayerDir() == static_cast<int>(AsoUtility::DIR::RIGHT));

			if (isRight)
			{
				Vector2 pPos = player->GetPlayerPos();
				pPos.y -= Player::PLAYER_WID;
				knife->KnifeCreate(pPos);

				// 剣を振ったので、ディレイタイマーをセット！
				knifeDelayTimer = KNIFE_DELAY_TIME;
			}
		}
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
}


//描画処理
void GameScene::Draw(void)
{
	int haikeiPosX = (Application::SCREEN_SIZE_WID - HAIKEI_WID) / 2;

	int haikeiPosY = (Application::SCREEN_SIZE_HIG - HAIKEI_HIG) / 2;

	// 1枚目の背景
	DrawGraph(haikeiPosX, haikeiPosY, img, true);

	player->Draw();

	enemy->Draw();

	knife->Draw();

	fire->Draw();

	for (int i = 0; i < STONE_MAX;i++)
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
		// 2. 画面の左上に配置するサイズと座標
		int pBarWidth = 400;                                 // プレイヤーHPバーの横幅（ボスの半分くらい）
		int pBarHeight = 40;                                 // プレイヤーHPバーの縦幅（少しスマートに）
		int pBarX = 50;                                      // 画面左端から20ピクセル右
		int pBarY = 980;                                      // 画面上端から40ピクセル下

		// 3. 現在のHPの割合から、残りHPバーの長さを計算
		int redBarWidth = static_cast<int>(pBarWidth * (static_cast<float>(currentHp) / 8));

		// 4. 外側の白枠
		DrawBox(pBarX - 3, pBarY - 3, pBarX + pBarWidth + 1, pBarY + pBarHeight + 3, GetColor(255, 255, 255), true);

		// 5. 内側の黒背
		DrawBox(pBarX, pBarY, pBarX + pBarWidth, pBarY + pBarHeight, GetColor(255, 0, 0), true);

		DrawBox(pBarX, pBarY, pBarX + redBarWidth, pBarY + pBarHeight, GetColor(76, 175, 50), true);

		DrawString(pBarX, pBarY - 20, "PLAYER", GetColor(255, 255, 255));
	}


	if (currentenemyHp > 0)
	{
		// 2. バーの表示位置とサイズ
		int barWidth = 1000;														// バー全体の横幅
		int barHeight = 50;														// バーの縦幅
		int barX = (Application::SCREEN_SIZE_WID - barWidth) / 2 + 400;		// 画面中央のX座標
		int barY = 80;															// 画面上端からのY座標

		// 現在のHPの割合から、緑色のバーの長さを計算
		int greenBarWidth = static_cast<int>(barWidth * (static_cast<float>(currentenemyHp) / 20));

		// 外側の白枠
		DrawBox(barX - 4, barY - 4, barX + barWidth + 4, barY + barHeight + 4, GetColor(255, 255, 255), true);

		// 内側の赤
		DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(255, 0, 0), true);

		DrawBox(barX, barY, barX + greenBarWidth, barY + barHeight, GetColor(76, 175, 50), true);

		DrawString(barX, barY - 22, "BOSS", GetColor(255, 255, 255));
	}
}

//開放処理（最後の一回のみ実行）
bool GameScene::Release(void)
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

void GameScene::Collision(void)
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

	// すべての石に対して、敵かプレイヤーの「どちらか片方だけ」に当たるようにループを綺麗にまとめる
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
				enemy->SetDamage(1);  // 敵にダメージ
				stones[i]->GameInit(); // 石を消す


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
				if (player->GetInvincibleTime() > 0.0f)
				{
					// 無敵時間中は何もせずスキップ
					continue;
				}

				player->SetDamage(1);
				player->SetInvincible(1.0f);
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

			player->SetDamage(2);
			player->SetInvincible(1.0f);
			PlaySoundMem(hidanSE1, DX_PLAYTYPE_BACK);

			fire->GameInit(); // 炎を消す処理
		}
	}
}