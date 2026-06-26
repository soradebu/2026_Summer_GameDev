#include <DxLib.h>
#include "GameOverScene.h"
#include "Application.h"
#include "InputManager.h"

GameOverScene::GameOverScene(void)
{
	goImage = -1;

	bgm = -1;
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

	bgm = LoadSoundMem("sound/GameOver.mp3");
	if (bgm == -1)return false;

	return true;
	
}

// ゲーム起動・再開時に必ず呼び出す処理
void  GameOverScene::GameInit(void)
{
	nextSceneID = E_SCENE_GAMEOVER;

	StopSoundMem(bgm);

	PlaySoundMem(bgm, DX_PLAYTYPE_BACK, true);

	//prevSpaceKey = nowSpaceKey = 0;
}

// 更新処理
void  GameOverScene::Update(void)
{
	InputManager& inputIns = InputManager::GetInstance();

	InputManager::JOYPAD_IN_STATE state =
		inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	padInput = GetJoypadInputState(DX_INPUT_PAD1);

	isPadBtnPressed = (padInput & PAD_INPUT_1);

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0 || isPadBtnPressed)
	{
		StopSoundMem(bgm);
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

	if (bgm != -1)
	{
		if (DeleteSoundMem(bgm) == -1) return false;
	}

	return true;
}
