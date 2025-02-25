#pragma once
#include "Actor.h"
enum class BlastDirection {
    Horizontal,
	Vertical,
};
/// <summary>
/// ����
/// </summary>
class Blast : public Actor
{
private:
	int frame_=0;
	int power_ = 1;
	BlastDirection dir_;
	using UpdateFunc_t = void (Blast::*)();
	using DrawFunc_t = void (Blast::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void SpreadUpdate();
	void KeepUpdate();
	void DisappearUpdate();

	void SpreadDraw();
	void KeepDraw();
	void DisappearDraw();

public:
	Blast(GameScene& gameScene, const Position2& pos,BlastDirection dir=BlastDirection::Horizontal,int power = 1);
	/// <summary>
	/// �ʒu�Ȃǂ̍X�V���s���N���X
	/// </summary>
	/// <param name="input">���̓I�u�W�F�N�g</param>
	virtual void Update(Input& input)override;
	/// <summary>
	/// �Ώۂ̃A�N�^�[��\������
	/// </summary>
	virtual void Draw()override;

	/// <summary>
	/// �����蔻��ɔ�������C�x���g�֐�
	/// </summary>
	/// <param name="collision">�����蔻��I�u�W�F�N�g</param>
	virtual void OnHit(const Collision& collision)override;
};

