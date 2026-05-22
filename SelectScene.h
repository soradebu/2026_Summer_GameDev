#pragma once

#include "StDefine.h"

class SelectScene
{
public:
	static constexpr int SELECT_WID = 1920;		      // select画像の横サイズ
	static constexpr int SELECT_HIG = 1080;		      // select画像の縦サイズ
	 
	static constexpr int SELECT_BAR_WID = 768;		  // ステージ選択バーの横サイズ
	static constexpr int SELECT_BAR_HID = 432;		  // ステージ選択バーの横サイズ

	static constexpr int SELECT_BUTTON_WID = 768;	  // ステージ選択画像の横サイズ
	static constexpr int SELECT_BUTTON_HIG = 432 ;	  // ステージ選択画像の縦サイズ

	SelectScene(void);
	~SelectScene(void);

	bool SystemInit(void);				// 初期化処理(最初の１回のみ実行)
	void GameInit(void);				// ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理
	bool Release(void);					// 解放処理(最後の１回のみ実行)

	E_SCENE_ID GetNextSceneID(void) { return nextSceneID; }

private:
	int img;					    // 背景画像のハンドル番号

	int sb;							// セレクトバー画像のハンドル番号
	int slc1;						// ステージ選択ボタン1のハンドル番号
	int slc2;					    // ステージ選択ボタン2のハンドル番号
	int slc3;						// ステージ選択ボタン3のハンドル番号

	int slc1_after;					// ステージ選択後ボタン1のハンドル番号
	int slc2_after;					// ステージ選択後ボタン2のハンドル番号
	int slc3_after;				    // ステージ選択後ボタン3のハンドル番号

	int slcse;						// クリック効果音のハンドル番号

	int idx;                        // 現在選択中のインデックス


	int	 padInput;
	bool isPadBtnPressed;			//PAD

	bool isTriggered;

	E_SCENE_ID nextSceneID;		    // 次に遷移するシーンのID

	int prevUpkey, nowUpkey;        // 上キー(↑)の状態を保存する変数
	int prevDownkey, nowDownkey;    // 下キー(↓)の状態を保存する変数
	int prevSpaceKey, nowSpaceKey;	// スペースキーの状態を保存する変数
};