#include <DxLib.h>
#include <time.h>
#include "Application.h"
#include "SceneManager.h"

// コンストラクタ
Application::Application(void)
{
	sceneMana = nullptr;
}

// デストラクタ
Application::~Application(void)
{
}

// 初期化処理(最初の1回のみ)
bool Application::SystemInit(void)
{
	// システム処理
	SetWindowText("2516025 馬場　洸成");   // ゲームウィンドウのタイトル
	SetGraphMode(SCREEN_SIZE_WID, SCREEN_SIZE_HIG, 32);

	// ゲームウィンドウのサイズと色モードを設定
	ChangeWindowMode(true);                // ゲームウィンドウの表示方法(false : フルスクリーン)

	if (DxLib_Init() == -1)return false;

	// 乱数の初期化
	SRand((unsigned int)time(NULL));

	// インスタンスの生成
	sceneMana = new SceneManager();
	if (sceneMana == nullptr)return false;

	sceneMana->SystemInit();

	return true;
}

// ゲーム起動
void Application::Run(void)
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		Update();			// 更新
		Draw();				// 描画
	}
}

// 更新処理
void Application::Update(void)
{
	sceneMana->Update();
}

// 描画処理
void Application::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);				// 描画する画面を裏の画面に設定する
	ClearDrawScreen();							// 描画する画面の内容を消去(クリア)する

	sceneMana->Draw();

	ScreenFlip();								// 裏画面と表画面を入れ替える
}

// 解放処理(最後の１回のみ実行)
bool Application::Release(void)
{
	sceneMana->Release();
	delete sceneMana;
	sceneMana = nullptr;

	if (DxLib_End() == -1)return false;

	return true;
}