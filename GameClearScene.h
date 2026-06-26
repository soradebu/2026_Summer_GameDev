#pragma once
#include"AsoUtility.h"
#include "StDefine.h"


class GameClearScene
{
public:
	// スクロールを発生させる範囲
	static constexpr int GAMECLEAR_WID = 1920;   // ゲームオーバー画面の横サイズ
	static constexpr int GAMECLEAR_HIG = 1080;    // ゲームオーバー画面の縦サイズ

	static constexpr int PLAYER_WID = 150;    //プレイヤーの横サイズ
	static constexpr int PLAYER_HIG = 150;   //プレイヤーの縦サイズ

	static constexpr int BUTTON_WID = 480;	  // 選択画像の横サイズ
	static constexpr int BUTTON_HIG = 270;	  // 選択画像の縦サイズ

	static constexpr int ANIM_NUMS = 2;     //方向毎のアニメーション数
	static constexpr int CHARA_MAX = ANIM_NUMS * static_cast<int>(AsoUtility::DIR::MAX);

	static constexpr int ANIM_INTERVAL = 30;  //アニメーションの更新間隔

	GameClearScene(void);
	~GameClearScene(void);

	bool SystemInit(void);			// 初期化処理(最初の１回のみ実行)
	void GameInit(void);			// ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理
	bool Release(void);				// 解放処理(最後の１回のみ実行)

	E_SCENE_ID GetNextSceneID(void) { return nextSceneID; }

private:

	enum class state
	{
		IDLE,   // 待機
		SELECT,	// 決定
		MAX
	};

	state currentstate;

	static const int MAX_ANIMS = 3;
	int playerImages[static_cast<int>(state::MAX)][MAX_ANIMS];

	int img;						// ゲームクリア画面のハンドル番号

	int player_img;					// プレイヤー（上下のみ移動）画面のハンドル番号
	int player_select;				// プレイヤー画面のハンドル番号

	int btn_stage;					// ステージボタン画面のハンドル番号
	int btn_title;					// タイトルボタン画面のハンドル番号

	int animCounter;				//アニメーションカウンター

	int idx;                        // 現在選択中のインデックス

	int bgm;

	int	 padInput;

	bool isPadBtnPressed;			//PAD

	E_SCENE_ID nextSceneID;         // 次に遷移するシーンのID

	int prevUpkey, nowUpkey;        // 上キーの状態を保存する変数
	int prevDownkey, nowDownkey;    // 下キーの状態を保存する変数
	int prevSpaceKey, nowSpaceKey;	// スペースキーの状態を保存する変数
};
