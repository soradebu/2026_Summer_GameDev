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

	static constexpr int MOVE_SPEED = 16;    //一回の移動量
	static constexpr int ANIM_INTERVAL = 10;  //アニメーションの更新間隔

	static constexpr int  PLAYER_HP_MAX = 8;   //プレイヤーのHPの最大値

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
	int GetPlayerDir(void){ return playerDir; }
	void SetInvincible(float time) { invincibleTime = time; }
	float GetInvincibleTime(void) { return invincibleTime; }
	void SetDamage(int dp);

	int GetHP(void){ return hp; }
	void DecreaseHP(void) { if (hp > 0) hp--; } 
	bool GetAlive(void) { return aliveFlg; }     //生存状態の取得


private:
	GameScene* gInst;

	enum class state
	{
		IDLE,   // 待機
		RUN,	// 走行
		JUMP,   // ジャンプ
		MAX
	};

	// ジャンプの初期速度
	float jumpSpeed;

	// 重力
	float gravity;

	// 今ジャンプ中かどうかのフラグ
	bool isJumping;

	state currentstate;

	static const int MAX_ANIMS = 6;
	int playerImages[static_cast<int>(state::MAX)][MAX_ANIMS];

	//プレイヤー画像
	int player_img;

	//プレイヤー(走行)画像
	int player_run;

	//プレイヤー(ジャンプ)画像
	int player_jump;

	int	 padInput;

	bool isPadBtnPressed;			//PAD

	//プレイヤーの表示座標
	Vector2 playerPos;
	//フレーム
	int frame;
	//ヒットポイント
	int hp;
	//サウンド
	int sound;

	//歩きの画像
	int runImages[6];

	//生存フラグ
	bool aliveFlg;

	//プレイヤーが向いている方向
	int playerDir;

	float invincibleTime = 0.0f;

	//アニメーションカウンター
	int animCounter;

};