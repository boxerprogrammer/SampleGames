#pragma once
#include "Actor.h"
class Bomb :
    public Actor
{
	int handle_=0;
	int animFrame_=0;
public:
	Bomb(GameScene& gameScene, const Position2& pos);
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

