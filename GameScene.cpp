#include  <DxLib.h>
#include"Enemy.h"
#include"Knife.h"
#include"Bullet.h"
#include "GameScene.h"
#include "Application.h"
#include "Player.h"
#include"Stone.h"


GameScene::GameScene(void)
{
	img = -1;
	player = nullptr;
	enemy = nullptr;
	knife = nullptr;
	bullet = nullptr;
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

	bullet = new Bullet(this);
	if (bullet == nullptr)return false;

	for (int i = 0; i < STONE_MAX; i++) {
		stones[i] = new Stone(this);
	}

	if (stones == nullptr)return false;

	if (player->SystemInit() == false)return false;

	if (enemy->SystemInit() == false)return false;

	if (knife->SystemInit() == false)return false;

	if (bullet->SystemInit() == false)return false;

	for (int i = 0; i < STONE_MAX; i++)
	{
		if (stones[i]->SystemInit() == false)
		{
			return false; // 1個でも初期化に失敗したらfalseを返す
		}
	}

	// ゲーム背景画像の読み込み
	img = LoadGraph("image/morimozaiku.jpg");
	if (img == -1)return false;

	return true;

}

//ゲーム起動・再開時に必ず呼び出す処理
void GameScene::GameInit(void)
{
	player->GameInit();
	enemy-> GameInit();
	knife-> GameInit();
	bullet->GameInit();

	

	for (int i = 0; i < STONE_MAX; i++)
	{
		if (stones[i] != nullptr)
		{
			stones[i]->GameInit();   // 画面でエラーになっていた関数
		}
	}


	enCounter = 0;
	prevShotKey = nowShotKey = 0;

	scrollX = 0;
	nextSceneID = E_SCENE_GAME;

}
//更新処理
void GameScene::Update(void)
{
	player->Update();
	enemy->Update();
	bullet->Update();

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
	// 2敵の叩きつけを検知したら連続落下モードを起動
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
			    //1920画面での中央の「1080ピクセル」の範囲に広く落とす
				// X座標が【 420 〜 1500 】の広範囲になります


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

	//-----------------------
	//剣の発射処理
	//-----------------------
	if (knife->GetCutFlg() == false) {
		//弾が未発射状態なので、弾を打つことができる
		prevShotKey = nowShotKey;
		Knife::DAMAGE;
		nowShotKey = CheckHitKey(KEY_INPUT_G);
		if (prevShotKey == 0 && nowShotKey == 1) {
			//ダウントリガーでキーの状態を判定して、弾を発射する
			Vector2 pPos = player->GetPlayerPos();
			pPos.y -= Player::PLAYER_WID;
			knife->KnifeCreate(pPos);
		}
	}

	//-----------------------
	//苦無の発射処理
	//-----------------------
	if (bullet->GetShotFlg() == false) {
		//弾が未発射状態なので、弾を打つことができる
		prevShotKey = nowShotKey;
		Bullet::DAMAGE;
		if (prevShotKey == 0 && nowShotKey == 1) {
			//ダウントリガーでキーの状態を判定して、弾を発射する
			Vector2 pPos = player->GetPlayerPos();
			pPos.y -= Player::PLAYER_WID;
			bullet->BulletCreate(pPos);
		}
	}

	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0)
	{
		nextSceneID = E_SCENE_GAMEOVER;
	}

	Collision();

}


//描画処理
void GameScene::Draw(void)
{
	int haikeiPosX = (Application::SCREEN_SIZE_WID - HAIKEI_WID) / 2;

	int haikeiPosY = (Application::SCREEN_SIZE_HIG - HAIKEI_HIG) / 2;

	// 1枚目の背景
	DrawGraph(haikeiPosX, haikeiPosY, img, true);

	// 2枚目の背
	DrawGraph(haikeiPosX, haikeiPosY, img, true);

	player->Draw();

	enemy->Draw();

	knife->Draw();

	bullet->Draw();

	for (int i = 0; i < STONE_MAX;i++)
	{
		stones[i]->Draw();
	}
}

//開放処理（最後の一回のみ実行）
bool GameScene::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	player->Release();
	delete player;
	player = nullptr;

	enemy->Release();
	delete enemy;
	enemy = nullptr;

	knife->Release();
	delete knife;
	knife = nullptr;

	bullet->Release();
	delete bullet;
	bullet = nullptr;

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

	if (enemy != nullptr && enemy->GetAlive()) // 敵が生きてる時だけ判定
	{
		// 敵の中心座標を取得（480x440の画像なので、Posが中心にあると仮定）
		Vector2 enemyPos = enemy->GetEnemyPos();

		// 敵の当たり判定の半径（見た目のサイズに合わせて調整してね）
		// 拡大率1.4倍で大きくなっているので、少し大きめの「120.0f」くらいに設定
		float enemyRadius = 120.0f;

		// 画面にあるすべての石をループでチェック
		for (int i = 0; i < STONE_MAX; i++)
		{
			// 石が存在していて、かつ「プレイヤーに弾き飛ばされた後（OnHit後）」だけ敵に当たるようにするなら
			// ※もし「落ちてくる石が敵に当たってもNG」なら、単に stones[i]->IsStoneActive() だけでOK
			if (stones[i] != nullptr && stones[i]->IsStoneActive())
			{
				Vector2 stonePos = stones[i]->GetStonePos();

				// 敵と石の2点間の距離を計算（三平方の定理）
				float diffX = stonePos.x - enemyPos.x;
				float diffY = stonePos.y - enemyPos.y;
				float distanceSq = (diffX * diffX) + (diffY * diffY);

				// お互いの半径の合計の2乗
				float combinedRadius = enemyRadius + stones[i]->GetStoneRadius();
				float combinedRadiusSq = combinedRadius * combinedRadius;

				// 当たったら
				if (distanceSq < combinedRadiusSq)
				{
					// 1. 敵にダメージを与える（1発 20 ダメージ）
					enemy->SetDamage(20);

					// 2. 当たった石は初期状態（非アクティブ）に戻して画面から消す
					stones[i]->GameInit();
				}
			}
		}
	}
}


