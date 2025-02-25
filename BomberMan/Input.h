#pragma once
#include<map>
#include<string>
#include<vector>

//おともだちをプロトタイプ宣言
class KeyconfigScene;

/// <summary>
/// 入力系をコントロールするクラス
/// </summary>
class Input
{
	friend KeyconfigScene;//KeyconfigSceneをお友達認定…privateでもprotectedでもアクセス可能
private:
	///周辺機器種別
	enum class PeripheralType {
		keybd,//キーボード
		pad1//パッド１
	};
	///入力情報定義用
	struct InputState {
		PeripheralType type;//周辺機器種別
		int id;//実際の入力を表す定数
	};

	//Table=表
	using InputTable_t = std::map<std::string, std::vector<InputState>>;
	InputTable_t inputTable_;//名前と実際の入力の対応表
	InputTable_t tempInputTable_;//名前と実際の入力の対応表(一時的なコピー)
	
	void SetDafault();

	/// <summary>
	/// キーコンフィグ用(行った変更を確定します)
	/// </summary>
	void CommitEdittedInputTable();

	/// <summary>
	/// キーコンフィグ用(行った変更をなかったことにします)
	/// </summary>
	void RollbackEdittedInputTable();

	//Record=記録
	//押されたかどうかを記録していくもの
	using InputRecord_t = std::map<std::string, bool>;
	InputRecord_t current_;//現在押してるかどうか
	InputRecord_t last_;//直前に押されてたかどうか

	std::vector<std::string> orderForDisplay_;//表示用(mapの順序はコントロールできないため)

	//キーコンフィグ用
	int currentRawPadState_;//今の生パッドステート
	int lastRawPadState_;//直前の生パッドステート
	char currentRawKeybdState_[256];//今のキーボード状態
	char lastRawKeybdState_[256];//直前のキーボード状態

	/// <summary>
	/// 現在のキーボード状態を調べて最も
	/// 若いキーコードを返します
	/// ひとつも押されてなければ-1を返す
	/// </summary>
	/// <returns></returns>
	int GetKeyboradState()const;

	/// <summary>
	/// 現在のPAD状態を調べて最も
	/// 若いキー入力を返します
	/// ひとつも押されてなければ-1を返す
	/// </summary>
	int GetPadState(int padno)const;

	/// <summary>
	/// 現在の入力テーブルをセーブします
	/// </summary>
	void SaveInputTable();

	/// <summary>
	/// 入力テーブルをロードして反映します
	/// </summary>
	void LoadInputTable();

public:
	Input();

	/// <summary>
	/// 入力状態を更新する
	/// </summary>
	void Update();

	/// <summary>
	/// 押した瞬間を検出する
	/// </summary>
	/// <param name="key">イベント名</param>
	/// <returns>true:押した瞬間 false:押されてないor押しっぱなし</returns>
	bool IsTrigger(const char* key)const;

	/// <summary>
	/// 今現在押されているかどうかを検出する
	/// </summary>
	/// <param name="key">イベント名</param>
	/// <returns>true:押されている / false:押されていない</returns>
	bool IsPressed(const char* key) const;
};

