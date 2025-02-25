#pragma once
#include "Actor.h"
enum class BlastDirection {
    Horizontal,
	Vertical,
};
/// <summary>
/// 爆風
/// </summary>
class Blast : public Actor
{
private:
	int frame_=0;
	int power_ = 1;
	BlastDirection dir_;
	using UpdateFunc_t = void (Blast::*)();
	using DrawFunc_t = void (Blast::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void SpreadUpdate();
	void KeepUpdate();
	void DisappearUpdate();

	void SpreadDraw();
	void KeepDraw();
	void DisappearDraw();

public:
	Blast(GameScene& gameScene, const Position2& pos,BlastDirection dir=BlastDirection::Horizontal,int power = 1);
	/// <summary>
	/// 位置などの更新を行うクラス
	/// </summary>
	/// <param name="input">入力オブジェクト</param>
	virtual void Update(Input& input)override;
	/// <summary>
	/// 対象のアクターを表示する
	/// </summary>
	virtual void Draw()override;

	/// <summary>
	/// 当たり判定に反応するイベント関数
	/// </summary>
	/// <param name="collision">当たり判定オブジェクト</param>
	virtual void OnHit(const Collision& collision)override;
};

