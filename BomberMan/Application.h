#pragma once
#include"Geometry.h"
/// <summary>
/// アプリケーション全体を管理するための
/// シングルトンクラス
/// </summary>
class Application
{
private:
	Size windowSize_;
	//シングルトンのためにコンストラクタ等をprivateに
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;
public:
	/// <summary>
	/// シングルトンオブジェクトを返す
	/// </summary>
	/// <returns>Applicationシングルトンオブジェクト</returns>
	static Application& GetInstance();
	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>true:初期化成功 / false:初期化失敗</returns>
	bool Init();
	
	/// <summary>
	/// メインループを起動
	/// </summary>
	void Run();

	/// <summary>
	/// アプリケーションの後処理
	/// </summary>
	void Terminate();

	const Size& GetWindowSize()const;

};



