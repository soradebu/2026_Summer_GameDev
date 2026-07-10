#include <DxLib.h>
#include "Meteor.h"
#include "AsoUtility.h"
#include "Application.h"

Meteor::Meteor(SceneBase* scene)
{
	m_pScene = scene;
}

Meteor::~Meteor(void)
{

}

bool Meteor::SystemInit(const char* fileName)
{
	img = LoadGraph(fileName);
	if (img == -1) return false;

	return true;
}

void Meteor::GameInit(void)
{
	Pos.x = 0;
	Pos.y = 0;
	speed.x = 0;
	speed.y = 0;
	radius = 20.0f; // 当たり判定の半径
	active = false;

	angle = 0.0f;
	isRotate = false;
}

void Meteor::Update(void)
{
	if (!active)return;

	if (speed.x > 0)
	{
		speed.y += 0.3f;
	}

	//座標の更新
	Pos.x += speed.x;
	Pos.y += speed.y;

	//画面外に出たら石を消去する
	if (Pos.y > 800 || Pos.x > 1920)
	{
		active = false;
	}

}

void Meteor::Draw(void)
{
	if (!active)return;

	Vector2 sPos = AsoUtility::Round(Pos);

	if (isRotate)
	{
		DrawRotaGraph(
			Pos.x,
			Pos.y,
			1.0,
			angle,
			img,
			true
		);
	}
	else
	{
		DrawGraph(Pos.x, Pos.y, img, true);
	}

	DrawGraph(Pos.x, Pos.y, img_2, true);

}

bool Meteor::Release(void)
{
	if (img != -1)
	{
		DeleteGraph(img);
		img = -1;
	}

	if (img_2 != -1)
	{
		DeleteGraph(img_2);
		img_2 = -1;
	}

	return true;
}

//敵が地面をたたいた時に呼び出す関数
void Meteor::activate(float startX, float startY)
{
	Pos.x = startX;
	Pos.y = startY;

	speed.x = -10.0f;     // 左へ飛ぶ
	speed.y =  18.0f;     // 少し下へ

	active = true;
}

void Meteor::OnHit(void)
{
	speed.x = 18;
	speed.y = 0;

	isRotate = true;
	angle = -135.0f * DX_PI_F / 180.0f;
}