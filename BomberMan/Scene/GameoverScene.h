#pragma once
#include "Scene.h"
class GameoverScene :
    public Scene
{
private:
	int backH_ = 0;
	int frame_ = 60;

	using UpdateFunc_t = void (GameoverScene::*)(Input& );
	using DrawFunc_t = void (GameoverScene::*)();

	UpdateFunc_t update_ = nullptr;
	DrawFunc_t draw_ = nullptr;

	void FadeInUpdate(Input& input);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input& input);

	void FadeDraw();
	void NormalDraw();

public:
	GameoverScene(SceneController& controller);
	/// <summary>
	/// 内部変数等を更新する
	/// </summary>
	/// <param name="input">入力状態</param>
	virtual void Update(Input& input)override;
    virtual void Draw()override;
};

