#pragma once
#include "Actor.h"
class Bomb :
    public Actor
{
	int power_ = 2;
	int handle_=0;
	int animFrame_=0;
	using UpdateFunc_t = void (Bomb::*)();

	UpdateFunc_t update_;

	void WaitUpdate();
	void BurstUpdate();

public:
	Bomb(GameScene& gameScene, const Position2& pos);
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

