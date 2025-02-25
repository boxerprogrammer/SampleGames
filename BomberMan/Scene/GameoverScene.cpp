#include<DxLib.h>
#include<cassert>
#include "GameoverScene.h"
#include"TitleScene.h"//���J�ڂ������V�[�����܂��C���N���[�h���܂�
#include"SceneController.h"//���J�ڂ��邽�߂ɕK�v
#include"../Input.h"//Input�I�u�W�F�N�g���g�����߂ɕK�v
#include"../Application.h"



namespace {
	constexpr int fade_interval = 60;
}
void GameoverScene::FadeInUpdate(Input& input)
{
	if (--frame_<=0) {//���̏�Ԃɍs���g���K�[
		update_ = &GameoverScene::NormalUpdate;
		draw_ = &GameoverScene::NormalDraw;
		frame_ = 0;
	}
}

void GameoverScene::NormalUpdate(Input& input)
{
	if (input.IsTrigger("next")) {//���̏�Ԃɍs���g���K�[
		update_ = &GameoverScene::FadeOutUpdate;
		draw_= &GameoverScene::FadeDraw;
		frame_ = 0;
	}
}

void GameoverScene::FadeOutUpdate(Input& input)
{
	if (++frame_>=fade_interval) {//���̏�Ԃɍs���g���K�[
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;//����return���͐�΂ɖY��Ȃ��悤��
	}
}

void GameoverScene::FadeDraw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f,static_cast<float>(wsize.h) * 0.5f };
	//�w�i�̕\��
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
	//�w�i�̕\��
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
