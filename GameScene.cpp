#include  <DxLib.h>
#include "GameScene.h"
#include "Application.h"
#include "Player.h"

GameScene::GameScene(void)
{
	img = -1;
	player = nullptr;
}

GameScene::~GameScene(void)
{

}

//初期化処理(最初の一回のみ実行)
bool GameScene::SystemInit(void)
{
	player = new Player(this);
	if (player == nullptr)return false;

	if (player->SystemInit() == false)return false;

	// ゲーム背景画像の読み込み
	img = LoadGraph("image/forest.jpg");
	if (img == -1)return false;

	return true;

}

//ゲーム起動・再開時に必ず呼び出す処理
void GameScene::GameInit(void)
{
	player->GameInit();

	enCounter = 0;
	prevShotKey = nowShotKey = 0;

	scrollX = 0;
	nextSceneID = E_SCENE_GAME;

}
//更新処理
void GameScene::Update(void)
{
	player->Update();

	// 背景を左へ動かす
	scrollX -= HAIKEI_MOVE_SPEED;

	if (scrollX <= -HAIKEI_WID) {
		scrollX = 0;
	}
}


//描画処理
void GameScene::Draw(void)
{


	int haikeiPosY = (Application::SCREEN_SIZE_HIG - HAIKEI_HIG) / 2;

	// 1枚目の背景
	DrawGraph(scrollX, 0, img, true);

	// 2枚目の背景（1枚目のすぐ右隣に配置）
	DrawGraph(scrollX + HAIKEI_WID, 0, img, true);

	player->Draw();

}

//開放処理（最後の一回のみ実行）
bool GameScene::Release(void)
{
	player->Release();
	delete player;
	player = nullptr;

	return true;
}


