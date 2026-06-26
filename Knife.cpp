#include<DxLib.h>
#include"Player.h"
#include"Knife.h"
#include"SceneManager.h"

Knife::Knife(SceneBase* scene)
{
	img = -1;
	m_pScene = scene;
}

Knife::~Knife(void)
{

}

bool Knife::SystemInit(void)

{
	img = LoadGraph("image/knife.png");
	if (img == -1)return false;

	sound = LoadSoundMem("sound/Knife.mp3");
	if (sound == -1)return false;

	return true;

}

void Knife::GameInit(void)

{
	pos.x = 0;
	pos.y = 0;
	CutFlg = false;
	radius = 120.0f;        //右斜め上から下までカバーする半径
	angle = 0.8f;           //右斜め下の固定角度
	angleSpeed = 0.12f;     // 1フレームに進む角度
	endAngle = 0.0f;
	imageRotation = 0.0f;
	knifeDistance = 120.0f;  // プレイヤーの中心からどれくらい離れた円を描くか

	coolTimeCount = 0;       // 最初はすぐに使える状態
	coolTimeMax = 30;

}

void Knife::Update(Vector2 playerPos)
{
	if (CutFlg)
	{
		// 1フレームごとに角度を進める
		angle += angleSpeed;

		// 受け取ったプレイヤーの最新座標を中心に、円形の位置を計算する
		pos.x = playerPos.x + cosf(angle) * knifeDistance;
		pos.y = playerPos.y + sinf(angle) * knifeDistance;

		// 画像の向きも振っている角度に合わせる
		imageRotation = angle + (DX_PI_F / 1.2f);

		// 右斜め下まで振り下ろしたら終了
		if (angle >= endAngle)
		{
			GetCutFlgOff();
		}
	}
}

void Knife::Draw(void)
{
	if (CutFlg == true)
	{
		int cx = 48;
		int cy = 16;

		DrawRotaGraph2(pos.x, pos.y, cx, cy, 1.0f, imageRotation, img, true);
	}
	else
	{
		if (coolTimeCount > 0)
		{
			coolTimeCount--;
		}
	}

}

bool Knife::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	return true;

}

void Knife::KnifeCreate(Vector2 bpos)
{
	if (coolTimeCount > 0) return;

	// 右斜め上（-60度）
	angle = -DX_PI_F / 8.0f;

	// 右斜め下（ 120度）
	endAngle = DX_PI_F / 3.0f;

	CutFlg = true; // 攻撃開始フラグON

	// 攻撃開始と同時にクールタイムを設定 
	coolTimeCount = coolTimeMax;

	PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}