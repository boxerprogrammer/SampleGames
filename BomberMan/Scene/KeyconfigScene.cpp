#include "KeyconfigScene.h"
#include"../Application.h"
#include"../Input.h"
#include"SceneController.h"
#include"../StringUtility.h"
#include<DxLib.h>
namespace {
	constexpr int appear_interval = 20;//出現時間
	constexpr int frame_margin = 10;//ゲーム画面からポーズ画面までの幅
	constexpr int config_text_start_y = 120;//キーコンフィグの開始Yオフセット
	constexpr int config_text_left = 150;//キーコンフィグの左Xオフセット
	constexpr int config_row_height = 30;//キーコンフィグのメニュー１項目当たりの高さ
	constexpr int config_selector_left = 100;//キーコンフィグのセレクタ
	constexpr unsigned int base_color = 0xaaffaa;//キーコンフィグの背景カラー(明るい緑)
	constexpr unsigned int bg_alpha = 220;//キーコンフィグの背景α(ほぼ不透明)

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
	const uint32_t itemCount = static_cast<uint32_t>(input_.orderForDisplay_.size() + menuString_.size());//もとのキーの数＋「戻る」「確定」を用意
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
			//キー以外のメニューのインデックスを取得する
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
		//現在編集中のイベント名を取得する
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
		return;//わすれずにreturn
	}
}



void KeyconfigScene::NormalDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//元の高さを計算する
	const int frameHeight = wsize.h - frame_margin - frame_margin;
	//真ん中のY座標を計算する
	const int middleY = (frame_margin + wsize.h - frame_margin) / 2;//真ん中のY座標

	//割合計算
	float rate = 1.0f;

	//現在の時間であり得るべき高さの半分を計算します
	int halfHeight = static_cast<int>((frameHeight * rate) / 2.0f);

	//ここから、top(上)とbottom(下)を計算します
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;

	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, bg_alpha);
	DrawBox(10, top,//左上
		wsize.w - 10, bottom,//右下
		base_color,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポーズ画面枠
	DrawBoxAA(10.0f, static_cast<float>(top),//左上
		wsize.w - 10.0f, static_cast<float>(bottom),//右下
		0xffffff,//カラー
		false,//塗り潰さない
		3.0f);//ちょっと太目の線



	DrawString(30, 30, L"KeyConfig Scene", 0x000000);
	DrawInputInfo();
}

void KeyconfigScene::ShiftingDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	//元の高さを計算する
	const int frameHeight = wsize.h - frame_margin - frame_margin;
	//真ん中のY座標を計算する
	const int middleY = (frame_margin + wsize.h - frame_margin) / 2;//真ん中のY座標

	//割合計算
	float rate = static_cast<float>(frame_) / static_cast<float>(appear_interval);

	//現在の時間であり得るべき高さの半分を計算します
	int halfHeight = static_cast<int>((frameHeight * rate) / 2.0f);

	//ここから、top(上)とbottom(下)を計算します
	int top = middleY - halfHeight;
	int bottom = middleY + halfHeight;

	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, bg_alpha);
	DrawBox(10, top,//左上
		wsize.w - 10, bottom,//右下
		base_color,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ポーズ画面枠
	DrawBoxAA(10.0f, static_cast<float>(top),//左上
		wsize.w - 10.0f, static_cast<float>(bottom),//右下
		0xffffff,//カラー
		false,//塗り潰さない
		3.0f);//ちょっと太目の線
}

