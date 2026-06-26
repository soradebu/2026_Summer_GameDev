#pragma once

#include "StDefine.h"

class Fader;
class TitleScene;
class SelectScene;
class GameScene;
class Stage2;
class Stage3;
class GameOverScene;
class GameClearScene;

class SceneManager
{
public:
  SceneManager(void);             // コンストラクタ
  ~SceneManager(void);            // デストラクタ

  bool SystemInit(void);          // 初期化処理(最初の１回のみ実行)
  void GameInit(void);            // ゲーム起動・再開時に必ず呼び出す処理
  void Update(void);              // ゲーム起動・再開時に必ず呼び出す処理
  void Draw(void);                // 描画処理
  bool Release(void);             // 解放処理(最後の１回のみ実行)

private:
  Fader* fader;                   // フェードクラスのインスタンスのポインタ
  TitleScene* titleInst;          // タイトルシーンクラスのインスタンスのポインタ
  SelectScene* selectInst;        // ステージ選択シーンクラスのインスタンスのポインタ
  GameScene* stage1;              // STAGE1クラスのインスタンスのポインタ
  Stage2* stage2;				  // STAGE2クラスのインスタンスのポインタ
  Stage3* stage3;				  // STAGE3クラスのインスタンスのポインタ
  GameOverScene* gameover;        // ゲームオーバーシーンクラスのインスタンスのポインタ
  GameClearScene* gameclear;      // ゲームクリアシーンクラスのインスタンスのポインタ

  E_SCENE_ID scene_ID;            // 現在のシーンID
  E_SCENE_ID waitScene;           // シーンチェンジで次に遷移するシーンのID

  bool sceneChangeFlg;            // シーンチェンジ実行中フラグ
  bool ChangeScene(E_SCENE_ID id);
  void ReleaseScene(E_SCENE_ID id);
};

