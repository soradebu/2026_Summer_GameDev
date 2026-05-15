#include  <DxLib.h>
#include "SelectScene.h"
#include "Application.h"

SelectScene::SelectScene(void)
{
	img = -1;

	sb = -1;

	slc1 = -1;
	slc2 = -1;
	slc3 = -1;

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

	// スタートボタン画像の読み込み
	slc1 = LoadGraph("image/Select1.png");
	if (slc1 == -1)return false;

	// スタートボタン画像の読み込み
	slc2 = LoadGraph("image/Select2.png");
	if (slc1 == -1)return false;

	// スタートボタン画像の読み込み
	slc3 = LoadGraph("image/Select3.png");
	if (slc1 == -1)return false;

	// ゲーム背景画像の読み込み
	img = LoadGraph("image/select.jpeg");
	if (img == -1)return false;

	return true;

}

//ゲーム起動・再開時に必ず呼び出す処理
void SelectScene::GameInit(void)
{
	prevUpkey = nowUpkey = 0;
	prevDownkey = nowDownkey = 0;
	prevSpaceKey = nowSpaceKey = 0;

	idx = 0;

	nextSceneID = E_SCENE_SELECT;

}
//更新処理
void SelectScene::Update(void)
{
	prevUpkey = nowUpkey;
	nowUpkey = CheckHitKey(KEY_INPUT_UP);

	prevDownkey = nowDownkey;
	nowDownkey = CheckHitKey(KEY_INPUT_DOWN);

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	// 上キーが離された瞬間
	if (prevUpkey == 1 && nowUpkey == 0)
	{ 
		idx--;
		if (idx < 0)
		{
			idx = 2;
		}
	}

	// アップトリガーでキーの押下を判定
	if (prevDownkey == 1 && nowDownkey == 0)
	{ 
		idx++;
		if (idx > 2)
		{
			idx = 0;
		}
	}

	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0)
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
			nextSceneID = E_SCENE_TITLE;    // STAGE 3へ
		}
	}
}


//描画処理
void SelectScene::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - SELECT_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - SELECT_HIG) / 2;
	DrawGraph(dx, dy, img, true);

	int sx1 = (Application::SCREEN_SIZE_WID - SELECT_BUTTON_WID) / 2;
	int sy1 = (Application::SCREEN_SIZE_HIG - SELECT_BUTTON_HIG) / 2 - 300;
	DrawGraph(sx1, sy1, slc1, true);

	int sx2 = (Application::SCREEN_SIZE_WID - SELECT_BUTTON_WID) / 2;
	int sy2 = (Application::SCREEN_SIZE_HIG - SELECT_BUTTON_HIG) / 2;
	DrawGraph(sx2, sy2, slc2, true);

	int sx3 = (Application::SCREEN_SIZE_WID - SELECT_BUTTON_WID) / 2;
	int sy3 = (Application::SCREEN_SIZE_HIG - SELECT_BUTTON_HIG) / 2 + 300;
	DrawGraph(sx3, sy3, slc3, true);

	if (idx == 0)
	{
		int sbx = (Application::SCREEN_SIZE_WID - SELECT_BUTTON_WID) / 2;
		int sby = (Application::SCREEN_SIZE_HIG - SELECT_BUTTON_HIG) / 2 - 300;
		DrawGraph(sbx, sby, sb, true);
	}
	else if(idx == 1)
	{
		int sbx = (Application::SCREEN_SIZE_WID - SELECT_BUTTON_WID) / 2;
		int sby = (Application::SCREEN_SIZE_HIG - SELECT_BUTTON_HIG) / 2 ;
		DrawGraph(sbx, sby, sb, true);
	}
	else
	{
		int sbx = (Application::SCREEN_SIZE_WID - SELECT_BUTTON_WID) / 2;
		int sby = (Application::SCREEN_SIZE_HIG - SELECT_BUTTON_HIG) / 2 + 300;
		DrawGraph(sbx, sby, sb, true);
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

	return true;
}


