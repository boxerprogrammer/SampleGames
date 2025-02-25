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
	static Application app;//この宣言の時点でメモリが確定されてアプリ終了まで残る
	return app;
}

bool Application::Init()
{

	SetGraphMode(windowSize_.w, windowSize_.h, 32);
	ChangeWindowMode(true);
	SetWindowText(L"なんかカッコイイゲーム");
	SetWindowIconID(IDI_ICON2);
	
	if (DxLib_Init() == -1) {
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//string→wstringへの変換
	//std::string path = "./img/dokuro.png";
	//auto wpath = StringUtility::GetWStringFromString(path);
	//imgH_ = LoadGraph(wpath.c_str());
	//assert(imgH_ >= 0);
	
	//連番のロード
	//for (int i = 0; i < _countof(heroH_); ++i) {
	//	std::string str = "./img/hero/adventurer-die-06.png";//00～04
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
	auto lastTime=GetTickCount64();///1000分の1秒が返る
	//このループは基本的に60FPSに固定したい
	//60FPSは1秒間に60ループなので、1000/60=16.666666
	constexpr uint64_t frame_milliseconds = 16;
	while (ProcessMessage() != -1) {//メインループ
		ClearDrawScreen();
		
		input.Update();//入力情報の更新
		controller.Update(input);
		controller.Draw();

		//int idx = (frame / 5) % 7;
		//DrawGraph(100, 100, imgH_, true);
		//DrawRotaGraph(200, 200, 3.0f, 0.0f, heroH_[idx], true);

		DrawFormatString(0, 0, 0xffffff, L"FPS=%2.2f", GetFPS());
		//待ちを行う

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
