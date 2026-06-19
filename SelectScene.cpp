#include  <DxLib.h>
#include "SelectScene.h"
#include "Application.h"
#include "InputManager.h"

SelectScene::SelectScene(void)
{
	img = -1;

	sb = -1;

	slc1 = -1;
	slc2 = -1;
	slc3 = -1;

	slc1_after = -1;
	slc2_after = -1;
	slc3_after = -1;

	slcse = -1;

}

SelectScene::~SelectScene(void)
{

}

//初期化処理(最初の一回のみ実行)
bool SelectScene::SystemInit(void)
{
	// スタートボタン画像の読み込み
	sb = LoadGraph("image/SelectBar.png");
	if (sb == -1)return false;

	// ステージ1画像の読み込み
	slc1 = LoadGraph("image/Select1.png");
	if (slc1 == -1)return false;

	// ステージ2画像の読み込み
	slc2 = LoadGraph("image/Select2.png");
	if (slc2 == -1)return false;

	// ステージ3画像の読み込み
	slc3 = LoadGraph("image/Select3.png");
	if (slc3 == -1)return false;

	// ステージ1クリック後画像の読み込み
	slc1_after = LoadGraph("image/Select1_after.png");
	if (slc1_after == -1)return false;

	// ステージ2クリック後画像の読み込み
	slc2_after = LoadGraph("image/Select2_after.png");
	if (slc2_after == -1)return false;

	// ステージ3クリック後画像の読み込み
	slc3_after = LoadGraph("image/Select3_after.png");
	if (slc3_after == -1)return false;

	// ゲーム背景画像の読み込み
	img = LoadGraph("image/select.jpeg");
	if (img == -1)return false;

	slcse = LoadSoundMem("sound/select.mp3");
	if (slcse == -1)return false;

	return true;

}

//ゲーム起動・再開時に必ず呼び出す処理
void SelectScene::GameInit(void)
{
	isTriggered = false;

	idx = 0;

	nextSceneID = E_SCENE_SELECT;

	prevUpkey = nowUpkey = 0;
	prevDownkey = nowDownkey = 0;
	prevSpaceKey = nowSpaceKey = 0;

}
//更新処理
void SelectScene::Update(void)
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
			idx = 2;
		}
	}

	// 下キーが離された瞬間
	if (prevDownkey == 1 && nowDownkey == 0 || stickDownReleased)
	{
		idx++;
		if (idx > 2)
		{
			idx = 0;
		}
	}

	if (prevSpaceKey == 0 && nowSpaceKey == 1 || isPadBtnPressed)
	{
		isTriggered = true;

		PlaySoundMem(slcse, DX_PLAYTYPE_BACK, false);
	}


	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0 || isPadBtnPressed)
	{
		if (idx == 0)
		{
			nextSceneID = E_SCENE_GAME; // STAGE 1へ
		}
		else if (idx == 1)
		{
			nextSceneID = E_SCENE_GAMEOVER; // STAGE 2へ
		}
		else
		{
			nextSceneID = E_SCENE_GAMECLEAR;    // STAGE 3へ
		}
	}
}


//描画処理
void SelectScene::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - SELECT_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - SELECT_HIG) / 2;
	DrawGraph(dx, dy, img, true);

	int sx = (Application::SCREEN_SIZE_WID - SELECT_BUTTON_WID) / 2;
	int sy[3] =
	{
		(Application::SCREEN_SIZE_HIG - SELECT_BUTTON_HIG) / 2 - 300,
		(Application::SCREEN_SIZE_HIG - SELECT_BUTTON_HIG) / 2,
		(Application::SCREEN_SIZE_HIG - SELECT_BUTTON_HIG) / 2 + 300
	};

	DrawGraph(sx, sy[0], slc1, true);
	DrawGraph(sx, sy[1], slc2, true);
	DrawGraph(sx, sy[2], slc3, true);

	if (isTriggered)
	{
		if (idx == 0)
		{
			DrawGraph(sx, sy[0], slc1_after, true);
		}
		else if (idx == 1)
		{
			DrawGraph(sx, sy[1], slc2_after, true);
		}
		else
		{
			DrawGraph(sx, sy[2], slc3_after, true);
		}
	}

	if (idx == 0)
	{
		DrawGraph(sx, sy[0], sb, true);
	}
	else if (idx == 1)
	{
		DrawGraph(sx, sy[1], sb, true);
	}
	else
	{
		DrawGraph(sx, sy[2], sb, true);
	}


}

//開放処理（最後の一回のみ実行）
bool SelectScene::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	if (DeleteGraph(sb) == -1)return false;

	if (DeleteGraph(slc1) == -1)return false;
	if (DeleteGraph(slc2) == -1)return false;
	if (DeleteGraph(slc3) == -1)return false;

	if (DeleteGraph(slc1_after) == -1)return false;
	if (DeleteGraph(slc2_after) == -1)return false;
	if (DeleteGraph(slc3_after) == -1)return false;

	if (slcse != -1)
	{
		if (DeleteSoundMem(slcse) == -1) return false;
	}

	return true;
}
