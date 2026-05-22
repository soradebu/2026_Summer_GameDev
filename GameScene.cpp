#include  <DxLib.h>
#include "GameScene.h"
#include "Application.h"
#include "Player.h"
#include "Enemy.h"
#include "Knife.h"

GameScene::GameScene(void)
{
	img = -1;
	player = nullptr;
	enemy = nullptr;
	knife = nullptr;
}

GameScene::~GameScene(void)
{

}

//‰Šú‰»ˆ—(Å‰‚Ìˆê‰ñ‚Ì‚ÝŽÀs)
bool GameScene::SystemInit(void)
{
	player = new Player(this);
	if (player == nullptr)return false;

	enemy = new Enemy(this);
	if (enemy == nullptr)return false;

	knife = new Knife(this);
	if (knife == nullptr)return false;

	if (player->SystemInit() == false)return false;

	if (enemy->SystemInit() == false)return false;

	if (knife->SystemInit() == false)return false;

	// ƒQ[ƒ€”wŒi‰æ‘œ‚Ì“Ç‚Ýž‚Ý
	img = LoadGraph("image/stage2.jpg");
	if (img == -1)return false;

	return true;

}

//ƒQ[ƒ€‹N“®EÄŠJŽž‚É•K‚¸ŒÄ‚Ño‚·ˆ—
void GameScene::GameInit(void)
{
	player->GameInit();
	enemy->GameInit();
	knife->GameInit();

	enCounter = 0;
	prevShotKey = nowShotKey = 0;

	scrollX = 0;
	nextSceneID = E_SCENE_GAME;

}
//XVˆ—
void GameScene::Update(void)
{
	player->Update();
	enemy->Update();
	knife->Update();

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_M);

	//-----------------------
	//’e‚Ì”­ŽËˆ—
	//-----------------------
	if (knife->GetCutFlg() == false)
	{
		//’e‚ª–¢”­ŽËó‘Ô‚È‚Ì‚ÅA’e‚ð‘Å‚Â‚±‚Æ‚ª‚Å‚«‚é
		prevShotKey = nowShotKey;

		nowShotKey = CheckHitKey(KEY_INPUT_G);
		if (prevShotKey == 0 && nowShotKey == 1)
		{
			//ƒ_ƒEƒ“ƒgƒŠƒK[‚ÅƒL[‚Ìó‘Ô‚ð”»’è‚µ‚ÄA’e‚ð”­ŽË‚·‚é
			Vector2 pPos = player->GetPlayerPos();
			pPos.y -= Player::PLAYER_WID;  // ƒvƒŒƒCƒ„[‚ÌŒü‚«‚ðŽæ“¾
		}
	}
	// ƒAƒbƒvƒgƒŠƒK[‚ÅƒL[‚Ì‰Ÿ‰º‚ð”»’è
	if (prevSpaceKey == 1 && nowSpaceKey == 0)
	{
		nextSceneID = E_SCENE_GAMEOVER;
	}
}


//•`‰æˆ—
void GameScene::Draw(void)
{
	int haikeiPosX = (Application::SCREEN_SIZE_WID - HAIKEI_WID) / 2;
	int haikeiPosY = (Application::SCREEN_SIZE_HIG - HAIKEI_HIG) / 2;
	

	// 1–‡–Ú‚Ì”wŒi
	DrawGraph(haikeiPosX, haikeiPosY, img, true);
	//DrawGraph(haikeiPosX, haikeiPosY, img, true);

	// 2–‡–Ú‚Ì”wŒi
	//DrawGraph(scrollX + HAIKEI_WID, haikeiPosY, img, true);

	player->Draw();

	enemy->Draw();

	knife->Draw();
}

//ŠJ•úˆ—iÅŒã‚Ìˆê‰ñ‚Ì‚ÝŽÀsj
bool GameScene::Release(void)
{
	if (DeleteGraph(img) == -1)return false;

	player->Release();
	delete player;
	player = nullptr;

	enemy->Release();
	delete enemy;
	enemy = nullptr;

	knife->Release();
	delete knife;
	knife = nullptr;

	return true;
}


