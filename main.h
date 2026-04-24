#pragma once

//------------------
//定数定義
//------------------
const int SCREEN_SIZE_WID = 1920;            //ゲーム画面の横サイズ
const int SCREEN_SIZE_HIG = 1080;            //ゲーム画面の縦サイズ

//プロトタイプ宣言
bool Init(void);
void Update(void);
void Draw(void);
bool Release(void);