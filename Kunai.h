#pragma once
#include<string>
#include"Vector2.h"
#include"Vector2F.h"
#include"AsoUtility.h"

class GameScene;

class Kunai
{

public:

	static constexpr int SIZE_WID = 110;
	static constexpr int SIZE_HIG = 110;
	static constexpr int DAMAGE = 2;

	Kunai(GameScene* gs);
	~Kunai(void);

	bool SystemInit(void);         //初期化処理(最初の一回のみ実行)
	void GameInit(void);                   //ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);                     //更新処理
	void Draw(void);                       //描画処理
	bool Release(void);            //開放処理（最後の一回のみ実行

	void KunaiCreate(Vector2 bpos);

	bool GetThrowFlg(void) { return ThrowFlg; }
	void GetThrowFlgOff(void) { ThrowFlg = false; }
	Vector2 GetKunaiPos(void) { return pos; }

private:
	GameScene* gInst;

	int img;                          //弾の画像のハンドル番号
	Vector2 pos;
	Vector2 vel;
	bool ThrowFlg;
	float angle = 270.0f;    //現在の角度
	float circle = 100.0f;  //剣の届く距離（半径）
	int speed = 30;    //剣を振るスピード
};

<<<<<<< HEAD

=======
>>>>>>> parent of dabdcb7 (Revert "謾ｻ謦�謇区ｮｵ縺ｮ霑ｽ蜉縲√ご繝ｼ繝繧ｷ繝ｼ繝ｳ縺ｮ譖ｴ譁ｰ")
