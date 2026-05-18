#pragma once
#include<string>
#include"Vector2.h"
#include"Vector2F.h"
#include"AsoUtility.h"

class GameScene;

class Enemy
{

	static constexpr int ANIM_NUMS = 4;     //方向毎のアニメーション数
	static constexpr int ANIM_INTERVAL = 10;  //アニメーションの更新間隔

public:

	Enemy(GameScene* gs);
	~Enemy(void);

	bool SystemInit(void);         //初期化処理(最初の一回のみ実行)
	void GameInit(void);           //ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);             //更新処理
	void Draw(void);               //描画処理
	bool Release(void);            //開放処理（最後の一回のみ実行

	// 敵の生存状態を取得する
	virtual bool GetAlive(void) { return aliveFlg; }
	//敵の生存状態を設定する
	virtual void SetAlive(bool bflg) { aliveFlg = bflg; }


	void SetDamage(int dp);

private:

	GameScene* gInst;

	//プレイヤー画像
	int img;

	//Vector2 enemyPos;
	Vector2F Pos;       //敵表示座標
	int animCounter;    //アニメーションカウンター
	int hp;             //ヒットポイント
	int hpMax;          //ヒットポイントの最大値
	bool aliveFlg;      //生存フラグ

};
