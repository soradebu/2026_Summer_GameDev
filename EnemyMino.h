#pragma once
#include <string>
#include "Vector2.h"
#include "Vector2F.h"
#include "AsoUtility.h"

class SceneBase;

class EnemyMino
{

public:

	enum class state
	{
		IDLE,    // 待機
		RUN,	 // 走行
		ATTACK,  // 攻撃
		MAX
	};

	static constexpr int ENEMY_SIZE_WID = 400;
	static constexpr int ENEMY_SIZE_HIG = 400;

	static constexpr int ANIM_INTERVAL = 10;  //アニメーションの更新間隔

	EnemyMino(SceneBase* scene);
	~EnemyMino(void);

	bool SystemInit(void);         //初期化処理(最初の一回のみ実行)
	void GameInit(void);           //ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);             //更新処理
	void Draw(void);               //描画処理
	bool Release(void);            //開放処理（最後の一回のみ実行

	// 敵の生存状態を取得する
	virtual bool GetAlive(void) { return aliveFlg; }

	//敵の場所を取得する
	Vector2 GetEnemyPos(void) { return Pos; }
	bool GetEnemyAlive(void) { return aliveFlg; }
	void SetEnemyAliveOff(void) { aliveFlg = false; }
	float GetEnemyRadius(void);

	// メイン側から、敵が地面を叩いたかどうかを確認するための関数
	bool CheckAndResetPoundFlag();

	bool CheckAndResetFireFlag(void);
	bool CheckResetStoneFlag();

	int GetHP(void) { return hp; }
	void SetDamage(int dp);

private:
	SceneBase* m_pScene;

	state currentstate; // 現在のアニメーション状態

	static const int MAX_ANIMS = 5;
	int minoImages[static_cast<int>(state::MAX)][MAX_ANIMS];


	int mino_img[2];	// 敵画像

	int mino_run[4];	// 敵走行画像

	int mino_attack[5]; // 敵攻撃画像

	//今描画する画像番号
	int currentImg;
	//待機アニメーション用タイマー
	int animeTimer;

	int animNo;

	//Vector2 enemyPos;
	Vector2F Pos;       //敵表示座標

	int animCounter;    //アニメーションカウンター
	int hp;             //ヒットポイント
	int hpMax;          //ヒットポイントの最大値
	bool aliveFlg;      //生存フラグ


};