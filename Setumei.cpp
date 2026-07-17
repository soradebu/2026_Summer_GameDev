#include  <DxLib.h>
#include "tutorial.h"
#include "Application.h"
#include "InputManager.h"

Tutorial::Tutorial(void)
{
	img = -1;
}

Tutorial::~Tutorial(void)
{
	
}

bool Tutorial::SystemInit(void)
{
	img = LoadGraph("image/letter.png");
	if (img == -1)return false;

	return true;
}

void Tutorial::GameInit(void)
{
	nextSceneID = E_SCENE_SELECT;

	prevSpaceKey = nowSpaceKey = 0;
}

void Tutorial::Update(void)
{
	InputManager& inputIns = InputManager::GetInstance();

	InputManager::JOYPAD_IN_STATE state =
		inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	padInput = GetJoypadInputState(DX_INPUT_PAD1);

	isPadBtnPressed = (padInput & PAD_INPUT_1);

	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);


	// アップトリガーでキーの押下を判定
	if (prevSpaceKey == 1 && nowSpaceKey == 0 || isPadBtnPressed)
	{
		nextSceneID = E_SCENE_SELECT;	// SELECT画面へ
	}

}

void Tutorial::Draw(void)
{
	int dx = (Application::SCREEN_SIZE_WID - TUTORIAL_WID) / 2;
	int dy = (Application::SCREEN_SIZE_HIG - TUTORIAL_HIG) / 2;
	DrawGraph(dx, dy, img, true);
}

bool Tutorial::Release(void)
{
	if (DeleteGraph(img) == -1)return false;
	return true;
}
