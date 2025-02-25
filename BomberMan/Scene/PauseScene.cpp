#include<DxLib.h>
#include"../Input.h"
#include"../Application.h"
#include "PauseScene.h"
#include"SceneController.h"
#include"TitleScene.h"
#include"KeyconfigScene.h"

namespace {
	constexpr int appear_interval = 20;
	constexpr int frame_margin = 10;//ゲーム画面からポーズ画面までの幅
	constexpr int menu_text_start_y = 150;//メニューの開始Yオフセット
	constexpr int menu_text_left = 250;//メニューの左Xオフセット
	constexpr int menu_row_height = 50;//メニュー１項目当たりの高さ
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
	//Pボタンでポーズ解除
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
		return;//わすれずにreturn
	}
}

void PauseScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(10, 10,//左上
		wsize.w - 10, wsize.h - 10,//右下
		0xffffff,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポーズ画面枠
	DrawBoxAA(10.0f, 10.0f,//左上
		wsize.w - 10.0f, wsize.h - 10.0f,//右下
		0xffffff,//カラー
		false,//塗り潰さない
		3.0f);//ちょっと太目の線

	DrawString(30, 30, L" Pause ", 0xffffff);


	DrawMenu();
}

void PauseScene::ShiftingDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//元の高さを計算する
	const int frameHeight = wsize.h - 10 - 10;
	//真ん中のY座標を計算する
	const int middleY = (10 + wsize.h - 10) / 2;//真ん中のY座標

	//割合計算
	float rate = static_cast<float>(frame_) / static_cast<float>(appear_interval);

	//現在の時間であり得るべき高さの半分を計算します
	int halfHeight = static_cast<int>((frameHeight * rate) *0.5f);

	//ここから、top(上)とbottom(下)を計算します
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;

	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
	DrawBox(10, top,//左上
		wsize.w - 10, bottom,//右下
		0xffffff,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポーズ画面枠
	DrawBoxAA(10.0f, static_cast<float>(top),//左上
		wsize.w - 10.0f, static_cast<float>(bottom),//右下
		0xffffff,//カラー
		false,//塗り潰さない
		3.0f);//ちょっと太目の線
}

void PauseScene::DrawMenu()
{
	int x = frame_margin + menu_text_left;//メニュー項目表示位置X
	int y = frame_margin + menu_text_start_y;//メニュー項目表示位置Y
	
	for (int i = 0; i < menuTexts_.size(); ++i) {
		int select_offset = 0;
		unsigned int menu_row_color = 0x0000ff;//選ばれてなければ青
		//セレクターの表示
		if (i == currentMenuIndex_) {
			DrawString(frame_margin + menu_selector_left + 1,//影
				y + 1,
				L"⇒",
				0x000000);
			DrawString(frame_margin + menu_selector_left,//本体⇒
				y,
				L"⇒",
				0xff0000);
			select_offset = 10;//選ばれた行だけ右にちょっとずらす
			menu_row_color = 0xbb0088;//選ばれていたら赤紫
		}
		const auto& text = menuTexts_[i];
		DrawFormatString(select_offset+x + 1, y + 1, 0x000000, L"%s", text.c_str());//影文字
		DrawFormatString(select_offset+x, y, menu_row_color, L"%s", text.c_str());//文字
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
	//自分の下になってるシーンを切り替える
	controller_.ChangeBaseScene(std::make_shared<TitleScene>(controller_));
	controller_.PopScene();//自分は消える
}

PauseScene::PauseScene(SceneController& controller):
	Scene(controller),
	update_(&PauseScene::AppearUpdate),
	draw_(&PauseScene::ShiftingDraw),
	frame_(0)
{
	menuTexts_={//順番を調整するため
		L"ゲームに戻る",
		L"キーコンフィグ",
		L"その他設定",
		L"タイトルに戻る",
		
	};
	functionTable_ = {//メニューの文言と実行される関数の対応表
		{L"ゲームに戻る",&PauseScene::BackGame },
		{L"キーコンフィグ",&PauseScene::GoKeyConfig},
		{L"その他設定",&PauseScene::EtcConfig},
		{L"タイトルに戻る",&PauseScene::BackTitle}
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
