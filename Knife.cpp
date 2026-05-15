#include<DxLib.h>
#include"Player.h"
#include "Knife.h"
#include"SceneManager.h"

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
		// 弾が発射されている状態
			// 弾を移動させる
		pos.y -= speed;

		if (pos.y < 0) {
			// 弾がウィンドウ外に出たので、未発射状態にする
			CutFlg = false;
		}
	}
}

void Knife::Draw(void)
{
	if (CutFlg == true)
	{
		DrawGraph(pos.x, pos.y, img, true);
	}

}

bool Knife::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	return true;
}

void Knife::KnifeCreate(Vector2 bpos)
{
	pos = bpos;
	CutFlg = true;
}