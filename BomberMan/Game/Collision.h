#pragma once
#include"../Geometry.h"
class Actor;

/// <summary>
/// �Փ˃^�C�v
/// </summary>
enum class CollisionType {
	Player,//�v���C���[
	Block,//�u���b�N
	Enemy,//�G
	Bomb,//���e
	Blast//����
};

//�����蔻�葮����^����N���X
class Collision
{
private:
	CollisionType type_;//�����蔻��^�C�v
	Rect rect_;//�������`
	Actor& owner_;//������ւ̎Q��
	bool isMoved_=false;//��������
	bool isDisable_ = false;//�g�p�֎~��Ԃ��H
public:
	Collision(Actor& owner, const Rect& rect, CollisionType type);
	Collision(Actor& owner);
	void SetRect(const Rect& rc);
	void SetType(CollisionType type);
	void SetPosition(const Position2& pos);
	CollisionType GetType()const;
	const Rect& GetRect()const;
	bool IsMoved()const;
	/// <summary>
	/// �����蔻��͖����ł����H
	/// </summary>
	/// <returns>true:���� / false:�����ł͂Ȃ�</returns>
	bool IsDisable()const;
	/// <summary>
	/// �����蔻��𖳌��ɂ���
	/// </summary>
	void Disable();

	/// <summary>
	/// �����蔻���L���ɂ���
	/// </summary>
	void Enable();
};

