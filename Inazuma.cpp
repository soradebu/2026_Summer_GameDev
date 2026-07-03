#include "Inazuma.h"
#include <DxLib.h>

Inazuma::Inazuma(SceneBase* scene)
{
	img = -1;

	m_pScene = scene;
}

Inazuma::~Inazuma(void)
{

}

bool Inazuma::SystemInit(void)
{
	img = LoadGraph("image/Inazuma.png");
	if (img == -1)return false;

	return true;
}

void Inazuma::GameInit(void)
{
	active = false;
}

void Inazuma::Update(void)
{
	if (!active) return;

	timer++;
	if (timer < 60) {
		isWarning = true;
	}
	else {
		isWarning = false;
	}

	// 120フレーム経過で完全に消滅
	if (timer > 120) {
		active = false;
	}
}

void Inazuma::Draw(void)
{
	if (!active) return;

	if (isWarning) {
		// 予告線
		DrawLine(pos.x + (INAZUMA_WID / 2)+100, 0, pos.x + (INAZUMA_WID / 2)+100, 1080, GetColor(255, 0, 0), 2);
	}
	else {
		DrawGraph(pos.x, pos.y, img, true);
	}
}

void Inazuma::Activate(float x, float y)
{
	pos.x = x; 
	pos.y = y;
	active = true;
	timer = 0;
}

bool Inazuma::Release(void)
{
	return true;
}
