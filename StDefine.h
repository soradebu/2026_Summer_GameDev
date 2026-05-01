#pragma once

//---------------------------
// 列挙型定義
//---------------------------

enum E_SCENE_ID
{
	E_SCENE_NON = -1,
	E_SCENE_TITLE,			// タイトル画面
	E_SCENE_GAME,			// ゲームプレイ画面
	E_SCENE_GAMEOVER,       // ゲームオーバー画面

	E_SCENE_ID_MAX,
};

// フェードの状態
enum E_FADE_STAT_ID {
	E_STAT_FADE_NON = -1,
	E_STAT_FADE_OUT,         // フェードアウト(徐々に暗転する)
	E_STAT_FADE_IN,          // フェードイン(徐々に明転する)

	E_STAT_FADE_MAX,
};



#if 0
// 構造体の例
struct POSITION
{
	int x;
	int y;
};

// 複数の種類の情報をまとめることができる
struct CHARA_DATA
{
	POSITION pos;
	char name[20];       // 名前
	bool aliveFlg;      // 存在フラグ
	int speed;          //移動速度
};

#endif