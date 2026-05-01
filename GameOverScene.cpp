#include <DxLib.h>
#include "GameOverScene.h"
#include "Application.h"

GameOverScene::GameOverScene(void)
{
	goImage = -1;
}

GameOverScene::~GameOverScene(void)
{

}

// 初期化処理(最初の一回のみ実行)
bool GameOverScene::SystemInit(void)
{
	// ゲーム背景画像の読み込み
	goImage = LoadGraph("image/Gameover.png");
	if (goImage == -1)return false;

	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void  GameOverScene::GameInit(void)
{
	nextSceneID = E_SCENE_GAMEOVER;

	//prevSpaceKey = nowSpaceKey = 0;
}

// 更新処理
void  GameOverScene::Update(void)
{
	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0)
	{
		nextSceneID = E_SCENE_TITLE;
	}
}

// 描画処理
void  GameOverScene::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - GAMEOVER_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - GAMEOVER_HIG) / 2;
	DrawGraph(dx, dy, goImage, true);
}

// 解放処理(最後の一回のみ実行)
bool  GameOverScene::Release(void)
{
	if (DeleteGraph(goImage) == -1)return false;

	return true;
}
