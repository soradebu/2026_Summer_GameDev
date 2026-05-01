#pragma once

#include "StDefine.h"

class TitleScene
{
public:
	static constexpr int TITLE_SIZE_WID = 1920;		// タイトル画像の横サイズ
	static constexpr int TITLE_SIZE_HIG = 1080;		// タイトル画像の縦サイズ

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
	E_SCENE_ID nextSceneID;		    // 次に遷移するシーンのID
	int prevSpaceKey, nowSpaceKey;	// スペースキーの状態を保存する変数
};