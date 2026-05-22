#include<DxLib.h>
#include "Stone.h"
#include"AsoUtility.h"
#include "Application.h"

Stone::Stone(GameScene* gs)
{
	gInst = gs;
	SystemInit();
}

Stone::~Stone(void)
{

}

bool Stone::SystemInit(void)
{


	return true;
}

void Stone::GameInit(void)
{
	Pos.x = 0;
	Pos.y = 0;
	speed.x = 0;
	speed.y = 0;
	radius = 20.0f; // 当たり判定の半径（ドット絵のサイズに合わせて調整してください）
	active = false;

}

void Stone::Update(void)
{
	if (!active)return;

	//座標の更新
	Pos.x += speed.x;
	Pos.y += speed.y;

	//画面外に出たら石を消去する
	if (Pos.y > 1080 || Pos.x > 1920) {
		active = false;
	}

}

void Stone::Draw(void)
{
	if (!active)return;

	DrawCircle(Pos.x,Pos.y,radius, GetColor(150, 150, 150), true);

	//座標をint 型に変換する
	Vector2 sPos;
	sPos = AsoUtility::Round(Pos);

}

bool Stone::Release(void)
{
	return true;
}

//敵が地面をたたいた時に呼び出す関数
void Stone::activate(float startX, float startY)
{
	Pos.x = startX;
	Pos.y = startY;
	speed.x = 0;
	speed.y = 7;
	active = true;
}

void Stone::OnHit(void)
{
	speed.x = 7;
	speed.y = 0;

	//剣が当たったらYへの移動量をゼロにして
	//Xへの移動量を増やす
}