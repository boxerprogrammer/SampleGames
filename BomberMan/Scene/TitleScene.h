#pragma once
#include "Scene.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
private:
	int frame_ = 0;
	int backH_ = -1;//�w�i�̃n���h��

	using UpdateFunc_t = void(TitleScene::*)(Input& );
	using DrawFunc_t = void(TitleScene::*)();

	UpdateFunc_t update_;//���݂̏�Ԃ�\���֐��|�C���^
	DrawFunc_t draw_;//�`��֐��|�C���^

	//�t�F�[�h�C�����̍X�V����
	void FadeinUpdate(Input&);
	//�ʏ�X�V����
	void NormalUpdate(Input&);
	//�t�F�[�h�A�E�g�X�V����
	void FadeoutUpdate(Input&);

	//�t�F�[�h���̕`��
	void FadeDraw();
	//�ʏ�`��
	void NormalDraw();

public:
	TitleScene(SceneController& controller);
	/// <summary>
	/// �����ϐ������X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(Input& input)override;
	virtual void Draw()override;
};

