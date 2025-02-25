#pragma once
class GameScene;//ゲームシーン
class Input;//入力
#include"Collision.h"
/// <summary>
/// ゲーム中に配置可能な物体の基底クラス
/// </summary>
class Actor
{
protected:
	GameScene& gameScene_;//持ち主であるゲームシーンの参照を持つ
	Position2 pos_;//アクターの現在座標(中心点とする)
	Collision collision_;
public:
	Actor(GameScene&);
	virtual ~Actor() {}

	virtual void SetPosition(const Position2& pos);

	/// <summary>
	/// 位置などの更新を行うクラス
	/// </summary>
	/// <param name="input">入力オブジェクト</param>
	virtual void Update(Input& input) = 0;
	/// <summary>
	/// 対象のアクターを表示する
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 当たり判定に反応するイベント関数
	/// </summary>
	/// <param name="collision">当たり判定オブジェクト</param>
	virtual void OnHit(const Collision& collision) = 0;

	const Collision& GetCollision()const;

};

