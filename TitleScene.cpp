#include <DxLib.h>
#include"TitleScene.h"
#include "Application.h"
#include "InputManager.h"

TitleScene::TitleScene(void)
{
	img = -1;

	start = -1;

	start_after = -1;

	bgm = -1;

	slcse = -1;
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

	bgm = LoadSoundMem("sound/Title.wav");
	if (bgm == -1)return false;

	slcse = LoadSoundMem("sound/select.mp3");
	if (slcse == -1)return false;

	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void  TitleScene::GameInit(void)
{
	isTriggered = false;

	nextSceneID = E_SCENE_TITLE;

	prevSpaceKey = nowSpaceKey = 0;

	StopSoundMem(bgm);

	PlaySoundMem(bgm , DX_PLAYTYPE_BACK ,true);
}

// 更新処理
void  TitleScene::Update(void)
{
	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	InputManager& inputIns = InputManager::GetInstance();

	padInput = GetJoypadInputState(DX_INPUT_PAD1);

	isPadBtnPressed = (padInput & PAD_INPUT_1);

	if (prevSpaceKey == 0 && nowSpaceKey == 1 || isPadBtnPressed)
	{
		isTriggered = true;

		PlaySoundMem(slcse, DX_PLAYTYPE_BACK, false);
	}

	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0 || isPadBtnPressed)
	{
		StopSoundMem(bgm);

		nextSceneID = E_SCENE_SELECT;
	}
}

// 描画処理
void  TitleScene::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - TITLE_SIZE_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - TITLE_SIZE_HIG) / 2;
	DrawGraph(dx, dy, img, true);

	int sx = (Application::SCREEN_SIZE_WID - START_WID) / 2 ;
	int sy = (Application::SCREEN_SIZE_HIG - START_HIG) / 2 + 100;
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

	if (bgm != -1)
	{
		if (DeleteSoundMem(bgm) == -1) return false;
	}

	if (slcse != -1)
	{
		if (DeleteSoundMem(slcse) == -1) return false;
	}

	return true;

}