void KeyconfigScene::DrawInputInfo()
{
	int y = frame_margin + config_text_start_y;
	int x = frame_margin + config_text_left;

	constexpr int actual_id_margin = 80;//実際の入力コードのオフセット
	constexpr int actual_id_width = 160;//入力コードの幅
	int idx = 0;
	//必要なキー情報だけ表示する
	for (const auto& inputRow : input_.orderForDisplay_) {
		int selectOffset = 0;
		if (idx == currentSelecterIndex_) {
			DrawStringWithShadow(frame_margin + config_selector_left,y,	L"⇒",0xff0000);
			selectOffset = 5;
		}
		x = frame_margin + config_text_left;//x位置をリセット
		const auto& eventName = StringUtility::GetWStringFromString(inputRow);

		unsigned int textColor = 0x0000ff;
		if (update_ == &KeyconfigScene::EdittingUpdate) {
			if (currentSelecterIndex_ == idx) {
				textColor = 0xff0000;
				selectOffset = 10;
			}
		}
		//イベント名を表示
		DrawFormatString(selectOffset+x + 1, y + 1, 0x000000, L"%s", eventName.c_str());//影文字
		DrawFormatString(selectOffset+x, y, textColor, L"%s", eventName.c_str());//文字

		//実際の入力信号情報
		//inputTable内の特定のイベントにおける入力ID配列をまわす
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
				0x000000, L"%s=%s ", periTypeStr.c_str(), inputStr.c_str());//影文字
			DrawFormatString(selectOffset+actual_id_margin + x, y,
				textColor, L"%s=%s ", periTypeStr.c_str(), inputStr.c_str());//本体文字
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
	for (auto& str : menuString_) {//キー以外のメニュー文字列を回す
		int selectOffset = 0;
		if (currentSelecterIndex_ == eventSize+menuIndex) {
			DrawStringWithShadow(x - arrow_minus_offset, y, L"⇒", 0xff0000);
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
	DrawString(x + 1,//影
		y + 1,
		str,
		0x000000);
	DrawString(x,//本体⇒
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
	//キーコード⇔キー名称の変換テーブル
	keyNameTable_[KEY_INPUT_A] = L"Ａキー";
	keyNameTable_[KEY_INPUT_B] = L"Ｂキー";
	keyNameTable_[KEY_INPUT_C] = L"Ｃキー";
	keyNameTable_[KEY_INPUT_D] = L"Ｄキー";
	keyNameTable_[KEY_INPUT_E] = L"Ｅキー";
	keyNameTable_[KEY_INPUT_F] = L"Ｆキー";
	keyNameTable_[KEY_INPUT_G] = L"Ｇキー";
	keyNameTable_[KEY_INPUT_H] = L"Ｈキー";
	keyNameTable_[KEY_INPUT_I] = L"Ｉキー";
	keyNameTable_[KEY_INPUT_J] = L"Ｊキー";
	keyNameTable_[KEY_INPUT_K] = L"Ｋキー";
	keyNameTable_[KEY_INPUT_L] = L"Ｌキー";
	keyNameTable_[KEY_INPUT_M] = L"Ｍキー";
	keyNameTable_[KEY_INPUT_N] = L"Ｎキー";
	keyNameTable_[KEY_INPUT_O] = L"Ｏキー";
	keyNameTable_[KEY_INPUT_P] = L"Ｐキー";
	keyNameTable_[KEY_INPUT_Q] = L"Ｑキー";
	keyNameTable_[KEY_INPUT_R] = L"Ｒキー";
	keyNameTable_[KEY_INPUT_S] = L"Ｓキー";
	keyNameTable_[KEY_INPUT_T] = L"Ｔキー";
	keyNameTable_[KEY_INPUT_U] = L"Ｕキー";
	keyNameTable_[KEY_INPUT_V] = L"Ｖキー";
	keyNameTable_[KEY_INPUT_W] = L"Ｗキー";
	keyNameTable_[KEY_INPUT_X] = L"Ｘキー";
	keyNameTable_[KEY_INPUT_Y] = L"Ｙキー";
	keyNameTable_[KEY_INPUT_Z] = L"Ｚキー";
	keyNameTable_[KEY_INPUT_0] = L"０キー";
	keyNameTable_[KEY_INPUT_1] = L"１キー";
	keyNameTable_[KEY_INPUT_2] = L"２キー";
	keyNameTable_[KEY_INPUT_3] = L"３キー";
	keyNameTable_[KEY_INPUT_4] = L"４キー";
	keyNameTable_[KEY_INPUT_5] = L"５キー";
	keyNameTable_[KEY_INPUT_6] = L"６キー";
	keyNameTable_[KEY_INPUT_7] = L"７キー";
	keyNameTable_[KEY_INPUT_8] = L"８キー";
	keyNameTable_[KEY_INPUT_9] = L"９キー";
	keyNameTable_[KEY_INPUT_BACK] = L"BackSpaceキー";
	keyNameTable_[KEY_INPUT_TAB] = L"Tabキー";
	keyNameTable_[KEY_INPUT_RETURN] = L"Enterキー";
	keyNameTable_[KEY_INPUT_LSHIFT] = L"左Shiftキー";
	keyNameTable_[KEY_INPUT_RSHIFT] = L"右Shiftキー";
	keyNameTable_[KEY_INPUT_LCONTROL] = L"左Ctrlキー";
	keyNameTable_[KEY_INPUT_RCONTROL] = L"右Ctrlキー";
	keyNameTable_[KEY_INPUT_ESCAPE] = L"Escキー";
	keyNameTable_[KEY_INPUT_SPACE] = L"スペースキー";
	keyNameTable_[KEY_INPUT_PGUP] = L"PageUpキー";
	keyNameTable_[KEY_INPUT_PGDN] = L"PageDownキー";
	keyNameTable_[KEY_INPUT_END] = L"Endキー";
	keyNameTable_[KEY_INPUT_HOME] = L"Homeキー";
	keyNameTable_[KEY_INPUT_LEFT] = L"左キー";
	keyNameTable_[KEY_INPUT_UP] = L"上キー";
	keyNameTable_[KEY_INPUT_RIGHT] = L"右キー";
	keyNameTable_[KEY_INPUT_DOWN] = L"下キー";
	keyNameTable_[KEY_INPUT_INSERT] = L"Insertキー";
	keyNameTable_[KEY_INPUT_DELETE] = L"Deleteキー";
	
	//PADボタンID⇔PADボタン名変換テーブル
	buttonNameTable_[PAD_INPUT_A] = L"Ａボタン";
	buttonNameTable_[PAD_INPUT_B] = L"Ｂボタン";
	buttonNameTable_[PAD_INPUT_C] = L"Ｘボタン";
	buttonNameTable_[PAD_INPUT_X] = L"Ｙボタン";
	buttonNameTable_[PAD_INPUT_Y] = L"L1ボタン";
	buttonNameTable_[PAD_INPUT_Z] = L"R1ボタン";
	buttonNameTable_[PAD_INPUT_L] = L"SELECTボタン";
	buttonNameTable_[PAD_INPUT_R] = L"STARTボタン";
	buttonNameTable_[PAD_INPUT_START] = L"左押込み";
	buttonNameTable_[PAD_INPUT_M] = L"左押込み";

	menuString_ = { L"戻る",
					L"確定" ,
					L"デフォルトに戻す"
	};

	menuFuncTable_ = {
		{L"戻る",&KeyconfigScene::CloseKeyConfig},
		{L"確定",&KeyconfigScene::CommitAndClose},
		{L"デフォルトに戻す",&KeyconfigScene::SetDefault},
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
