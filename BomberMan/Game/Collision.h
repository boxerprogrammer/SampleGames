#pragma once
#include"../Geometry.h"
class Actor;

/// <summary>
/// 衝突タイプ
/// </summary>
enum class CollisionType {
	Player,//プレイヤー
	Block,//ブロック
	Enemy,//敵
	Bomb,//爆弾
	Blast//爆風
};

//当たり判定属性を与えるクラス
class Collision
{
private:
	CollisionType type_;//当たり判定タイプ
	Rect rect_;//当たり矩形
	Actor& owner_;//持ち主への参照
	bool isMoved_=false;//動いたか
	bool isDisable_ = false;//使用禁止状態か？
public:
	Collision(Actor& owner, const Rect& rect, CollisionType type);
	Collision(Actor& owner);
	void SetRect(const Rect& rc);
	void SetType(CollisionType type);
	void SetPosition(const Position2& pos);
	CollisionType GetType()const;
	const Rect& GetRect()const;
	bool IsMoved()const;
	/// <summary>
	/// 当たり判定は無効ですか？
	/// </summary>
	/// <returns>true:無効 / false:無効ではない</returns>
	bool IsDisable()const;
	/// <summary>
	/// 当たり判定を無効にする
	/// </summary>
	void Disable();

	/// <summary>
	/// 当たり判定を有効にする
	/// </summary>
	void Enable();
};

