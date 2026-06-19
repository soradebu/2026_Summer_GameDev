#include<DxLib.h>
#include "Fire.h"

Fire::Fire(GameScene* gs)
{
	img = -1;
	gInst = gs;
}

Fire::~Fire()
{

}

bool Fire::SystemInit(void)
{
	img = LoadGraph("image/fire.png");
	if (img == -1)return false;

	return true;
}

void Fire::GameInit(void)
{
	pos.x = 0;
	pos.y = 0;

	ActiveFlg = false;
	firetimer = 0;
}

void Fire::Update(void)
{
	if (!ActiveFlg)return;

	if (speed.x > 0)
	{
		speed.y += 0.3f;
	}

	//座標の更新
	pos.x += speed.x;
	pos.y += speed.y;
	
	firetimer++;
	// 120フレーム（約2秒）で攻撃終了する
	if (firetimer >= 240) {
		ActiveFlg = false;
	}
}

void Fire::Draw(void)
{
	if (!ActiveFlg) return;

	// 中心描画
	DrawGraph(pos.x - SIZE_WID / 2, pos.y - SIZE_HIG / 2, img, true);
}

bool Fire::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	return true;
}

void Fire::activate(float x, float y)
{
	pos.x = x;
	pos.y = y;
	speed.x = -30.0f; // 左に飛ぶ速度
	speed.y = 0;
	ActiveFlg = true;
	firetimer = 0;

}

float Fire::GetFireRadius(void) {
	return 80.0f;
}