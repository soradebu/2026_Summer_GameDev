#include<DxLib.h>
#include "Player.h"
#include "Knife.h"
#include "SceneManager.h"
#include "AsoUtility.h"

Knife::Knife(GameScene* gs)
{
	img = -1;
	gInst = gs;
}

Knife::~Knife(void)
{

}

bool Knife::SystemInit(void)
{
	img = LoadGraph("image/knife.png");
	if (img == -1)return false;

	return true;
}

void Knife::GameInit(void)
{
	pos.x = 0;
	pos.y = 0;
	CutFlg = false;
}

void Knife::Update(void)
{
	if (CutFlg) 
	{
		pos.x += speed;
		pos.y += speed*2;

		if (pos.x < 220)
		{
			pos.x -= speed;
		}

		if (pos.y > 700 ) 
		{
			// 弾がウィンドウ外に出たので、未発射状態にする
			CutFlg = false;
		}
	}
}

void Knife::Draw(void)
{
	
	if (CutFlg == true)
	{
		if (dir == static_cast<int>(AsoUtility::DIR::LEFT))
		{
			//左右反転して描画する
			DrawTurnGraph(pos.x -50, pos.y, img, true);
		}
		else
		{
			DrawGraph(pos.x + 50, pos.y, img, true);
		}
	}

}

bool Knife::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	return true;
}

void Knife::KnifeCreate(Vector2 bpos, int dir)
{
	pos = bpos;
	this->dir = dir; //ナイフの向きを記憶
	CutFlg = true;
}