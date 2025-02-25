#pragma once
class Input;
class SceneController;
/// <summary>
/// 各シーンの基底クラス
/// </summary>
class Scene
{
protected:
	//各状態が切り替えるために
	//シーンコントローラの参照を持っておく
	SceneController& controller_;
public:
	Scene(SceneController& controller);
	/// <summary>
	/// 内部変数等を更新する
	/// </summary>
	/// <param name="input">入力状態</param>
	virtual void Update(Input& input)=0;

	/// <summary>
	/// 描画全般を行う
	/// </summary>
	virtual void Draw()=0;
};


