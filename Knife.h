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
	void Update(Vector2 playerPos);                     //更新処理
	void Draw(void);                       //描画処理
	bool Release(void);            //開放処理（最後の一回のみ実行

	void KnifeCreate(Vector2 bpos);

	bool GetCutFlg(void) { return CutFlg; }
	void GetCutFlgOff(void) { CutFlg = false; }
	Vector2 GetKnifePos(void) { return pos; }

	// 半径を取る関数（当たり判定で使います）
	float GetRadius(void) const { return radius; }

private:
	GameScene* gInst;

	int img;                          //弾の画像のハンドル番号
	Vector2 pos;
	Vector2 vel;
	bool CutFlg;
	int sound;

	float angle;        //現在の角度
	float angleSpeed;   //1フレームに進む角度の速さ
	float endAngle;     //振り下ろしの終着点
	float knifeDistance;  // プレイヤーの中心からナイフまでの距離（手の長さ）
	float imageRotation;  // ナイフの画像自体の傾き角度
	float radius;
	int speed = 5;      //剣を振るスピード
};