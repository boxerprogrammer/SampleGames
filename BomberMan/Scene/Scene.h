#pragma once
class Input;
class SceneController;
/// <summary>
/// �e�V�[���̊��N���X
/// </summary>
class Scene
{
protected:
	//�e��Ԃ��؂�ւ��邽�߂�
	//�V�[���R���g���[���̎Q�Ƃ������Ă���
	SceneController& controller_;
public:
	Scene(SceneController& controller);
	/// <summary>
	/// �����ϐ������X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(Input& input)=0;

	/// <summary>
	/// �`��S�ʂ��s��
	/// </summary>
	virtual void Draw()=0;
};


