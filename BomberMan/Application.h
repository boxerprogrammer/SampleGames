#pragma once
#include"Geometry.h"
/// <summary>
/// �A�v���P�[�V�����S�̂��Ǘ����邽�߂�
/// �V���O���g���N���X
/// </summary>
class Application
{
private:
	Size windowSize_;
	//�V���O���g���̂��߂ɃR���X�g���N�^����private��
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;
public:
	/// <summary>
	/// �V���O���g���I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <returns>Application�V���O���g���I�u�W�F�N�g</returns>
	static Application& GetInstance();
	/// <summary>
	/// �A�v���P�[�V�����̏�����
	/// </summary>
	/// <returns>true:���������� / false:���������s</returns>
	bool Init();
	
	/// <summary>
	/// ���C�����[�v���N��
	/// </summary>
	void Run();

	/// <summary>
	/// �A�v���P�[�V�����̌㏈��
	/// </summary>
	void Terminate();

	const Size& GetWindowSize()const;

};



