#pragma once
#include"Vector2.h"


class SceneBase;

class Inazuma
{
public:

    static constexpr int INAZUMA_WID = 60;
    static constexpr int INAZUMA_HIG = 1080;

	Inazuma(SceneBase* scene);
	~Inazuma(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

    void Activate(float x, float y); // —‹‚ğ”­¶‚³‚¹‚é
    bool IsActive() const { return active; }
    bool GetActiveFlg(void) { return active; }
    Vector2 GetPos() const { return pos; }
    float GetRadius() const { return 80.0f; } // “–‚½‚è”»’è—p
    float IsWarning()const { return isWarning; }

private:
    SceneBase* m_pScene;
    Vector2 pos;
    bool isWarning;
    bool active;
    int timer;      // ŒxŠÔ`—‰º—p
    int img;
};

