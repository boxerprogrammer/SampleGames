#pragma once
#include "Actor.h"
class Player : public Actor
{
private:
	//プレイヤーの向き(描画用)
	enum class Direction {
		down,//下=0
		left,//左=1
		leftup,//左上=2
		up,//上=3
		rightup,//右上=4
		right,//右=5
	};
	Direction currentDir_ = Direction::down;//現在向いている方向
	int idleH_ = 0;//待機状態の画像ハンドル
	int walkH_ = 0;//歩き状態の画像ハンドル
	int deathH_ = 0;//死に状態の画像ハンドル
	int animFrame_=0;//アニメーション用フレーム番号
	int frame_ = 0;//汎用フレーム

	//「状態」遷移のための変数
	using UpdateFunc_t = void (Player::*)(Input&);
	using DrawFunc_t = void (Player::*)();

	//待機状態
	void IdleUpdate(Input&);
	void IdleDraw();

	//歩き状態
	void WalkUpdate(Input&);
	void WalkDraw();

	//死に状態
	void DeadUpdate(Input&);
	void DeadDraw();
	void DeadAfterUpdate(Input&);//死んだ後の余韻

	//状態に合わせたメンバ関数ポインタ
	UpdateFunc_t update_;
	DrawFunc_t draw_;
	/// <summary>
	/// 移動関数
	/// </summary>
	bool Move(Input& input);
	void OnHitBlock(const Collision& coll);
public:
	Player(GameScene& gameScene);
	~Player();

	//Actorから継承した関数
	//座標等の更新
	void Update(Input& input)override;
	
	//描画
	void Draw()override;
	//当たった時の反応
	void OnHit(const Collision& coll)override;
	
	void SetPosition(const Position2& pos)override;
};

