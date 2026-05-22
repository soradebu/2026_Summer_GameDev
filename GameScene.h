#pragma once

#include <vector>
#include "StDefine.h"
#include "Vector2.h"

class Enemy;
class Player;
class Knife;
class Bullet;
class Stone;

class GameScene
{

public:
	static constexpr int HAIKEI_WID = 1920;       // 背景画像の横サイズ
	static constexpr int HAIKEI_HIG = 1080;		  // 背景画像の縦サイズ
	static constexpr int HAIKEI_MOVE_SPEED = 10;   // 背景の移動量
	static constexpr int STONE_MAX = 5;


	GameScene(void);
	~GameScene(void);

	bool SystemInit(void);			// 初期化処理(最初の１回のみ実行)
	void GameInit(void);			// ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理
	bool Release(void);				// 解放処理(最後の１回のみ実行)

	E_SCENE_ID GetNextSceneID(void) { return nextSceneID; }

	void Collision(void);

private:

	Player* player;                   //プレイヤークラスのインスタンスポインタ
	Enemy* enemy;                     //エネミークラスのインスタンスのポインタ
	Knife* knife;                     //ナイフクラスのインスタンスのポインタ
	Bullet* bullet;                   //銃クラスのインスタンスのポインタ
	Stone* stones[STONE_MAX];         //石クラスのインスタンスのポインタ

	// 敵の発生頻度用のカウンター
	int enCounter;

	int prevShotKey, nowShotKey;		// キーの入力状態

	int BossHPCounter;       //ボスのHPカウンター

	int img;
	
	int scrollX;


	bool isSpawnActive;         //石の連続落下が動いているか
	int spawnTimer;             //時間差を図るためのフレーム
	int spawnedCount;           //現在までに意思を何個落としたか


	E_SCENE_ID nextSceneID;         // 次に遷移するシーンのID
	int prevSpaceKey, nowSpaceKey;	// スペースキーの状態を保存する変数

};