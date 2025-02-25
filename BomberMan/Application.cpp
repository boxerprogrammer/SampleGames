#include<DxLib.h>
#include<string>
#include<cassert>
#include "Application.h"
#include"Scene/SceneController.h"
#include"Input.h"
#include"StringUtility.h"
#include"resource.h"


Application::Application() :windowSize_({ 640,480 }) {
}

Application&
Application::GetInstance()
{
	static Application app;//���̐錾�̎��_�Ń��������m�肳��ăA�v���I���܂Ŏc��
	return app;
}

bool Application::Init()
{

	SetGraphMode(windowSize_.w, windowSize_.h, 32);
	ChangeWindowMode(true);
	SetWindowText(L"�Ȃ񂩃J�b�R�C�C�Q�[��");
	SetWindowIconID(IDI_ICON2);
	
	if (DxLib_Init() == -1) {
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//string��wstring�ւ̕ϊ�
	//std::string path = "./img/dokuro.png";
	//auto wpath = StringUtility::GetWStringFromString(path);
	//imgH_ = LoadGraph(wpath.c_str());
	//assert(imgH_ >= 0);
	
	//�A�Ԃ̃��[�h
	//for (int i = 0; i < _countof(heroH_); ++i) {
	//	std::string str = "./img/hero/adventurer-die-06.png";//00�`04
	//	str.push_back('\0');
	//	sprintf_s(str.data(), str.size(), "./img/hero/adventurer-die-%02d.png", i);
	//	heroH_[i] = LoadGraph(StringUtility::GetWStringFromString(str).c_str());
	//	assert(heroH_[i] >= 0);
	//}
	return true;
}

void Application::Run()
{
	SceneController controller;
	Input input;
	auto lastTime=GetTickCount64();///1000����1�b���Ԃ�
	//���̃��[�v�͊�{�I��60FPS�ɌŒ肵����
	//60FPS��1�b�Ԃ�60���[�v�Ȃ̂ŁA1000/60=16.666666
	constexpr uint64_t frame_milliseconds = 16;
	while (ProcessMessage() != -1) {//���C�����[�v
		ClearDrawScreen();
		
		input.Update();//���͏��̍X�V
		controller.Update(input);
		controller.Draw();

		//int idx = (frame / 5) % 7;
		//DrawGraph(100, 100, imgH_, true);
		//DrawRotaGraph(200, 200, 3.0f, 0.0f, heroH_[idx], true);

		DrawFormatString(0, 0, 0xffffff, L"FPS=%2.2f", GetFPS());
		//�҂����s��

		auto nowTime = GetTickCount64();
		if (nowTime < lastTime + frame_milliseconds) {
			Sleep(static_cast<uint32_t>(lastTime + frame_milliseconds-nowTime));
		}
		lastTime = GetTickCount64();

		ScreenFlip();
	}
}

void Application::Terminate()
{
	DxLib_End();
}

const Size& Application::GetWindowSize() const
{
	return windowSize_;
}
