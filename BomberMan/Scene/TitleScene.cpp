#include<DxLib.h>
#include<cassert>
#include"../Application.h"
#include"../Input.h"
#include "TitleScene.h"
#include"SceneController.h"
#include"GameScene.h"


namespace {//�����l�[���X�y�[�X
	constexpr int fade_interval = 60;
}

void TitleScene::FadeinUpdate(Input&)
{
	if (--frame_ <= 0) {
		update_ = &TitleScene::NormalUpdate;
		draw_ = &TitleScene::NormalDraw;
	}
}

void TitleScene::FadeoutUpdate(Input&)
{

	if (frame_++ >= fade_interval) {
		//����ChangeScene���Ăяo���ꂽ�����TitleScene�I�u�W�F�N�g�͏��ł��Ă�
		//���̌�ɉ��������ƁA���񂾃������ɃA�N�Z�X���ăN���b�V�����܂�
		controller_.ChangeScene(std::make_shared<GameScene>(controller_));
		return;//���Ȃ炸�����ŏ������I��(return)���Ă�������
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsTrigger("next")) {
		update_ = &TitleScene::FadeoutUpdate;
		draw_= &TitleScene::FadeDraw;
		frame_ = 0;
	}
}

void TitleScene::FadeDraw()
{
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = {static_cast<float>(wsize.w)*0.5f,static_cast<float>(wsize.h)*0.5f};
	//�w�i�̕\��
	DrawRotaGraph(static_cast<int>(center.x), 
				static_cast<int>(center.y), 1.0f, 0.0f, backH_, true);

	//�t�F�[�h�����̌v�Z(0.0�`1.0)
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(rate*255.0f));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000,true);//�^�����̒����`
	//BlendMode���g�p�������NOBLEND��Y��Ȃ��悤��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{ 
	DrawString(10, 10, L"Title Scene", 0xffffff);
	const Size& wsize = Application::GetInstance().GetWindowSize();
	Position2 center = { static_cast<float>(wsize.w) * 0.5f,static_cast<float>(wsize.h) * 0.5f };
	//�w�i�̕\��
	DrawRotaGraph(static_cast<int>(center.x),
		static_cast<int>(center.y), 1.0f, 0.0f, backH_, true);
}

TitleScene::TitleScene(SceneController& controller):
	Scene(controller),//�e�N���X�̏�����
	update_ (&TitleScene::FadeinUpdate),//�ŏ��̏��
	draw_ (&TitleScene::FadeDraw)//�ŏ��̕`��֐�
{
	backH_=LoadGraph(L"img/title.png");
	assert(backH_ >= 0);
	frame_ = fade_interval;
}

void TitleScene::Update(Input& input)
{
	(this->*update_)(input);
}

void TitleScene::Draw()
{
	(this->*draw_)();
}
