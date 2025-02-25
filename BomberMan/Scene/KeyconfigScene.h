#pragma once
#include "Scene.h"
#include<vector>
#include<string>
#include<map>
/// <summary>
/// キーコンフィグ画面シーン
/// </summary>
class KeyconfigScene : public Scene
{
private:
	int frame_ = 0;
	Input& input_;//Inputへの参照
	using UpdateFunc_t = void(KeyconfigScene::*)(Input& input);
	using DrawFunc_t = void(KeyconfigScene::*)();
	std::map<int, std::wstring> keyNameTable_;
	std::map<int, std::wstring> buttonNameTable_;

	unsigned int currentSelecterIndex_ = 0;//現在選択中の行番号

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	//出現中状態
	void AppearUpdate(Input& input);
	//ノーマル状態
	void NormalUpdate(Input& input);
	//消滅中状態
	void DisappearUpdate(Input& input);
	//編集中状態
	void EdittingUpdate(Input& input);


	//通常描画
	void NormalDraw();
	//途中描画
	void ShiftingDraw();
	
	/// <summary>
	/// Inputの情報を表示する
	/// </summary>
	void DrawInputInfo();

	void DrawStringWithShadow(int x, int y, const wchar_t* str, uint32_t color);

	void Close();
	using MenuFunc_t = void (KeyconfigScene::*)(Input&);
	//保存せずにキーコンフィグを閉じる
	//閉じた時点で、編集した内容は破棄されます
	void CloseKeyConfig(Input&);
	//編集した内容を確定します
	//キーの内容は変わり、変更は残り続けます
	void CommitAndClose(Input&);
	//完全にデフォルトに戻す
	void SetDefault(Input&);
	void CancelEdit(Input&);


	std::map<std::wstring, MenuFunc_t> menuFuncTable_;//メニューに対応した関数テーブル
	std::vector<std::wstring> menuString_;//メニューの文字列

public:
	KeyconfigScene(SceneController& controller,Input& input);
	/// <summary>
	/// 内部変数等を更新する
	/// </summary>
	/// <param name="input">入力状態</param>
	virtual void Update(Input& input)override;
	virtual void Draw()override;
};

