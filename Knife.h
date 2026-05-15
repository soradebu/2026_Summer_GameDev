#pragma once
#include<string>
#include"Vector2.h"
#include"Vector2F.h"
#include"AsoUtility.h"

class GameScene;

class Knife
{

public:

	static constexpr int SIZE_WID = 92;
	static constexpr int SIZE_HIG = 100;
	static constexpr int DAMAGE = 2;

	Knife(GameScene* gs);
	~Knife(void);

	bool SystemInit(void);         //初期化処理(最初の一回のみ実行)
	void GameInit(void);                   //ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);                     //更新処理
	void Draw(void);                       //描画処理
	bool Release(void);            //開放処理（最後の一回のみ実行

	void KnifeCreate(Vector2 bpos);

	bool GetCutFlg(void) { return CutFlg; }
	void GetCutFlgOff(void) { CutFlg = false; }
	Vector2 GetKnifePos(void) { return pos; }

private:
	GameScene* gInst;

	int img;                          //弾の画像のハンドル番号
	Vector2 pos;
	bool CutFlg;
	float angle = 270.0f;    //現在の角度
	float circle = 100.0f;  //剣の届く距離（半径）
	float speed = 5.0f;    //剣を振るスピード
};

