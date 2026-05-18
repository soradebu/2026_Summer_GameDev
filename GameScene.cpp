#include  <DxLib.h>
#include"Enemy.h"
#include"Knife.h"
#include"Kunai.h"
#include "GameScene.h"
#include "Application.h"
#include "Player.h"

GameScene::GameScene(void)
{
	img = -1;
	player = nullptr;
	enemy = nullptr;
	knife = nullptr;
	kunai = nullptr;
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

	kunai = new Kunai(this);
	if (kunai == nullptr)return false;

	if (player->SystemInit() == false)return false;

	if (enemy->SystemInit() == false)return false;

	if (knife->SystemInit() == false)return false;

	if (kunai->SystemInit() == false)return false;

	// ゲーム背景画像の読み込み
	img = LoadGraph("image/Stage2.jpg");
	if (img == -1)return false;

	return true;

}

//ゲーム起動・再開時に必ず呼び出す処理
void GameScene::GameInit(void)
{
	player->GameInit();
	enemy-> GameInit();
	knife-> GameInit();
	kunai->GameInit();

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
	knife->Update();
	kunai->Update();

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);
	// 背景を左へ動かす
	scrollX -= HAIKEI_MOVE_SPEED;

	if (scrollX <= -HAIKEI_WID) 
	{
		scrollX = 0;
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
	//剣の発射処理
	//-----------------------
	if (kunai->GetThrowFlg() == false) {
		//弾が未発射状態なので、弾を打つことができる
		prevShotKey = nowShotKey;
		Kunai::DAMAGE;
		nowShotKey = CheckHitKey(KEY_INPUT_T);
		if (prevShotKey == 0 && nowShotKey == 1) {
			//ダウントリガーでキーの状態を判定して、弾を発射する
			Vector2 pPos = player->GetPlayerPos();
			pPos.y -= Player::PLAYER_WID;
			kunai->KunaiCreate(pPos);
		}
	}

	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0)
	{
		nextSceneID = E_SCENE_GAMEOVER;
	}


}


//描画処理
void GameScene::Draw(void)
{
	int haikeiPosY = (Application::SCREEN_SIZE_HIG - HAIKEI_HIG) / 2;

	// 1枚目の背景
	DrawGraph(scrollX, haikeiPosY, img, true);

	// 2枚目の背景
	DrawGraph(scrollX + HAIKEI_WID, haikeiPosY, img, true);

	player->Draw();

	enemy->Draw();

	knife->Draw();

	kunai->Draw();
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

	kunai->Release();
	delete kunai;
	kunai = nullptr;

	return true;
}

void GameScene::Collision(void)
{
	Vector2 enPos = enemy->GetEnemyPos();
	Vector2 bpos;

	// 弾と敵
	if (enemy->GetEnemyAlive() == true && bullet->GetShotFlg() == true) {
		Vector2 bulletPos = bullet->GetBulletPos();
		// 敵と弾の両方ともが表示されている
		if (enPos.y + Enemy::ENEMY_SIZE_WID > bulletPos.y			//①敵の下 ＞ 弾の上
			&& enPos.y < bulletPos.y + Bullet::BULLET_SIZE_HIG		//②敵の上 ＜ 弾の下
			&& enPos.x < bulletPos.x + Bullet::BULLET_SIZE_WID		//③敵の左 ＜ 弾の右
			&& enPos.x + Enemy::ENEMY_SIZE_WID > bulletPos.x		//④敵の右 ＞ 弾の左
			) {
			enemy->SetEnemyAliveOff();  // 敵を倒す
			bullet->GetShotFlgOff();    // 弾を消す

			// 爆発アニメーション用設定
			bpos.x = (enPos.x + Enemy::ENEMY_SIZE_WID / 2) - (Blast::BLAST_SIZE_WID / 2);
			bpos.y = (enPos.y + Enemy::ENEMY_SIZE_HIG / 2) - (Blast::BLAST_SIZE_HIG / 2);
			blast->SetBlastPos(bpos);
			blast->ClrBlastAnimCounter();
			blast->SetBlastAnimFlgOn();
			blast->SetBlastEnemyFlgOn();

			//--------------------------------
			//カウンターの値設定
			hitStopCounter = HIT_STOP_TIME;
			slowCounter = SLOW_DISP_TIME;
			//--------------------------------
		}
	}


