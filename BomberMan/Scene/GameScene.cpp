#include<DxLib.h>
#include<cassert>
#include"../Input.h"
#include"../Application.h"
#include "GameScene.h"
#include"SceneController.h"
#include"GameoverScene.h"
#include"PauseScene.h"
#include"../Game/Player.h"
#include"../Game/UnbreakableBlock.h"
#include"../Game/Enemy.h"
#include"../Game/Bomb.h"
#include"../Game/Blast.h"
#include<cmath>
#include<iterator>

namespace {
	constexpr int fade_interval = 60;
	constexpr float chip_scale = 3.0f;
	constexpr int chip_size = 16;
	constexpr int ground_idx_x = 12;
	constexpr int ground_idx_y = 0;
}

void GameScene::FadeInUpdate(Input& input)
{
	if (--frame_ <= 0) {
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
	}
}

void GameScene::NormalUpdate(Input& input)
{
	angle_ += 0.05f;
	if (input.IsTrigger("next")) {//次の状態に行くトリガー
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
	}
	if (input.IsTrigger("pause")) {
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
	}
	for (auto& actor : actors_) {
		actor->Update(input);
	}
	CheckHit();
	std::copy(additionalActors_.begin(),additionalActors_.end(),std::back_inserter(actors_));
	additionalActors_.clear();
	auto remIt = std::remove_if(actors_.begin(), actors_.end(), [](const std::shared_ptr<Actor>& actor) {
		return actor->IsDead();
		});
	actors_.erase(remIt, actors_.end());
}

void GameScene::FadeOutUpdate(Input& input)
{
	if(++frame_>=fade_interval)
	{
		controller_.ChangeScene(std::make_shared<GameoverScene>(controller_));
		return;//←忘れるな
	}
	//ここに何か処理があった場合、上記のreturnがなければ、持ち主が死んでるのに
	//なにかゾンビ処理をすることになる←色々マズいことになる
}

void GameScene::FadeDraw()
{
	DrawGround();
	for (auto& actor : actors_) {
		actor->Draw();
	}
	const Size& wsize = Application::GetInstance().GetWindowSize();
	auto rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(rate* 255.0f));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::NormalDraw()
{
	DrawString(10, 10, L"Game Play Scene", 0xffffff);

	DrawGround();
	for (auto& actor : actors_) {
		actor->Draw();
	}
	
}

void GameScene::DrawGround()
{
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 10; ++i) {
			DrawRectRotaGraphF(chip_size * chip_scale / 2 + i * chip_size * chip_scale,
				chip_size * chip_scale / 2 + j * chip_size * chip_scale,
				ground_idx_x * chip_size, ground_idx_y * chip_size,
				chip_size, chip_size,
				chip_scale, 0.0f,
				backH_, true);
		}
	}
}

void GameScene::CheckHit()
{
	for (auto& actA : actors_) {
		for (auto& actB : actors_) {
			if (actA == actB) {
				continue;
			}
			const auto& collA = actA->GetCollision();
			const auto& collB = actB->GetCollision();
			if (collA.IsDisable() || collB.IsDisable()) {
				continue;
			}
			if (collA.GetType() == collB.GetType()) {
				continue;
			}
			if (!collA.IsMoved() && !collB.IsMoved()) {
				continue;
			}
			if (IsHit(collA.GetRect() , collB.GetRect())) {
				actA->OnHit(collB);
				actB->OnHit(collA);
			}
		}
	}
}

bool GameScene::IsHit(const Rect& rcA, const Rect& rcB) const
{
	//当たらない条件を反転させた方がシンプルなのでこう書いています
	return !(fabsf(rcA.center.x - rcB.center.x) > (rcA.size.w + rcB.size.w) / 2 ||
		fabsf(rcA.center.y - rcB.center.y) > (rcA.size.h + rcB.size.h) / 2);
}

GameScene::GameScene(SceneController& controller) :Scene(controller),
frame_(fade_interval),
update_(&GameScene::FadeInUpdate),
draw_(&GameScene::FadeDraw),
backH_(0)
{
	backH_ = LoadGraph(L"img/texture.png");

	actors_.push_back(std::make_shared<Player>(*this));

	actors_.push_back(std::make_shared<Enemy>(*this));
	actors_.back()->SetPosition(Position2(chip_size*3, 42*3));

	//壊せないブロックを配置(偶数ポジションに配置)
	for (int j = 1; j <=5 ; ++j) {
		for (int i = 1; i <=5; ++i) {
			actors_.push_back(std::make_shared<UnbreakableBlock>(
				*this,
				Position2((static_cast<float>(i*2)-0.5f)*chip_size*chip_scale,
					(static_cast<float>(j * 2)-0.5f) * chip_size * chip_scale)
				));
		}
	}


}


void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}


void GameScene::Draw()
{
	(this->*draw_)();
}

void GameScene::SetBomb(const Position2& pos)
{
	additionalActors_.push_back(std::make_shared<Bomb>(*this, pos));
}

void GameScene::SetBlastH(const Position2& pos, int power)
{
	additionalActors_.push_back(std::make_shared<Blast>(*this, pos, BlastDirection::Horizontal, power));
}

void GameScene::SetBlastV(const Position2& pos, int power)
{
	additionalActors_.push_back(std::make_shared<Blast>(*this, pos, BlastDirection::Vertical, power));
}

