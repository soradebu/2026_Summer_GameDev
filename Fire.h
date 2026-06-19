#pragma once
#include"Vector2.h"

class GameScene;

class Fire
{
public:
    static constexpr int SIZE_WID = 200; // 火の横サイズ
    static constexpr int SIZE_HIG = 163; //火の縦サイズ
    static constexpr int DAMAGE = 5;     // 火のダメージ量

    Fire(GameScene* gs);
    ~Fire(void);

    bool SystemInit(void);         //初期化処理(最初の一回のみ実行)
    void GameInit(void);           //ゲーム起動・再開時に必ず呼び出す処理
    void Update(void);             //更新処理
    void Draw(void);               //描画処理
    bool Release(void);            //開放処理（最後の一回のみ実行

    // 攻撃の開始・終了管理
    void activate(float x, float y);
    bool GetActiveFlg(void) { return ActiveFlg; }
    void SetActiveFlgOff(void) { ActiveFlg = false; }
    Vector2 GetFirePos(void) { return pos; }
    float GetFireRadius(void);

private:
    GameScene* gInst;

    int img;                    //火の画像ハンドル
    Vector2 speed;                    //火の移動速度
    Vector2 pos;
    bool ActiveFlg;             //攻撃の確認
    int firetimer;              //攻撃継続時間

};

