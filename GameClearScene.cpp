#include <DxLib.h>
#include "GameClearScene.h"
#include "Application.h"
#include "InputManager.h"

GameClearScene::GameClearScene(void)
{
	player_img = -1;

	player_select = -1;

	img = -1;

	bgm = -1;
}

GameClearScene::~GameClearScene(void)
{

}

// 初期化処理(最初の一回のみ実行)
bool GameClearScene::SystemInit(void)
{
	player_img = LoadGraph("image/player_clear.png");
	if (player_img == -1) return false;

	player_select = LoadGraph("image/player_select.png");
	if (player_select == -1) return false;

	btn_stage = LoadGraph("image/next_stage.png");
	if (btn_stage == -1) return false;

	btn_title = LoadGraph("image/to_title.png");
	if (btn_title == -1) return false;

	bgm = LoadSoundMem("sound/Title.wav");
	if (bgm == -1)return false;

	// ゲーム背景画像の読み込み
	img = LoadGraph("image/GameClear.png");
	if (img == -1)return false;

	for (int i = 0; i < 3; i++)
	{
		// 待機アニメーション
		playerImages[static_cast<int>(state::IDLE)][i] = DerivationGraph(i * 150, 0, 150, 150, player_img);

		playerImages[static_cast<int>(state::SELECT)][i] = DerivationGraph(i * 150, 0, 150, 150, player_select);
	}

	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void  GameClearScene::GameInit(void)
{
	idx = 0;

	animCounter = 0;

	currentstate = state::IDLE;

	nextSceneID = E_SCENE_GAMECLEAR;

	StopSoundMem(bgm);

	PlaySoundMem(bgm, DX_PLAYTYPE_BACK, true);

	prevUpkey = nowUpkey = 0;
	prevDownkey = nowDownkey = 0;
	prevSpaceKey = nowSpaceKey = 0;
}

// 更新処理
void  GameClearScene::Update(void)
{
	animCounter++;
	if (animCounter >= 60) animCounter = 0;

	prevUpkey = nowUpkey;
	nowUpkey = CheckHitKey(KEY_INPUT_UP);

	prevDownkey = nowDownkey;
	nowDownkey = CheckHitKey(KEY_INPUT_DOWN);

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	InputManager& inputIns = InputManager::GetInstance();

	InputManager::JOYPAD_IN_STATE state =
		inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	padInput = GetJoypadInputState(DX_INPUT_PAD1);

	isPadBtnPressed = (padInput & PAD_INPUT_1);

	// 左アナログキーのX値
	int analogKeyY = state.AKeyLY;

	bool nowStickUp = (analogKeyY < 0);
	bool nowStickDown = (analogKeyY > 0);


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
		currentstate = state::SELECT;

		StopSoundMem(bgm);
	}



	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0 || isPadBtnPressed)
	{
		currentstate = state::SELECT;

		if (idx == 0)
		{
			nextSceneID = E_SCENE_STAGE2;     // 次のステージへ
		}
		else
		{
			nextSceneID = E_SCENE_TITLE;    // タイトル画面へ
		}
	}
}

// 描画処理
void  GameClearScene::Draw(void)
{
	int animNo = 0;

	if (currentstate == state::IDLE)
	{
		animNo = (animCounter / ANIM_INTERVAL) % 2;
	}
	else
	{
		animNo = (animCounter / ANIM_INTERVAL) % 3;
	}

	// 現在の画像ハンドルを取得
	int currentHandle = playerImages[static_cast<int>(currentstate)][animNo];

	int dx = (Application::SCREEN_SIZE_WID - GAMECLEAR_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - GAMECLEAR_HIG) / 2;
	DrawGraph(dx, dy, img, true);

	int bx = (Application::SCREEN_SIZE_WID - BUTTON_WID) / 2;
	int by[2] =
	{
		(Application::SCREEN_SIZE_HIG - BUTTON_HIG) / 2 + 50,
		(Application::SCREEN_SIZE_HIG - BUTTON_HIG) / 2 + 300
	};

	DrawGraph(bx, by[0], btn_stage, true);
	DrawGraph(bx, by[1], btn_title, true);

	int px = bx = (Application::SCREEN_SIZE_WID - PLAYER_WID) / 2 - 350;
	int py = 0;

	if (idx == 0)
	{
		py = (Application::SCREEN_SIZE_HIG - PLAYER_HIG) / 2 + 50;
	}
	else
	{
		py = (Application::SCREEN_SIZE_HIG - PLAYER_HIG) / 2 + 300;
	}

	DrawGraph(px, py, currentHandle, true);
}

// 解放処理(最後の一回のみ実行)
bool  GameClearScene::Release(void)
{

	if (player_img != -1)
	{
		if (DeleteGraph(player_img) == -1) return false;
	}

	if (player_select != -1)
	{
		if (DeleteGraph(player_select) == -1) return false;
	}

	if (DeleteGraph(img) == -1)return false;

	if (bgm != -1)
	{
		if (DeleteSoundMem(bgm) == -1) return false;
	}

	return true;
}
