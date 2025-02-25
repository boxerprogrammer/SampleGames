#pragma once
#include<memory>
#include<list>
class Scene;
class Input;
/// <summary>
/// 各シーンを管理するクラス
/// 管理はするが、遷移を管理はしない。
/// インターフェースを用意するのみのクラス
/// </summary>
class SceneController
{
private:
	using SceneStack_t = std::list<std::shared_ptr<Scene>>;//シーンをスタック構造にするための型
	SceneStack_t scenes_;//現在実行中のシーン(中が何かは知らない)
public:
	SceneController();

	/// <summary>
	/// Applicationから呼び出されるUpdateで、内部のSceneのUpdateを呼び出すだけ
	/// </summary>
	/// <param name="input">入力状態</param>
	void Update(Input& input);

	/// <summary>
	/// Applicationから呼び出されるDrawで、内部のSceneのDrawを呼び出すだけ
	/// </summary>
	void Draw();

	/// <summary>
	/// 次の状態をセットする(セットするのは各状態の役割)
	/// </summary>
	/// <param name="scene">次の状態(シーン)のポインタ(スマポ)</param>
	void ChangeScene(std::shared_ptr<Scene> scene);
	//一番下になっているシーンを切り替える
	//ポーズからタイトルシーンに戻るとき等に使用する
	
	/// <summary>
	/// 一番下になっているシーンを切り替える
	/// ポーズからタイトルシーンに戻るとき等に使用する
	/// </summary>
	/// <param name="scene">ベースを切り替えたいシーン</param>
	void ChangeBaseScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// 現在実行中のシーンの上に別のシーンを乗っける
	/// </summary>
	/// <param name="scene">乗っけたいシーン</param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// 現在、表面(一番上)にある実行中のシーンを取り除く
	/// </summary>
	void PopScene();
};

