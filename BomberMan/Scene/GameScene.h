#pragma once
#include<vector>
#include<memory>
#include "Scene.h"
class Actor;
class GameScene : public Scene
{
private:
	int frame_=60;
	float angle_ = 0.0f;

	void FadeInUpdate(Input& input);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input& input);

	void FadeDraw();
	void NormalDraw();

	using UpdateFunc_t = void(GameScene::*)(Input& input);
	using DrawFunc_t = void(GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	int backH_;//�w�i�摜�̃n���h��

	//�Q�[�����̓o��l��(�z�u�\�I�u�W�F�N�g)
	std::vector<std::shared_ptr<Actor>> actors_;
	std::vector<std::shared_ptr<Actor>> additionalActors_;


	void DrawGround();
	/// <summary>
	/// �S�Ă�Actor�̓����蔻����s��
	/// </summary>
	void CheckHit();

	/// <summary>
	/// ��`���m���������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <param name="rcA">��`A</param>
	/// <param name="rcB">��`B</param>
	/// <returns>�������Ă�:true / �������ĂȂ�:false</returns>
	bool IsHit(const Rect& rcA,const Rect& rcB)const;
public:
	GameScene(SceneController& controller);
	/// <summary>
	/// �����ϐ������X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(Input& input)override;
	virtual void Draw()override;
	//����̏ꏊ�ɔ��e��u��
	void SetBomb(const Position2& pos);
	void SetBlastH(const Position2& pos, int power);
	void SetBlastV(const Position2& pos, int power);
};

