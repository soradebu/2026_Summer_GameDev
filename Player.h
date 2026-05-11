#include"Vector2.h"
#include"AsoUtility.h"
#include"GameScene.h"
#include<DxLib.h>

class Player
{
public:
	static constexpr int PLAYER_WID = 96;   //プレイヤーの横サイズ
	static constexpr int PLAYER_HIG = 128;   //プレイヤーの縦サイズ

	static constexpr int ANIM_NUMS = 4;     //方向毎のアニメーション数
	static constexpr int CHARA_MAX = ANIM_NUMS * static_cast<int>(AsoUtility::DIR::MAX);

	static constexpr int MOVE_SPEED = 4;    //一回の移動量
	static constexpr int ANIM_INTERVAL = 10;  //アニメーションの更新間隔

	static constexpr int  PLAYER_HP_MAX = 100;   //プレイヤーのHPの最大値

	static constexpr float scale = 10.0f;    //10倍のサイズに変更する

	Player(GameScene* gs);
	~Player(void);

	bool SystemInit(void);         //初期化処理(最初の一回のみ実行)
	void GameInit(void);           //ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);             //更新処理
	void Draw(void);               //描画処理
	bool Release(void);            //開放処理（最後の一回のみ実行）

	//ゲッター関数
	Vector2 GetPlayerPos(void) { return playerPos; }
	void SetPlayerPos(Vector2 mPos) { playerPos = mPos; }

	int GetHp(void) { return hp; }               //HPの取得
	bool GetAlive(void) { return aliveFlg; }     //生存状態の取得
	void SetDamage(int dp);

private:
	GameScene* gInst;

	//プレイヤー画像
	int player_img;
	//プレイヤーの表示座標
	Vector2 playerPos;
	//フレーム
	int frame;
	//ヒットポイント
	int hp;
	//歩きの画像
	int walkImages[8];
	//生存フラグ
	bool aliveFlg;
	//プレイヤーが向いている方向
	int playerDir;
	//アニメーションカウンター
	int animCounter;
};