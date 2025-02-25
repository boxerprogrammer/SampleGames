#pragma once
#include "Actor.h"
class Enemy :  public Actor
{
	int idleH_ = 0;//待機状態の画像ハンドル
	int walkH_ = 0;//歩き状態の画像ハンドル
	int deathH_ = 0;//死亡状態の画像ハンドル
	int animFrame_ = 0;//アニメーション用フレーム番号

	//「状態」遷移のための変数
	using UpdateFunc_t = void (Enemy::*)(Input&);
	using DrawFunc_t = void (Enemy::*)();

	//待機状態
	void IdleUpdate(Input&);
	void IdleDraw();

	//歩き状態
	void WalkUpdate(Input&);
	void WalkDraw();

	//死に状態
	void DeadUpdate(Input&);
	void DeadDraw();

	//状態に合わせたメンバ関数ポインタ
	UpdateFunc_t update_;
	DrawFunc_t draw_;
	/// <summary>
	/// 移動関数
	/// </summary>
	bool Move(Input& input);
public:
	Enemy(GameScene& gameScene);
	~Enemy();

	//Actorから継承した関数
	//座標等の更新
	void Update(Input& input)override;

	//描画
	void Draw()override;
	//当たった時の反応
	void OnHit(const Collision& coll)override;

	void SetPosition(const Position2& pos)override;
};

