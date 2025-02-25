#pragma once
#include "Scene.h"
#include<vector>
#include<string>
#include<map>
/// <summary>
/// ポーズ中シーンクラス
/// </summary>
class PauseScene : public Scene
{

private:
	int frame_ = 0;
	using UpdateFunc_t = void(PauseScene::*)(Input& input);
	using DrawFunc_t = void(PauseScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	//出現中状態
	void AppearUpdate(Input& input);
	//ノーマル状態
	void NormalUpdate(Input& input);
	//消滅中状態
	void DisappearUpdate(Input& input);

	//通常描画
	void NormalDraw();
	//途中描画
	void ShiftingDraw();


	using MenuFunc_t = void(PauseScene::*)(Input&);
	void BackGame(Input&);//ゲームに戻る
	void GoKeyConfig(Input&);//キーコンフィグを立ち上げる
	void EtcConfig(Input&);//その他(今は何もしない。あるだけ)
	void BackTitle(Input&);//タイトルに戻る

	unsigned int currentMenuIndex_ = 0;//今どのメニュー項目を選んでいるかインデックス
	//メニューを並べるためのvector
	std::vector<std::wstring> menuTexts_;

	//メニューと対応する関数をセットにしたテーブル
	std::map<std::wstring, MenuFunc_t> functionTable_;

	void DrawMenu();



public:
	PauseScene(SceneController& controller);
	/// <summary>
	/// 内部変数等を更新する
	/// </summary>
	/// <param name="input">入力状態</param>
	virtual void Update(Input& input)override;
	virtual void Draw()override;
};

