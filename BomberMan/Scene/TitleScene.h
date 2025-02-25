#pragma once
#include "Scene.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
private:
	int frame_ = 0;
	int backH_ = -1;//背景のハンドル

	using UpdateFunc_t = void(TitleScene::*)(Input& );
	using DrawFunc_t = void(TitleScene::*)();

	UpdateFunc_t update_;//現在の状態を表す関数ポインタ
	DrawFunc_t draw_;//描画関数ポインタ

	//フェードイン時の更新処理
	void FadeinUpdate(Input&);
	//通常更新処理
	void NormalUpdate(Input&);
	//フェードアウト更新処理
	void FadeoutUpdate(Input&);

	//フェード時の描画
	void FadeDraw();
	//通常描画
	void NormalDraw();

public:
	TitleScene(SceneController& controller);
	/// <summary>
	/// 内部変数等を更新する
	/// </summary>
	/// <param name="input">入力状態</param>
	virtual void Update(Input& input)override;
	virtual void Draw()override;
};

