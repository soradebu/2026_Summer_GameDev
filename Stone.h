#pragma once
#include"Vector2.h"

class GameScene;

class Stone
{
public:

	static constexpr int STONE_HIG = 64;
	static constexpr int STONE_WID = 64;

	Stone(GameScene* gs);
	~Stone(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	void activate(float startX, float startY);  //敵が地面をたたいた時に呼び出す関数
	void OnHit(void);                           //剣が当たった時に呼び出す関数
	

	
	Vector2 GetStonePos(void) const { return Pos; }
	float GetStoneRadius(void) const { return radius; }
	bool IsStoneActive(void) const { return active; }

private:
	GameScene* gInst;      //ゲームシーンクラスのインスタンスのポインタ
	Vector2 Pos;           //石の座標
	Vector2 speed;         //石の移動速度

	float radius;          //当たり判定の半径
	bool active;           //出現中フラグ

};


