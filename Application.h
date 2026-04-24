#pragma once


class Application
{
public:

	static constexpr int SCREEN_SIZE_WID = 1920;  //ゲーム画面の横サイズ
	static constexpr int SCREEN_SIZE_HIG = 1080;  //ゲーム画面の縦サイズ

	Application(void);             //コンストラクタ
	~Application(void);            //デストラクタ

	bool SystemInit(void);         //初期化処理(最初の一回のみ実行)
	void Run(void);                //ゲーム起動
	bool Release(void);            //開放処理(最後の一回のみ実行)

private:

	void Update(void);             //更新処理
	void Draw(void);               //描画処理
};
