#include <DxLib.h>
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "Fader.h"
#include "GameOverScene.h"

// コンストラクタ
SceneManager::SceneManager(void)
{
	titleInst = nullptr;
	gameInst = nullptr;
	fader = nullptr;
	gameover = nullptr;

	scene_ID = waitScene = E_SCENE_NON;
}

// デストラクタ
SceneManager::~SceneManager(void)
{
}

// 初期化処理(最初の１回のみ実行)
bool SceneManager::SystemInit(void)
{
	fader = new Fader();
	if (fader == nullptr)return false;

	SetTransColor(0xff, 0x00, 0xff);			// 透過色の設定

	//fader->SystemInit();

	sceneChangeFlg = false;

	ChangeScene(E_SCENE_TITLE);

	return true;
}

// ゲーム起動・再開時に必ず呼び出す処理
void SceneManager::GameInit(void)
{
}

// 更新処理
void SceneManager::Update(void)
{
	fader->Update();

	if (sceneChangeFlg)
	{
		// シーンチェンジ実行中
		if (fader->IsEnd() && waitScene != E_SCENE_NON) {
			// フェードアウトが終了
			ChangeScene(waitScene);
			waitScene = E_SCENE_NON;
			fader->SetFade(E_STAT_FADE_IN);
		}
		else if (fader->IsEnd() && waitScene == E_SCENE_NON) {
			// フェードインが終了
			sceneChangeFlg = false;		// シーンチェンジ処理が終了
		}
	}
	else {
		E_SCENE_ID nextSceneId = scene_ID;

		switch (scene_ID)
		{
		case E_SCENE_TITLE:
			titleInst->Update();
			nextSceneId = titleInst->GetNextSceneID();
			break;
		case E_SCENE_GAME:
			gameInst->Update();
			nextSceneId = gameInst->GetNextSceneID();
			break;
		case E_SCENE_GAMEOVER:
			gameover->Update();
			nextSceneId = gameover->GetNextSceneID();
			break;
		}

		if (scene_ID != nextSceneId)
		{
			sceneChangeFlg = true;
			waitScene = nextSceneId;
			fader->SetFade(E_STAT_FADE_OUT);
		}
	}
}

// 描画処理
void SceneManager::Draw(void)
{
	switch (scene_ID)
	{
	case E_SCENE_TITLE:
		titleInst->Draw();
		break;
	case E_SCENE_GAME:
		gameInst->Draw();
		break;
	case E_SCENE_GAMEOVER:
		gameover->Draw();
		break;
	}
	//fader->Draw();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Current Scene: %d", scene_ID);
}

// 解放処理(最後の１回のみ実行)
bool SceneManager::Release(void)
{
	ReleaseScene(E_SCENE_TITLE);
	ReleaseScene(E_SCENE_GAME);
	ReleaseScene(E_SCENE_GAMEOVER);

	fader->Release();
	delete fader;
	fader = nullptr;

	return true;
}

bool SceneManager::ChangeScene(E_SCENE_ID id)
{
	// 現在のシーンを解放
	ReleaseScene(scene_ID);

	// シーンIDを変更
	scene_ID = id;

	switch (scene_ID)
	{
	case E_SCENE_TITLE:
		if (titleInst == nullptr)
		{
			titleInst = new TitleScene();
			if (titleInst == nullptr)return false;
			titleInst->SystemInit();
			titleInst->GameInit();
		}
		break;
	case E_SCENE_GAME:
		if (gameInst == nullptr)
		{
			gameInst = new GameScene();
			if (gameInst == nullptr)return false;
			gameInst->SystemInit();
			gameInst->GameInit();
		}
		break;
	case E_SCENE_GAMEOVER:
		if (gameover == nullptr)
		{
			gameover = new GameOverScene();
			if (gameover == nullptr)return false;
			gameover->SystemInit();
			gameover->GameInit();
		}
		break;
	}
	return true;
}

void SceneManager::ReleaseScene(E_SCENE_ID id)
{
	switch (id) {
	case E_SCENE_TITLE:
		if (titleInst != nullptr)
		{
			titleInst->Release();
			delete titleInst;
			titleInst = nullptr;
		}
		break;
	case E_SCENE_GAME:
		if (gameInst != nullptr)
		{
			gameInst->Release();
			delete gameInst;
			gameInst = nullptr;
		}
		break;
	case E_SCENE_GAMEOVER:
		if (gameover != nullptr)
		{
			gameover->Release();
			delete gameover;
			gameover = nullptr;
		}
		break;
	}
}
