#pragma once
#include<vector>
#include<memory>
#include "Scene.h"
class Actor;
class GameScene : public Scene
{
private:
	int frame_=60;
	float angle_ = 0.0f;

	void FadeInUpdate(Input& input);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input& input);

	void FadeDraw();
	void NormalDraw();

	using UpdateFunc_t = void(GameScene::*)(Input& input);
	using DrawFunc_t = void(GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	int backH_;//背景画像のハンドル

	//ゲーム中の登場人物(配置可能オブジェクト)
	std::vector<std::shared_ptr<Actor>> actors_;
	std::vector<std::shared_ptr<Actor>> additionalActors_;


	void DrawGround();
	/// <summary>
	/// 全てのActorの当たり判定を行う
	/// </summary>
	void CheckHit();

	/// <summary>
	/// 矩形同士が当たっているかどうかを返す
	/// </summary>
	/// <param name="rcA">矩形A</param>
	/// <param name="rcB">矩形B</param>
	/// <returns>当たってる:true / 当たってない:false</returns>
	bool IsHit(const Rect& rcA,const Rect& rcB)const;
public:
	GameScene(SceneController& controller);
	/// <summary>
	/// 内部変数等を更新する
	/// </summary>
	/// <param name="input">入力状態</param>
	virtual void Update(Input& input)override;
	virtual void Draw()override;
	//特定の場所に爆弾を置く
	void SetBomb(const Position2& pos);
	void SetBlastH(const Position2& pos, int power);
	void SetBlastV(const Position2& pos, int power);
};

