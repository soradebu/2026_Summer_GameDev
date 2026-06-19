#include<DxLib.h>
#include"Player.h"
#include "Bullet.h"
#include"SceneManager.h"

Bullet::Bullet(GameScene* gs)
{
	img = -1;
	gInst = gs;
}

Bullet::~Bullet(void)
{

}

bool Bullet::SystemInit(void)
{
  	img = LoadGraph("image/kunai1.png");
	if (img == -1)return false;

	return true;
}

void Bullet::GameInit(void)
{
	pos.x = 0;
	pos.y = 0;
	ShotFlg = false;
}

void Bullet::Update(void)
{
	if (ShotFlg) {
		// 弾が発射されている状態
		// 弾を移動させる
		pos.x += speed;


		if (pos.x > 1920) {
			// 弾がウィンドウ外に出たので、未発射状態にする
			ShotFlg = false;
		}
	}
}

void Bullet::Draw(void)
{

	if (ShotFlg == true)
	{
		DrawGraph(pos.x - 30, pos.y, img, true);
	}

}

bool Bullet::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	return true;
}

void Bullet::BulletCreate(Vector2 bpos)
{
	pos = bpos;
	ShotFlg = true;
}