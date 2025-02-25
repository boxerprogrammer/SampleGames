#pragma once
#include "Actor.h"
class Player : public Actor
{
private:
	//�v���C���[�̌���(�`��p)
	enum class Direction {
		down,//��=0
		left,//��=1
		leftup,//����=2
		up,//��=3
		rightup,//�E��=4
		right,//�E=5
	};
	Direction currentDir_ = Direction::down;//���݌����Ă������
	int idleH_ = 0;//�ҋ@��Ԃ̉摜�n���h��
	int walkH_ = 0;//������Ԃ̉摜�n���h��
	int deathH_ = 0;//���ɏ�Ԃ̉摜�n���h��
	int animFrame_=0;//�A�j���[�V�����p�t���[���ԍ�
	int frame_ = 0;//�ėp�t���[��

	//�u��ԁv�J�ڂ̂��߂̕ϐ�
	using UpdateFunc_t = void (Player::*)(Input&);
	using DrawFunc_t = void (Player::*)();

	//�ҋ@���
	void IdleUpdate(Input&);
	void IdleDraw();

	//�������
	void WalkUpdate(Input&);
	void WalkDraw();

	//���ɏ��
	void DeadUpdate(Input&);
	void DeadDraw();
	void DeadAfterUpdate(Input&);//���񂾌�̗]�C

	//��Ԃɍ��킹�������o�֐��|�C���^
	UpdateFunc_t update_;
	DrawFunc_t draw_;
	/// <summary>
	/// �ړ��֐�
	/// </summary>
	bool Move(Input& input);
	void OnHitBlock(const Collision& coll);
public:
	Player(GameScene& gameScene);
	~Player();

	//Actor����p�������֐�
	//���W���̍X�V
	void Update(Input& input)override;
	
	//�`��
	void Draw()override;
	//�����������̔���
	void OnHit(const Collision& coll)override;
	
	void SetPosition(const Position2& pos)override;
};

