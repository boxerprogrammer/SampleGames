#pragma once
class GameScene;//�Q�[���V�[��
class Input;//����
#include"Collision.h"
/// <summary>
/// �Q�[�����ɔz�u�\�ȕ��̂̊��N���X
/// </summary>
class Actor
{
protected:
	GameScene& gameScene_;//������ł���Q�[���V�[���̎Q�Ƃ�����
	Position2 pos_;//�A�N�^�[�̌��ݍ��W(���S�_�Ƃ���)
	Collision collision_;
public:
	Actor(GameScene&);
	virtual ~Actor() {}

	virtual void SetPosition(const Position2& pos);

	/// <summary>
	/// �ʒu�Ȃǂ̍X�V���s���N���X
	/// </summary>
	/// <param name="input">���̓I�u�W�F�N�g</param>
	virtual void Update(Input& input) = 0;
	/// <summary>
	/// �Ώۂ̃A�N�^�[��\������
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �����蔻��ɔ�������C�x���g�֐�
	/// </summary>
	/// <param name="collision">�����蔻��I�u�W�F�N�g</param>
	virtual void OnHit(const Collision& collision) = 0;

	const Collision& GetCollision()const;

};

