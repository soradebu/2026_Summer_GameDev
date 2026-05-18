#include<DxLib.h>
#include"Player.h"
#include "Kunai.h"
#include"SceneManager.h"

Kunai::Kunai(GameScene* gs)
{
	img = -1;
	gInst = gs;
}

Kunai::~Kunai(void)
{

}

bool Kunai::SystemInit(void)
{
	img = LoadGraph("image/kunai1.png");
	if (img == -1)return false;

	return true;
}

void Kunai::GameInit(void)
{
	pos.x = 0;
	pos.y = 0;
	ThrowFlg = false;
}

void Kunai::Update(void)
{
	if (ThrowFlg) {
		// 弾が発射されている状態
		// 弾を移動させる
		pos.x += speed;


		if (pos.x > 1920) {
			// 弾がウィンドウ外に出たので、未発射状態にする
			ThrowFlg = false;
		}
	}
}

void Kunai::Draw(void)
{

	if (ThrowFlg == true)
	{
		DrawGraph(pos.x - 30, pos.y, img, true);
	}

}

bool Kunai::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	return true;
}

void Kunai::KunaiCreate(Vector2 bpos)
{
	pos = bpos;
	ThrowFlg = true;
<<<<<<< HEAD
}
=======
}
>>>>>>> parent of dabdcb7 (Revert "謾ｻ謦�謇区ｮｵ縺ｮ霑ｽ蜉縲√ご繝ｼ繝繧ｷ繝ｼ繝ｳ縺ｮ譖ｴ譁ｰ")
