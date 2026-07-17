#pragma once

#include "StDefine.h"

class Tutorial
{
public:
	static constexpr int TUTORIAL_WID = 1920;		      // tutorialt画像の横サイズ
	static constexpr int TUTORIAL_HIG = 1080;		      // tutorialt画像の縦サイズ

	static constexpr int SELECT_BUTTON_WID = 768;	  // ステージ選択画像の横サイズ
	static constexpr int SELECT_BUTTON_HIG = 432;	  // ステージ選択画像の縦サイズ

	Tutorial(void);
	~Tutorial(void);

	bool SystemInit(void);				// 初期化処理(最初の１回のみ実行)
	void GameInit(void);				// ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理
	bool Release(void);					// 解放処理(最後の１回のみ実行)

	E_SCENE_ID GetNextSceneID(void) { return nextSceneID; }

private:
	int img;                        //説明画像

	int	 padInput;
	bool isPadBtnPressed;			//PAD

	E_SCENE_ID nextSceneID;		    // 次に遷移するシーンのID
	int prevSpaceKey, nowSpaceKey;	// スペースキーの状態を保存する変数

};

