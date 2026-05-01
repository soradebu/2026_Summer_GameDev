#include  <DxLib.h>
#include "GameScene.h"
#include "Application.h"
#include "Player.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{

}

//初期化処理(最初の一回のみ実行)
bool GameScene::SystemInit(void)
{
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
}

//開放処理（最後の一回のみ実行）
bool GameScene::Release(void)
{
	return true;
}


