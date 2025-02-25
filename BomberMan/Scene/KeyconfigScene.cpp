#include "KeyconfigScene.h"
#include"../Application.h"
#include"../Input.h"
#include"SceneController.h"
#include"../StringUtility.h"
#include<DxLib.h>
namespace {
	constexpr int appear_interval = 20;//�o������
	constexpr int frame_margin = 10;//�Q�[����ʂ���|�[�Y��ʂ܂ł̕�
	constexpr int config_text_start_y = 120;//�L�[�R���t�B�O�̊J�nY�I�t�Z�b�g
	constexpr int config_text_left = 150;//�L�[�R���t�B�O�̍�X�I�t�Z�b�g
	constexpr int config_row_height = 30;//�L�[�R���t�B�O�̃��j���[�P���ړ�����̍���
	constexpr int config_selector_left = 100;//�L�[�R���t�B�O�̃Z���N�^
	constexpr unsigned int base_color = 0xaaffaa;//�L�[�R���t�B�O�̔w�i�J���[(���邢��)
	constexpr unsigned int bg_alpha = 220;//�L�[�R���t�B�O�̔w�i��(�قڕs����)

}

void KeyconfigScene::AppearUpdate(Input& input)
{
	if (++frame_ == appear_interval) {
		update_ = &KeyconfigScene::NormalUpdate;
		draw_ = &KeyconfigScene::NormalDraw;
		return;
	}
}

void KeyconfigScene::NormalUpdate(Input& input)
{
	const uint32_t itemCount = static_cast<uint32_t>(input_.orderForDisplay_.size() + menuString_.size());//���Ƃ̃L�[�̐��{�u�߂�v�u�m��v��p��
	if (input.IsTrigger("pause")) {
		Close();
	}else if (input.IsTrigger("up")) {
		currentSelecterIndex_ = (currentSelecterIndex_ + itemCount - 1) % itemCount;
	}
	else if (input.IsTrigger("down")) {
		currentSelecterIndex_ = (currentSelecterIndex_ +1) % itemCount;
	}
	else if (input.IsTrigger("ok")) {
		if (currentSelecterIndex_ < static_cast<uint32_t>(input.orderForDisplay_.size())) {
			update_=&KeyconfigScene::EdittingUpdate;
		}
		else {
			//�L�[�ȊO�̃��j���[�̃C���f�b�N�X���擾����
			auto menuIdx = currentSelecterIndex_ - input.orderForDisplay_.size();
			auto menuStr = menuString_[menuIdx];
			(this->*menuFuncTable_[menuStr])(input);
		}
	}

}
void KeyconfigScene::EdittingUpdate(Input& input)
{
	if (input.IsTrigger("ok")) {
		if (currentSelecterIndex_ < input.orderForDisplay_.size()) {
			update_ = &KeyconfigScene::NormalUpdate;
		}
	}
	else {
		//���ݕҏW���̃C�x���g�����擾����
		std::string edittingEventName = input.orderForDisplay_[currentSelecterIndex_];
		for (auto& row : input.tempInputTable_[edittingEventName]) {
			if (row.type == Input::PeripheralType::keybd) {
				int keystate = input.GetKeyboradState();
				if (keystate != -1) {
					row.id = keystate;
				}
			}
			else if (row.type == Input::PeripheralType::pad1) {
				int padState = input.GetPadState(DX_INPUT_PAD1);
				if (padState != -1) {
					row.id = padState;
				}
			}
		}
	}
}

void KeyconfigScene::DisappearUpdate(Input& input)
{
	if (--frame_ == 0) {
		controller_.PopScene();
		return;//�킷�ꂸ��return
	}
}



void KeyconfigScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//���̍������v�Z����
	const int frameHeight = wsize.h - frame_margin - frame_margin;
	//�^�񒆂�Y���W���v�Z����
	const int middleY = (frame_margin + wsize.h - frame_margin) / 2;//�^�񒆂�Y���W

	//�����v�Z
	float rate = 1.0f;

	//���݂̎��Ԃł��蓾��ׂ������̔������v�Z���܂�
	int halfHeight = static_cast<int>((frameHeight * rate) / 2.0f);

	//��������Atop(��)��bottom(��)���v�Z���܂�
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;

	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, bg_alpha);
	DrawBox(10, top,//����
		wsize.w - 10, bottom,//�E��
		base_color,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�|�[�Y��ʘg
	DrawBoxAA(10.0f, static_cast<float>(top),//����
		wsize.w - 10.0f, static_cast<float>(bottom),//�E��
		0xffffff,//�J���[
		false,//�h��ׂ��Ȃ�
		3.0f);//������Ƒ��ڂ̐�



	DrawString(30, 30, L"KeyConfig Scene", 0x000000);
	DrawInputInfo();
}

void KeyconfigScene::ShiftingDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//���̍������v�Z����
	const int frameHeight = wsize.h - frame_margin - frame_margin;
	//�^�񒆂�Y���W���v�Z����
	const int middleY = (frame_margin + wsize.h - frame_margin) / 2;//�^�񒆂�Y���W

	//�����v�Z
	float rate = static_cast<float>(frame_) / static_cast<float>(appear_interval);

	//���݂̎��Ԃł��蓾��ׂ������̔������v�Z���܂�
	int halfHeight = static_cast<int>((frameHeight * rate) / 2.0f);

	//��������Atop(��)��bottom(��)���v�Z���܂�
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;

	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, bg_alpha);
	DrawBox(10, top,//����
		wsize.w - 10, bottom,//�E��
		base_color,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�|�[�Y��ʘg
	DrawBoxAA(10.0f, static_cast<float>(top),//����
		wsize.w - 10.0f, static_cast<float>(bottom),//�E��
		0xffffff,//�J���[
		false,//�h��ׂ��Ȃ�
		3.0f);//������Ƒ��ڂ̐�
}

void KeyconfigScene::DrawInputInfo()
{
	int y = frame_margin + config_text_start_y;
	int x = frame_margin + config_text_left;

	constexpr int actual_id_margin = 80;//���ۂ̓��̓R�[�h�̃I�t�Z�b�g
	constexpr int actual_id_width = 160;//���̓R�[�h�̕�
	int idx = 0;
	//�K�v�ȃL�[��񂾂��\������
	for (const auto& inputRow : input_.orderForDisplay_) {
		int selectOffset = 0;
		if (idx == currentSelecterIndex_) {
			DrawStringWithShadow(frame_margin + config_selector_left,y,	L"��",0xff0000);
			selectOffset = 5;
		}
		x = frame_margin + config_text_left;//x�ʒu�����Z�b�g
		const auto& eventName = StringUtility::GetWStringFromString(inputRow);

		unsigned int textColor = 0x0000ff;
		if (update_ == &KeyconfigScene::EdittingUpdate) {
			if (currentSelecterIndex_ == idx) {
				textColor = 0xff0000;
				selectOffset = 10;
			}
		}
		//�C�x���g����\��
		DrawFormatString(selectOffset+x + 1, y + 1, 0x000000, L"%s", eventName.c_str());//�e����
		DrawFormatString(selectOffset+x, y, textColor, L"%s", eventName.c_str());//����

		//���ۂ̓��͐M�����
		//inputTable���̓���̃C�x���g�ɂ��������ID�z����܂킷
		for (const auto& keyVakue : input_.tempInputTable_[inputRow]) {
			std::wstring periTypeStr = L"";
			std::wstring inputStr = L"";
			if (keyVakue.type == Input::PeripheralType::keybd) {
				periTypeStr = L"keybd";
				inputStr = keyNameTable_[keyVakue.id];
			}
			else if (keyVakue.type == Input::PeripheralType::pad1) {
				periTypeStr = L"pad1";
				inputStr = buttonNameTable_[keyVakue.id];
			}
			DrawFormatString(selectOffset+actual_id_margin + x + 1, y + 1,
				0x000000, L"%s=%s ", periTypeStr.c_str(), inputStr.c_str());//�e����
			DrawFormatString(selectOffset+actual_id_margin + x, y,
				textColor, L"%s=%s ", periTypeStr.c_str(), inputStr.c_str());//�{�̕���
			x += actual_id_width;
		}
		y += config_row_height;
		++idx;
	}
	
	const uint32_t eventSize= static_cast<uint32_t>(input_.orderForDisplay_.size());
	
	y += config_row_height;
	const auto& wsize = Application::GetInstance().GetWindowSize();
	x = wsize.w / 2 - 80;

	constexpr int arrow_minus_offset = 10;
	int menuIndex = 0;
	for (auto& str : menuString_) {//�L�[�ȊO�̃��j���[���������
		int selectOffset = 0;
		if (currentSelecterIndex_ == eventSize+menuIndex) {
			DrawStringWithShadow(x - arrow_minus_offset, y, L"��", 0xff0000);
			selectOffset = 5;
		}
		int width = GetDrawStringWidth(str.c_str(), static_cast<int>(str.length()));
		DrawStringWithShadow(320-width/2+selectOffset, y, str.c_str(), 0x666666);
		y += config_row_height;
		++menuIndex;
	}
}

void KeyconfigScene::DrawStringWithShadow(int x, int y, const wchar_t* str, uint32_t color)
{
	DrawString(x + 1,//�e
		y + 1,
		str,
		0x000000);
	DrawString(x,//�{�́�
		y,
		str,
		color);
}

void KeyconfigScene::Close()
{
	update_ = &KeyconfigScene::DisappearUpdate;
	draw_ = &KeyconfigScene::ShiftingDraw;
	frame_ = appear_interval;
}

void KeyconfigScene::CloseKeyConfig(Input&)
{
	input_.RollbackEdittedInputTable();
	Close();
}

void KeyconfigScene::CommitAndClose(Input& input)
{
	input_.CommitEdittedInputTable();
	Close();
}

void KeyconfigScene::CancelEdit(Input&)
{
}

void KeyconfigScene::SetDefault(Input&)
{
	input_.SetDafault();
	Close();
}

KeyconfigScene::KeyconfigScene(SceneController& controller,Input& input):
	Scene(controller),
	input_(input),
	frame_(0),
	update_(&KeyconfigScene::AppearUpdate),
	draw_(&KeyconfigScene::ShiftingDraw)
{
	//�L�[�R�[�h�̃L�[���̂̕ϊ��e�[�u��
	keyNameTable_[KEY_INPUT_A] = L"�`�L�[";
	keyNameTable_[KEY_INPUT_B] = L"�a�L�[";
	keyNameTable_[KEY_INPUT_C] = L"�b�L�[";
	keyNameTable_[KEY_INPUT_D] = L"�c�L�[";
	keyNameTable_[KEY_INPUT_E] = L"�d�L�[";
	keyNameTable_[KEY_INPUT_F] = L"�e�L�[";
	keyNameTable_[KEY_INPUT_G] = L"�f�L�[";
	keyNameTable_[KEY_INPUT_H] = L"�g�L�[";
	keyNameTable_[KEY_INPUT_I] = L"�h�L�[";
	keyNameTable_[KEY_INPUT_J] = L"�i�L�[";
	keyNameTable_[KEY_INPUT_K] = L"�j�L�[";
	keyNameTable_[KEY_INPUT_L] = L"�k�L�[";
	keyNameTable_[KEY_INPUT_M] = L"�l�L�[";
	keyNameTable_[KEY_INPUT_N] = L"�m�L�[";
	keyNameTable_[KEY_INPUT_O] = L"�n�L�[";
	keyNameTable_[KEY_INPUT_P] = L"�o�L�[";
	keyNameTable_[KEY_INPUT_Q] = L"�p�L�[";
	keyNameTable_[KEY_INPUT_R] = L"�q�L�[";
	keyNameTable_[KEY_INPUT_S] = L"�r�L�[";
	keyNameTable_[KEY_INPUT_T] = L"�s�L�[";
	keyNameTable_[KEY_INPUT_U] = L"�t�L�[";
	keyNameTable_[KEY_INPUT_V] = L"�u�L�[";
	keyNameTable_[KEY_INPUT_W] = L"�v�L�[";
	keyNameTable_[KEY_INPUT_X] = L"�w�L�[";
	keyNameTable_[KEY_INPUT_Y] = L"�x�L�[";
	keyNameTable_[KEY_INPUT_Z] = L"�y�L�[";
	keyNameTable_[KEY_INPUT_0] = L"�O�L�[";
	keyNameTable_[KEY_INPUT_1] = L"�P�L�[";
	keyNameTable_[KEY_INPUT_2] = L"�Q�L�[";
	keyNameTable_[KEY_INPUT_3] = L"�R�L�[";
	keyNameTable_[KEY_INPUT_4] = L"�S�L�[";
	keyNameTable_[KEY_INPUT_5] = L"�T�L�[";
	keyNameTable_[KEY_INPUT_6] = L"�U�L�[";
	keyNameTable_[KEY_INPUT_7] = L"�V�L�[";
	keyNameTable_[KEY_INPUT_8] = L"�W�L�[";
	keyNameTable_[KEY_INPUT_9] = L"�X�L�[";
	keyNameTable_[KEY_INPUT_BACK] = L"BackSpace�L�[";
	keyNameTable_[KEY_INPUT_TAB] = L"Tab�L�[";
	keyNameTable_[KEY_INPUT_RETURN] = L"Enter�L�[";
	keyNameTable_[KEY_INPUT_LSHIFT] = L"��Shift�L�[";
	keyNameTable_[KEY_INPUT_RSHIFT] = L"�EShift�L�[";
	keyNameTable_[KEY_INPUT_LCONTROL] = L"��Ctrl�L�[";
	keyNameTable_[KEY_INPUT_RCONTROL] = L"�ECtrl�L�[";
	keyNameTable_[KEY_INPUT_ESCAPE] = L"Esc�L�[";
	keyNameTable_[KEY_INPUT_SPACE] = L"�X�y�[�X�L�[";
	keyNameTable_[KEY_INPUT_PGUP] = L"PageUp�L�[";
	keyNameTable_[KEY_INPUT_PGDN] = L"PageDown�L�[";
	keyNameTable_[KEY_INPUT_END] = L"End�L�[";
	keyNameTable_[KEY_INPUT_HOME] = L"Home�L�[";
	keyNameTable_[KEY_INPUT_LEFT] = L"���L�[";
	keyNameTable_[KEY_INPUT_UP] = L"��L�[";
	keyNameTable_[KEY_INPUT_RIGHT] = L"�E�L�[";
	keyNameTable_[KEY_INPUT_DOWN] = L"���L�[";
	keyNameTable_[KEY_INPUT_INSERT] = L"Insert�L�[";
	keyNameTable_[KEY_INPUT_DELETE] = L"Delete�L�[";
	
	//PAD�{�^��ID��PAD�{�^�����ϊ��e�[�u��
	buttonNameTable_[PAD_INPUT_A] = L"�`�{�^��";
	buttonNameTable_[PAD_INPUT_B] = L"�a�{�^��";
	buttonNameTable_[PAD_INPUT_C] = L"�w�{�^��";
	buttonNameTable_[PAD_INPUT_X] = L"�x�{�^��";
	buttonNameTable_[PAD_INPUT_Y] = L"L1�{�^��";
	buttonNameTable_[PAD_INPUT_Z] = L"R1�{�^��";
	buttonNameTable_[PAD_INPUT_L] = L"SELECT�{�^��";
	buttonNameTable_[PAD_INPUT_R] = L"START�{�^��";
	buttonNameTable_[PAD_INPUT_START] = L"��������";
	buttonNameTable_[PAD_INPUT_M] = L"��������";

	menuString_ = { L"�߂�",
					L"�m��" ,
					L"�f�t�H���g�ɖ߂�"
	};

	menuFuncTable_ = {
		{L"�߂�",&KeyconfigScene::CloseKeyConfig},
		{L"�m��",&KeyconfigScene::CommitAndClose},
		{L"�f�t�H���g�ɖ߂�",&KeyconfigScene::SetDefault},
	};

}

void KeyconfigScene::Update(Input& input)
{
	(this->*update_)(input);
}

void KeyconfigScene::Draw()
{
	(this->*draw_)();
}
