#pragma once
#include"Vector2.h"


class SceneBase;

class Meteor
{
public:

	static constexpr int METEOR_HIG = 200;
	static constexpr int METEOR_WID = 200;

	Meteor(SceneBase* scene);
	~Meteor(void);

	bool SystemInit(const char* fileName);
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
	SceneBase* m_pScene;

	Vector2 Pos;			//隕石の座標
	Vector2 speed;			//隕石移動速度

	int img;				//隕石の画像
	int img_2;              //隕石の画像2

	float radius;			//当たり判定の半径
	bool active;			//出現中フラグ

	float angle;      // 回転角
	bool isRotate;    // 回転中	: true 違うなら:false

};

