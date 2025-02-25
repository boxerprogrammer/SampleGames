#pragma once
#include<memory>
#include<list>
class Scene;
class Input;
/// <summary>
/// �e�V�[�����Ǘ�����N���X
/// �Ǘ��͂��邪�A�J�ڂ��Ǘ��͂��Ȃ��B
/// �C���^�[�t�F�[�X��p�ӂ���݂̂̃N���X
/// </summary>
class SceneController
{
private:
	using SceneStack_t = std::list<std::shared_ptr<Scene>>;//�V�[�����X�^�b�N�\���ɂ��邽�߂̌^
	SceneStack_t scenes_;//���ݎ��s���̃V�[��(���������͒m��Ȃ�)
public:
	SceneController();

	/// <summary>
	/// Application����Ăяo�����Update�ŁA������Scene��Update���Ăяo������
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(Input& input);

	/// <summary>
	/// Application����Ăяo�����Draw�ŁA������Scene��Draw���Ăяo������
	/// </summary>
	void Draw();

	/// <summary>
	/// ���̏�Ԃ��Z�b�g����(�Z�b�g����̂͊e��Ԃ̖���)
	/// </summary>
	/// <param name="scene">���̏��(�V�[��)�̃|�C���^(�X�}�|)</param>
	void ChangeScene(std::shared_ptr<Scene> scene);
	//��ԉ��ɂȂ��Ă���V�[����؂�ւ���
	//�|�[�Y����^�C�g���V�[���ɖ߂�Ƃ����Ɏg�p����
	
	/// <summary>
	/// ��ԉ��ɂȂ��Ă���V�[����؂�ւ���
	/// �|�[�Y����^�C�g���V�[���ɖ߂�Ƃ����Ɏg�p����
	/// </summary>
	/// <param name="scene">�x�[�X��؂�ւ������V�[��</param>
	void ChangeBaseScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// ���ݎ��s���̃V�[���̏�ɕʂ̃V�[�����������
	/// </summary>
	/// <param name="scene">����������V�[��</param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// ���݁A�\��(��ԏ�)�ɂ�����s���̃V�[������菜��
	/// </summary>
	void PopScene();
};

