#include  <DxLib.h>
#include "GameScene.h"
#include "Application.h"
#include "Player.h"

GameScene::GameScene(void)
{
	img = -1;
}

GameScene::~GameScene(void)
{

}

//初期化処理(最初の一回のみ実行)
bool GameScene::SystemInit(void)
{
	// ゲーム背景画像の読み込み
	img = LoadGraph("image/doraemon.png");
	if (img == -1)return false;

	return true;
}

//ゲーム起動・再開時に必ず呼び出す処理
void GameScene::GameInit(void)
{
	enCounter = 0;
	prevShotKey = nowShotKey = 0;

	nextSceneID = E_SCENE_GAME;
}
//更新処理
void GameScene::Update(void)
{

}


//描画処理
void GameScene::Draw(void)
{
	int haikeiPosX = (Application::SCREEN_SIZE_WID - HAIKEI_WID) / 2;
	int haikeiPosY = (Application::SCREEN_SIZE_HIG - HAIKEI_HIG) / 2;

	haikeiPosX += HAIKEI_MOVE_SPEED;
	if (haikeiPosX > HAIKEI_WID) {
		haikeiPosX -= HAIKEI_WID;
	}

	// 背景画像を表示する
	DrawGraph(haikeiPosX, haikeiPosY, img, true);
	DrawGraph(haikeiPosX, haikeiPosY - HAIKEI_HIG, img, true);

}

//開放処理（最後の一回のみ実行）
bool GameScene::Release(void)
{
	return true;
}


