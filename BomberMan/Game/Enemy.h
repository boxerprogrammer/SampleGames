#pragma once
#include "Actor.h"
class Enemy :  public Actor
{
	int idleH_ = 0;//�ҋ@��Ԃ̉摜�n���h��
	int walkH_ = 0;//������Ԃ̉摜�n���h��
	int deathH_ = 0;//���S��Ԃ̉摜�n���h��
	int animFrame_ = 0;//�A�j���[�V�����p�t���[���ԍ�

	//�u��ԁv�J�ڂ̂��߂̕ϐ�
	using UpdateFunc_t = void (Enemy::*)(Input&);
	using DrawFunc_t = void (Enemy::*)();

	//�ҋ@���
	void IdleUpdate(Input&);
	void IdleDraw();

	//�������
	void WalkUpdate(Input&);
	void WalkDraw();

	//���ɏ��
	void DeadUpdate(Input&);
	void DeadDraw();

	//��Ԃɍ��킹�������o�֐��|�C���^
	UpdateFunc_t update_;
	DrawFunc_t draw_;
	/// <summary>
	/// �ړ��֐�
	/// </summary>
	bool Move(Input& input);
public:
	Enemy(GameScene& gameScene);
	~Enemy();

	//Actor����p�������֐�
	//���W���̍X�V
	void Update(Input& input)override;

	//�`��
	void Draw()override;
	//�����������̔���
	void OnHit(const Collision& coll)override;

	void SetPosition(const Position2& pos)override;
};

