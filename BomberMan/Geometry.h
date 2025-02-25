#pragma once
//�Q�[�����Ŏg�p����􉽊w���̌^���`����t�@�C��

///�x�N�g��(2D)
struct Vector2 {
	float x;
	float y;

	/// <summary>
	/// �x�N�g�����m�̉��Z
	/// </summary>
	/// <param name="rval">�E�Ӓl</param>
	/// <returns>�a</returns>
	Vector2 operator+(const Vector2& rval)const;

	/// <summary>
	/// �x�N�g�����m�̌��Z
	/// </summary>
	/// <param name="rval">�E�Ӓl</param>
	/// <returns>��</returns>
	Vector2 operator-(const Vector2& rval)const;

	/// <summary>
	/// �����ɑΏۃx�N�g���𑫂�
	/// </summary>
	/// <param name="vec">�����ɑ����x�N�g��</param>
	void operator+=(const Vector2& vec);

	/// <summary>
	/// ��������Ώۃx�N�g��������
	/// </summary>
	/// <param name="vec"></param>
	void operator-=(const Vector2& vec);

	/// <summary>
	/// �x�N�g���̃X�J���[�{(�����{)
	/// </summary>
	/// <param name="scale">�{��</param>
	/// <returns>�X�J���[�{���ꂽ�x�N�g��</returns>
	Vector2 operator*(float scale)const;

	/// <summary>
	/// �������g���X�J���[�{����
	/// </summary>
	/// <param name="scale">�{��</param>
	void operator*=(float scale);

	/// <summary>
	/// �x�N�g���������Ŋ���
	/// </summary>
	/// <param name="div">����l</param>
	/// <returns>����������(��)</returns>
	Vector2 operator/(float div)const;

	/// <summary>
	/// �������g�������Ŋ���
	/// </summary>
	/// <param name="div">����l</param>
	void operator/=(float div);

	/// <summary>
	/// �x�N�g���̐��K��(�傫�����P�ɂ���)
	/// </summary>
	void Normalize();

	/// <summary>
	/// �����𐳋K�������x�N�g����Ԃ�
	/// </summary>
	/// <returns>���K���x�N�g��</returns>
	Vector2 Normalized()const;

	/// <summary>
	/// �x�N�g���̑傫���𑪂�
	/// </summary>
	/// <returns></returns>
	float Length()const;
	
};

using Position2 = Vector2;


///�T�C�Y(2D�Q�[���Ŏg�p����s�N�Z���T�C�Y1)
struct Size {
	int w;
	int h;
};
/// <summary>
/// ��`(������)�\����
/// ��`�Ƃ͒����`�̎�
/// </summary>
struct Rect {
	Position2 center;//���S�_
	Size size;//���ƍ���
	float Left()const;
	float Top()const;
	float Right()const;
	float Bottom()const;
	Rect() {};
	Rect(const Position2& c, const Size& sz);
	void SetLTRB(float l, float t, float r, float b);
	/// <summary>
	/// �f�o�b�O�p��`�\��
	/// </summary>
	/// <param name="col">�J���[</param>
	void Draw(unsigned int col = 0xffffff)const;
};

Size GetOverlappedSize(const Rect& rcA, const Rect& rcB);
Vector2 GetAdjustVector(const Rect& rcA, const Rect& rcB);

