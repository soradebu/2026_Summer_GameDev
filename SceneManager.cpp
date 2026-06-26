#include <DxLib.h>
#include "SceneManager.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Fader.h"
#include "GameOverScene.h"
#include"GameClearScene.h"

// コンストラクタ
SceneManager::SceneManager(void)
{
	titleInst = nullptr;
	selectInst = nullptr;
	stage1 = nullptr;
	stage2 = nullptr;
	stage3 = nullptr;
	fader = nullptr;
	gameover = nullptr;
	gameclear = nullptr;

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

	fader->SystemInit();

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
		case E_SCENE_SELECT:
			selectInst->Update();
			nextSceneId = selectInst->GetNextSceneID();
			break;
		case E_SCENE_STAGE1:
			stage1->Update();
			nextSceneId = stage1->GetNextSceneID();
			break;
		case E_SCENE_STAGE2:
			stage2->Update();
			nextSceneId = stage2->GetNextSceneID();
			break;
		case E_SCENE_STAGE3:
			stage3->Update();
			nextSceneId = stage3->GetNextSceneID();
			break;
		case E_SCENE_GAMEOVER:
			gameover->Update();
			nextSceneId = gameover->GetNextSceneID();
			break;
		case E_SCENE_GAMECLEAR:
			gameclear->Update();
			nextSceneId = gameclear->GetNextSceneID();
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
	case E_SCENE_SELECT:
		selectInst->Draw();
		break;
	case E_SCENE_STAGE1:
		stage1->Draw();
		break;
	case E_SCENE_STAGE2:
		stage2->Draw();
		break;
	case E_SCENE_STAGE3:
		stage3->Draw();
		break;
	case E_SCENE_GAMEOVER:
		gameover->Draw();
		break;
	case E_SCENE_GAMECLEAR:
		gameclear->Draw();
		break;
	}

	fader->Draw();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Current Scene: %d", scene_ID);
}

// 解放処理(最後の１回のみ実行)
bool SceneManager::Release(void)
{
	ReleaseScene(E_SCENE_TITLE);
	ReleaseScene(E_SCENE_SELECT);
	ReleaseScene(E_SCENE_STAGE1);
	ReleaseScene(E_SCENE_STAGE2);
	ReleaseScene(E_SCENE_STAGE3);
	ReleaseScene(E_SCENE_GAMEOVER);
	ReleaseScene(E_SCENE_GAMECLEAR);

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
	case E_SCENE_SELECT:
		if (selectInst == nullptr)
		{
			selectInst = new SelectScene();
			if (selectInst == nullptr)return false;
			selectInst->SystemInit();
			selectInst->GameInit();
		}
		break;
	case E_SCENE_STAGE1:
		if (stage1 == nullptr)
		{
			stage1 = new GameScene();
			if (stage1 == nullptr)return false;
			stage1->SystemInit();
			stage1->GameInit();
		}
		break;
	case E_SCENE_STAGE2:
		if (stage2 == nullptr)
		{
			stage2 = new Stage2();
			if (stage2 == nullptr)return false;
			stage2->SystemInit();
			stage2->GameInit();
		}
		break;
	case E_SCENE_STAGE3:
		if (stage3 == nullptr)
		{
			stage3 = new Stage3();
			if (stage3 == nullptr)return false;
			stage3->SystemInit();
			stage3->GameInit();
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
	case E_SCENE_GAMECLEAR:
		if (gameclear == nullptr)
		{
			gameclear = new GameClearScene();
			if (gameclear == nullptr)return false;
			gameclear->SystemInit();
			gameclear->GameInit();
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
	case E_SCENE_SELECT:
		if (selectInst != nullptr)
		{
			selectInst->Release();
			delete selectInst;
			selectInst = nullptr;
		}
		break;
	case E_SCENE_STAGE1:
		if (stage1 != nullptr)
		{
			stage1->Release();
			delete stage1;
			stage1 = nullptr;
		}
		break;
	case E_SCENE_STAGE2:
		if (stage2 != nullptr)
		{
			stage2->Release();
			delete stage2;
			stage2 = nullptr;
		}
		break;
	case E_SCENE_STAGE3:
		if (stage3 != nullptr)
		{
			stage3->Release();
			delete stage3;
			stage3 = nullptr;
		}
		break;
	case E_SCENE_GAMEOVER:
		if (gameover != nullptr)
		{
			gameover->Release();
			delete gameover;
			gameover = nullptr;
		}
	case E_SCENE_GAMECLEAR:
		if (gameclear != nullptr)
		{
			gameclear->Release();
			delete gameclear;
			gameclear = nullptr;
		}
		break;
	}
}
