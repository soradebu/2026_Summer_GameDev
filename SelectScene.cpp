#include  <DxLib.h>
#include "SelectScene.h"
#include "Application.h"

SelectScene::SelectScene(void)
{
	img = -1;
}

SelectScene::~SelectScene(void)
{

}

//初期化処理(最初の一回のみ実行)
bool SelectScene::SystemInit(void)
{
	player = new Player(this);
	if (player == nullptr)return false;

	if (player->SystemInit() == false)return false;

	// ゲーム背景画像の読み込み
	img = LoadGraph("image/forest2.jpg");
	if (img == -1)return false;

	return true;

}

//ゲーム起動・再開時に必ず呼び出す処理
void SelectScene::GameInit(void)
{
	player->GameInit();

	enCounter = 0;
	prevShotKey = nowShotKey = 0;

	scrollX = 0;
	nextSceneID = E_SCENE_GAME;

}
//更新処理
void SelectScene::Update(void)
{
	player->Update();

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);
	// 背景を左へ動かす
	scrollX -= HAIKEI_MOVE_SPEED;

	if (scrollX <= -HAIKEI_WID)
	{
		scrollX = 0;
	}

	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0)
	{
		nextSceneID = E_SCENE_GAMEOVER;
	}
}


//描画処理
void SelectScene::Draw(void)
{
	int haikeiPosY = (Application::SCREEN_SIZE_HIG - HAIKEI_HIG) / 2;

	// 1枚目の背景
	DrawGraph(scrollX, haikeiPosY, img, true);

	// 2枚目の背景
	DrawGraph(scrollX + HAIKEI_WID, haikeiPosY, img, true);

	player->Draw();

}

//開放処理（最後の一回のみ実行）
bool SelectScene::Release(void)
{
	player->Release();
	delete player;
	player = nullptr;

	return true;
}


