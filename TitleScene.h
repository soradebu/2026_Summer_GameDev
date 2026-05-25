#pragma once

#include "StDefine.h"

class TitleScene
{
public:
	static constexpr int TITLE_SIZE_WID = 1920;		// タイトル画像の横サイズ
	static constexpr int TITLE_SIZE_HIG = 1080;		// タイトル画像の縦サイズ

	static constexpr int BUTTON_WID = 480;		// ボタン画像の横サイズ
	static constexpr int BUTTON_HIG = 480;		// ボタン画像の縦サイズ

	TitleScene(void);
	~TitleScene(void);

	bool SystemInit(void);				// 初期化処理(最初の１回のみ実行)
	void GameInit(void);				// ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理
	bool Release(void);					// 解放処理(最後の１回のみ実行)

	E_SCENE_ID GetNextSceneID(void) { return nextSceneID; }

private:
	int img;					    // 背景画像のハンドル番号
	int start;					    // スタートボタン画像のハンドル番号
	int start_after;			    // スタートボタン画像のハンドル番号
	int exit;					    // exit画像のハンドル番号
	int exit_after;					// exit画像のハンドル番号

	int sb;							// セレクトバー画像のハンドル番号

	int idx;                        // 現在選択中のインデックス

	int bgm;                        // BGMのハンドル番号
	int slcse;                     // クリック効果音のハンドル番号

	bool isTriggered;               //KEY

	int	 padInput;
	bool isPadBtnPressed;			//PAD


	E_SCENE_ID nextSceneID;		    // 次に遷移するシーンのID

	int prevUpkey, nowUpkey;        // 上キー(↑)の状態を保存する変数
	int prevDownkey, nowDownkey;    // 下キー(↓)の状態を保存する変数
	int prevSpaceKey, nowSpaceKey;	// スペースキーの状態を保存する変数
};