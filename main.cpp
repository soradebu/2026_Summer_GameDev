//-------------------------------------
//ゲーム制作：冒険
//氏名　　　：2516023  林田孝裕
//-------------------------------------
#include<DxLib.h>
#include"Application.h"

//---------------------------
// WinMain関数
//---------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// シーンマネージャーのオブジェクトを作成
	Application apl;

	// 初期化処理
	apl.SystemInit();

	// ゲーム起動(メインループ)
	apl.Run();

	// 解放処理
	apl.Release();

	return 0;             //ゲームの終了
}
