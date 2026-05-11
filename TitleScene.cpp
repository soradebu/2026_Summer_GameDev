#include <DxLib.h>
#include"TitleScene.h"
#include "Application.h"

TitleScene::TitleScene(void)
{
	img = -1;

	start = -1;

	start_after = -1;
}

TitleScene::~TitleScene(void)
{

}

// 初期化処理(最初の一回のみ実行)
bool TitleScene::SystemInit(void)
{
	// ゲーム背景画像の読み込み
	img = LoadGraph("image/Title.jpg");
	if (img == -1)return false;

	// スタートボタン画像の読み込み
	start = LoadGraph("image/Start.png");
	if (start == -1)return false;

	// スタートボタン画像の読み込み
	start_after = LoadGraph("image/Start_after.png");
	if (start_after == -1)return false;

	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void  TitleScene::GameInit(void)
{
	isTriggered = false;

	nextSceneID = E_SCENE_TITLE;

	prevSpaceKey = nowSpaceKey = 0;
}

// 更新処理
void  TitleScene::Update(void)
{
	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	if (prevSpaceKey == 0 && nowSpaceKey == 1)
	{
		isTriggered = true;
	}

	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0)
	{
		nextSceneID = E_SCENE_GAME;
	}
}

// 描画処理
void  TitleScene::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - TITLE_SIZE_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - TITLE_SIZE_HIG) / 2;
	DrawGraph(dx, dy, img, true);

	int sx = (Application::SCREEN_SIZE_WID - START_WID) / 2 - 180;
	int sy = (Application::SCREEN_SIZE_HIG - START_HIG) / 2 + 60;
	DrawGraph(sx, sy, start, true);

	if (isTriggered) {
		DrawGraph(sx, sy, start_after, true);
	}
	else {
		DrawGraph(sx, sy, start, true);
	}
}

// 解放処理(最後の一回のみ実行)
bool  TitleScene::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	if (DeleteGraph(start) == -1)return false;

	return true;

}
