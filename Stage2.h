#pragma once

#include <vector>
#include "StDefine.h"
#include "Vector2.h"
#include "SceneBase.h"

class EnemyMino;
class Player;
class Knife;

class Stage2 : public SceneBase
{


public:
	static constexpr int HAIKEI_WID = 1920;			// 背景画像の横サイズ
	static constexpr int HAIKEI_HIG = 1080;			// 背景画像の縦サイズ

	static constexpr int HAIKEI_MOVE_SPEED = 10;	// 背景の移動量

	static constexpr int KNIFE_DELAY_TIME = 60;

	static constexpr int HP_WID = 640;			// 背景画像の横サイズ
	static constexpr int HP_HIG = 360;			// 背景画像の縦サイズ

	static constexpr int PAUSE_WID = 1200;		// ポーズ画像の横サイズ
	static constexpr int PAUSE_HIG = 900;		// ポーズ画像の縦サイズ

	static constexpr int PLAY_WID = 93;			// 矢印画像の横サイズ
	static constexpr int PLAY_HIG = 100;		// 矢印画像の縦サイズ


	Stage2(void);
	~Stage2(void);

	bool SystemInit(void);			// 初期化処理(最初の１回のみ実行)
	void GameInit(void);			// ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理
	bool Release(void);				// 解放処理(最後の１回のみ実行)

	E_SCENE_ID GetNextSceneID(void) { return nextSceneID; }

	void Collision(void);

	struct HitEffect 
	{
		float x, y;
		int timer;
		bool active = false;
	};

private:

	Player* player;                   //プレイヤークラスのインスタンスポインタ
	EnemyMino* mino;                     //エネミークラスのインスタンスのポインタ
	Knife* knife;                     //ナイフクラスのインスタンスのポインタ

	// 1. ゲームシーン内の細かい状態を定義
	enum class SceneState
	{
		PLAYING,   // 通常プレイ中
		PAUSE      // ポーズ（中断）中
	};

	SceneState sState;    // 現在の状態
	int pauseMenuCursor;  // ポーズメニューの選択カーソル

	int enCounter;		  // 敵の発生頻度用のカウンター

	int prevShotKey, nowShotKey;		// キーの入力状態

	int sound;
	int sound1;

	int knifeDelayTimer;

	int img;

	int pauseimg;				// ポーズ画
	int pl;						// PLAY画像のハンドル番号

	int scrollX;

	int bgm;

	bool isSpawnActive;         //石の連続落下が動いているか

	int spawnTimer;             //時間差を図るためのフレーム

	int spawnedCount;           //現在までに意思を何個落としたか

	int attackSE;

	int hidanSE;

	int hidanSE1;

	int zimenSE;

	int	 padInput;
	bool isPadBtnPressed1;			//PAD(Aボタン)

	bool isPadBtnPressed2;			//PAD(RTボタン)

	bool isPadBtnPressed3;			//PAD(STARTボタン)

	E_SCENE_ID nextSceneID;         // 次に遷移するシーンのID

	int prevUpkey, nowUpkey;        // 上キーの状態を保存する変数
	int prevDownkey, nowDownkey;    // 下キーの状態を保存する変数
	int prevSpaceKey, nowSpaceKey;	// スペースキーの状態を保存する変数
	int prevEscapeKey, nowEscapeKey;	// エスケープキーの状態を保存する変数

};