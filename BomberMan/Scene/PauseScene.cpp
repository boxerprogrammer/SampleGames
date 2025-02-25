#include<DxLib.h>
#include"../Input.h"
#include"../Application.h"
#include "PauseScene.h"
#include"SceneController.h"
#include"TitleScene.h"
#include"KeyconfigScene.h"

namespace {
	constexpr int appear_interval = 20;
	constexpr int frame_margin = 10;//�Q�[����ʂ���|�[�Y��ʂ܂ł̕�
	constexpr int menu_text_start_y = 150;//���j���[�̊J�nY�I�t�Z�b�g
	constexpr int menu_text_left = 250;//���j���[�̍�X�I�t�Z�b�g
	constexpr int menu_row_height = 50;//���j���[�P���ړ�����̍���
	constexpr int menu_selector_left = 200;
}

void PauseScene::AppearUpdate(Input& input)
{
	if (++frame_ == appear_interval) {
		update_ = &PauseScene::NormalUpdate;
		draw_ = &PauseScene::NormalDraw;
		return;
	}
}

void PauseScene::NormalUpdate(Input& input)
{
	//P�{�^���Ń|�[�Y����
	if (input.IsTrigger("pause")) {
		update_ = &PauseScene::DisappearUpdate;
		draw_ = &PauseScene::ShiftingDraw;
		frame_ = appear_interval;
		return;
	}
	else if (input.IsTrigger("up")) {
		currentMenuIndex_ = static_cast<uint32_t>((currentMenuIndex_  + menuTexts_.size() - 1) % menuTexts_.size());
	}
	else if (input.IsTrigger("down")) {
		currentMenuIndex_ = (currentMenuIndex_ + 1) % menuTexts_.size();
	}
	else if (input.IsTrigger("ok")) {
		const auto& key = menuTexts_[currentMenuIndex_];
		(this->*functionTable_[key])(input);
	}
}

void PauseScene::DisappearUpdate(Input& input)
{
	if (--frame_ == 0) {
		controller_.PopScene();
		return;//�킷�ꂸ��return
	}
}

void PauseScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(10, 10,//����
		wsize.w - 10, wsize.h - 10,//�E��
		0xffffff,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�|�[�Y��ʘg
	DrawBoxAA(10.0f, 10.0f,//����
		wsize.w - 10.0f, wsize.h - 10.0f,//�E��
		0xffffff,//�J���[
		false,//�h��ׂ��Ȃ�
		3.0f);//������Ƒ��ڂ̐�

	DrawString(30, 30, L" Pause ", 0xffffff);


	DrawMenu();
}

void PauseScene::ShiftingDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//���̍������v�Z����
	const int frameHeight = wsize.h - 10 - 10;
	//�^�񒆂�Y���W���v�Z����
	const int middleY = (10 + wsize.h - 10) / 2;//�^�񒆂�Y���W

	//�����v�Z
	float rate = static_cast<float>(frame_) / static_cast<float>(appear_interval);

	//���݂̎��Ԃł��蓾��ׂ������̔������v�Z���܂�
	int halfHeight = static_cast<int>((frameHeight * rate) *0.5f);

	//��������Atop(��)��bottom(��)���v�Z���܂�
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;

	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawBox(10, top,//����
		wsize.w - 10, bottom,//�E��
		0xffffff,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�|�[�Y��ʘg
	DrawBoxAA(10.0f, static_cast<float>(top),//����
		wsize.w - 10.0f, static_cast<float>(bottom),//�E��
		0xffffff,//�J���[
		false,//�h��ׂ��Ȃ�
		3.0f);//������Ƒ��ڂ̐�
}

void PauseScene::DrawMenu()
{
	int x = frame_margin + menu_text_left;//���j���[���ڕ\���ʒuX
	int y = frame_margin + menu_text_start_y;//���j���[���ڕ\���ʒuY
	
	for (int i = 0; i < menuTexts_.size(); ++i) {
		int select_offset = 0;
		unsigned int menu_row_color = 0x0000ff;//�I�΂�ĂȂ���ΐ�
		//�Z���N�^�[�̕\��
		if (i == currentMenuIndex_) {
			DrawString(frame_margin + menu_selector_left + 1,//�e
				y + 1,
				L"��",
				0x000000);
			DrawString(frame_margin + menu_selector_left,//�{�́�
				y,
				L"��",
				0xff0000);
			select_offset = 10;//�I�΂ꂽ�s�����E�ɂ�����Ƃ��炷
			menu_row_color = 0xbb0088;//�I�΂�Ă�����Ԏ�
		}
		const auto& text = menuTexts_[i];
		DrawFormatString(select_offset+x + 1, y + 1, 0x000000, L"%s", text.c_str());//�e����
		DrawFormatString(select_offset+x, y, menu_row_color, L"%s", text.c_str());//����
		y += menu_row_height;
	}
}

void PauseScene::BackGame(Input&)
{
	update_ = &PauseScene::DisappearUpdate;
	draw_ = &PauseScene::ShiftingDraw;
	frame_ = appear_interval;
}

void PauseScene::GoKeyConfig(Input& input)
{
	controller_.PushScene(std::make_shared<KeyconfigScene>(controller_,input));
}

void PauseScene::EtcConfig(Input&)
{
}

void PauseScene::BackTitle(Input&)
{
	//�����̉��ɂȂ��Ă�V�[����؂�ւ���
	controller_.ChangeBaseScene(std::make_shared<TitleScene>(controller_));
	controller_.PopScene();//�����͏�����
}

PauseScene::PauseScene(SceneController& controller):
	Scene(controller),
	update_(&PauseScene::AppearUpdate),
	draw_(&PauseScene::ShiftingDraw),
	frame_(0)
{
	menuTexts_={//���Ԃ𒲐����邽��
		L"�Q�[���ɖ߂�",
		L"�L�[�R���t�B�O",
		L"���̑��ݒ�",
		L"�^�C�g���ɖ߂�",
		
	};
	functionTable_ = {//���j���[�̕����Ǝ��s�����֐��̑Ή��\
		{L"�Q�[���ɖ߂�",&PauseScene::BackGame },
		{L"�L�[�R���t�B�O",&PauseScene::GoKeyConfig},
		{L"���̑��ݒ�",&PauseScene::EtcConfig},
		{L"�^�C�g���ɖ߂�",&PauseScene::BackTitle}
	};
}

void PauseScene::Update(Input& input)
{
	(this->*update_)(input);
}

void PauseScene::Draw()
{
	(this->*draw_)();
}
