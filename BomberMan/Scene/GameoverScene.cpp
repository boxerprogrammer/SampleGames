#include<DxLib.h>
#include<cassert>
#include "GameoverScene.h"
#include"TitleScene.h"//←遷移したいシーンをまずインクルードします
#include"SceneController.h"//←遷移するために必要
#include"../Input.h"//Inputオブジェクトを使うために必要
#include"../Application.h"



namespace {
	constexpr int fade_interval = 60;
}
void GameoverScene::FadeInUpdate(Input& input)
{
	if (--frame_<=0) {//次の状態に行くトリガー
		update_ = &GameoverScene::NormalUpdate;
		draw_ = &GameoverScene::NormalDraw;
		frame_ = 0;
	}
}

void GameoverScene::NormalUpdate(Input& input)
{
	if (input.IsTrigger("next")) {//次の状態に行くトリガー
		update_ = &GameoverScene::FadeOutUpdate;
		draw_= &GameoverScene::FadeDraw;
		frame_ = 0;
	}
}

void GameoverScene::FadeOutUpdate(Input& input)
{
	if (++frame_>=fade_interval) {//次の状態に行くトリガー
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;//このreturn文は絶対に忘れないように
	}
}

void GameoverScene::FadeDraw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f,static_cast<float>(wsize.h) * 0.5f };
	//背景の表示
	DrawRotaGraph(static_cast<int>(center.x),
		static_cast<int>(center.y), 1.0f, 0.0f, backH_, true);
	auto rate = (float)frame_ / (float)fade_interval;
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(rate * 255.0f));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameoverScene::NormalDraw()
{
	DrawString(10, 10, L"Game Over Scene", 0xffffff);
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f,static_cast<float>(wsize.h) * 0.5f };
	//背景の表示
	DrawRotaGraph(static_cast<int>(center.x),
		static_cast<int>(center.y), 1.0f, 0.0f, backH_, true);
}

GameoverScene::GameoverScene(SceneController& controller) :	Scene(controller),
update_(&GameoverScene::FadeInUpdate),
draw_(&GameoverScene::FadeDraw),
frame_(fade_interval)
{
	backH_ = LoadGraph(L"img/dokuro.png");
	assert(backH_ >= 0);

}

void GameoverScene::Update(Input& input)
{
	(this->*update_)(input);
}



void GameoverScene::Draw()
{
	(this->*draw_)();
}
