#pragma once
#include<string>
#include"Vector2.h"
#include"Vector2F.h"
#include"AsoUtility.h"

class GameScene;


class Enemy
{

public:

	static constexpr int ENEMY_SIZE_WID = 550;
	static constexpr int ENEMY_SIZE_HIG = 400;

	Enemy(GameScene* gs);
	~Enemy(void);

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

	GameScene* gInst;

	//敵画像
	int img[12];
	//今描画する画像番号
	int currentImg;
	//待機アニメーション用タイマー
	int animeTimer;

	//次の攻撃が発生するまでの待ち時間
	int nextAttackDelay;

	//Vector2 enemyPos;
	Vector2F Pos;       //敵表示座標
	int animCounter;    //アニメーションカウンター
	int hp;             //ヒットポイント
	int hpMax;          //ヒットポイントの最大値
	bool aliveFlg;      //生存フラグ

	int state;           // 敵の状態（0: 通常、1: 叩きモーション中）
	int motionTimer;     // モーションの時間を計るタイマー
	bool isLeft;

	bool isGroundPounded;// 地面を叩いた瞬間だけTRUEになるフラグ
	bool isFireBreathing;  //火を噴いているか
	bool isStoneRaining;   //石を振らせているか
	int attack;            //攻撃パターンの抽選養
};