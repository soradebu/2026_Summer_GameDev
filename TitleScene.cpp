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
	img = LoadGraph("image/Title.png");
	if (img == -1)return false;

	// スタートボタン画像の読み込み
	pl = LoadGraph("image/play.png");
	if (pl == -1)return false;

	// スタートボタン画像の読み込み
	start = LoadGraph("image/Start.png");
	if (start == -1)return false;

	// スタートボタン画像の読み込み
	start_after = LoadGraph("image/Start_after.png");
	if (start_after == -1)return false;

	// スタートボタン画像の読み込み
	exit = LoadGraph("image/Exit.png");
	if (exit == -1)return false;

	// スタートボタン画像の読み込み
	exit_after = LoadGraph("image/exit_after.png");
	if (exit_after == -1)return false;

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

	idx = 0;

	nextSceneID = E_SCENE_TITLE;

	prevUpkey = nowUpkey = 0;
	prevDownkey = nowDownkey = 0;
	prevSpaceKey = nowSpaceKey = 0;

	StopSoundMem(bgm);

	PlaySoundMem(bgm, DX_PLAYTYPE_BACK, true);
}

// 更新処理
void  TitleScene::Update(void)
{
	InputManager& inputIns = InputManager::GetInstance();

	InputManager::JOYPAD_IN_STATE state =
		inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	padInput = GetJoypadInputState(DX_INPUT_PAD1);

	isPadBtnPressed = (padInput & PAD_INPUT_1);

	// 左アナログキーのX値
	int analogKeyY = state.AKeyLY;

	bool nowStickUp = (analogKeyY < 0);
	bool nowStickDown = (analogKeyY > 0);

	prevUpkey = nowUpkey;
	nowUpkey = CheckHitKey(KEY_INPUT_UP);

	prevDownkey = nowDownkey;
	nowDownkey = CheckHitKey(KEY_INPUT_DOWN);

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	static bool prevStickUp = false;
	static bool prevStickDown = false;

	bool stickUpReleased = (prevStickUp && !nowStickUp);
	bool stickDownReleased = (prevStickDown && !nowStickDown);

	prevStickUp = nowStickUp;		// 現在の位置保存
	prevStickDown = nowStickDown;	// 現在の位置保存

	// 上キーが離された瞬間
	if (prevUpkey == 1 && nowUpkey == 0 || stickUpReleased)
	{
		idx--;
		if (idx < 0)
		{
			idx = 1;
		}
	}

	// 下キーが離された瞬間
	if (prevDownkey == 1 && nowDownkey == 0 || stickDownReleased)
	{
		idx++;
		if (idx > 1)
		{
			idx = 0;
		}
	}

	if (prevSpaceKey == 0 && nowSpaceKey == 1 || isPadBtnPressed)
	{
		isTriggered = true;

		StopSoundMem(bgm);

		PlaySoundMem(slcse, DX_PLAYTYPE_BACK, false);
	}


	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0 || isPadBtnPressed)
	{
		if (idx == 0)
		{
			nextSceneID = E_SCENE_SELECT;	// SELECT画面へ
		}
		else
		{
			Release();

			DxLib_End();

			::exit(0);// ゲーム終了
		}
	}
}

// 描画処理
void  TitleScene::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - TITLE_SIZE_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - TITLE_SIZE_HIG) / 2;
	DrawGraph(dx, dy, img, true);

	int ex = (Application::SCREEN_SIZE_WID - BUTTON_WID) / 2;
	int ey = (Application::SCREEN_SIZE_HIG - BUTTON_HIG) / 2 + 350;
	DrawGraph(ex, ey, exit, true);

	int sx = (Application::SCREEN_SIZE_WID - BUTTON_WID) / 2;
	int sy = (Application::SCREEN_SIZE_HIG - BUTTON_HIG) / 2 + 50;
	DrawGraph(sx, sy, start, true);

	if (idx == 0)
	{
		int sbx = (Application::SCREEN_SIZE_WID - PLAY_WID) / 2 - 300;
		int sby = (Application::SCREEN_SIZE_HIG - PLAY_HIG) / 2 + 50;
		DrawGraph(sbx, sby, pl, true);

		if (isTriggered)
		{
			DrawGraph(sx, sy, start_after, true);
		}
	}
	else
	{
		int sbx = (Application::SCREEN_SIZE_WID - PLAY_WID) / 2 - 300;
		int sby = (Application::SCREEN_SIZE_HIG - PLAY_HIG) / 2 + 350;
		DrawGraph(sbx, sby, pl, true);

		if (isTriggered)
		{
			DrawGraph(ex, ey, exit_after, true);
		}
	}
}

// 解放処理(最後の一回のみ実行)
bool  TitleScene::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	if (DeleteGraph(start) == -1)return false;


	if (DeleteGraph(start_after) == -1)return false;


	if (DeleteGraph(exit) == -1)return false;


	if (DeleteGraph(exit_after) == -1)return false;


	if (DeleteGraph(pl) == -1)return false;


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
