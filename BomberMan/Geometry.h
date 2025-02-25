#pragma once
//ゲーム中で使用する幾何学情報の型を定義するファイル

///ベクトル(2D)
struct Vector2 {
	float x;
	float y;

	/// <summary>
	/// ベクトル同士の加算
	/// </summary>
	/// <param name="rval">右辺値</param>
	/// <returns>和</returns>
	Vector2 operator+(const Vector2& rval)const;

	/// <summary>
	/// ベクトル同士の減算
	/// </summary>
	/// <param name="rval">右辺値</param>
	/// <returns>差</returns>
	Vector2 operator-(const Vector2& rval)const;

	/// <summary>
	/// 自分に対象ベクトルを足す
	/// </summary>
	/// <param name="vec">自分に足すベクトル</param>
	void operator+=(const Vector2& vec);

	/// <summary>
	/// 自分から対象ベクトルを引く
	/// </summary>
	/// <param name="vec"></param>
	void operator-=(const Vector2& vec);

	/// <summary>
	/// ベクトルのスカラー倍(実数倍)
	/// </summary>
	/// <param name="scale">倍数</param>
	/// <returns>スカラー倍されたベクトル</returns>
	Vector2 operator*(float scale)const;

	/// <summary>
	/// 自分自身をスカラー倍する
	/// </summary>
	/// <param name="scale">倍数</param>
	void operator*=(float scale);

	/// <summary>
	/// ベクトルを実数で割る
	/// </summary>
	/// <param name="div">割る値</param>
	/// <returns>割った結果(除)</returns>
	Vector2 operator/(float div)const;

	/// <summary>
	/// 自分自身を実数で割る
	/// </summary>
	/// <param name="div">割る値</param>
	void operator/=(float div);

	/// <summary>
	/// ベクトルの正規化(大きさを１にする)
	/// </summary>
	void Normalize();

	/// <summary>
	/// 自分を正規化したベクトルを返す
	/// </summary>
	/// <returns>正規化ベクトル</returns>
	Vector2 Normalized()const;

	/// <summary>
	/// ベクトルの大きさを測る
	/// </summary>
	/// <returns></returns>
	float Length()const;
	
};

using Position2 = Vector2;


///サイズ(2Dゲームで使用するピクセルサイズ1)
struct Size {
	int w;
	int h;
};
/// <summary>
/// 矩形(くけい)構造体
/// 矩形とは長方形の事
/// </summary>
struct Rect {
	Position2 center;//中心点
	Size size;//幅と高さ
	float Left()const;
	float Top()const;
	float Right()const;
	float Bottom()const;
	Rect() {};
	Rect(const Position2& c, const Size& sz);
	void SetLTRB(float l, float t, float r, float b);
	/// <summary>
	/// デバッグ用矩形表示
	/// </summary>
	/// <param name="col">カラー</param>
	void Draw(unsigned int col = 0xffffff)const;
};

Size GetOverlappedSize(const Rect& rcA, const Rect& rcB);
Vector2 GetAdjustVector(const Rect& rcA, const Rect& rcB);

