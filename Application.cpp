#include "Application.h"
#include<DxLib.h>

//コンストラクタ
Application::Application(void)
{
	
}

//デストラクタ
Application::~Application(void)
{

}

//初期化処理(最初の一回のみ実行)
bool Application::SystemInit(void)
{
	//システム処理
	SetWindowText("2516020 林田　孝裕");           //ゲームウィンドウのタイトル
	SetGraphMode(SCREEN_SIZE_WID, SCREEN_SIZE_HIG, 32);
	//ゲームウィンドウモードのサイズと色モードを設定
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)return false;

	return true;

}

//ゲーム起動
void Application::Run(void)
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		Update();                     //更新
		Draw();                       //描画
	}
}

//更新
void Application::Update(void)
{
	
}

//描画
void Application::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);     //描画する画面を裏画面に設定する
	ClearDrawScreen();                 //描画する画面の内容を消去（クリア）する

	ScreenFlip();                      //裏画面と表画面を入れ替える

}

//開放処理(最後の一回のみ実行)
bool Application::Release(void)
{

	if (DxLib_End() == -1)return false;

	return true;
}