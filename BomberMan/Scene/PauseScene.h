#pragma once
#include "Scene.h"
#include<vector>
#include<string>
#include<map>
/// <summary>
/// �|�[�Y���V�[���N���X
/// </summary>
class PauseScene : public Scene
{

private:
	int frame_ = 0;
	using UpdateFunc_t = void(PauseScene::*)(Input& input);
	using DrawFunc_t = void(PauseScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	//�o�������
	void AppearUpdate(Input& input);
	//�m�[�}�����
	void NormalUpdate(Input& input);
	//���Œ����
	void DisappearUpdate(Input& input);

	//�ʏ�`��
	void NormalDraw();
	//�r���`��
	void ShiftingDraw();


	using MenuFunc_t = void(PauseScene::*)(Input&);
	void BackGame(Input&);//�Q�[���ɖ߂�
	void GoKeyConfig(Input&);//�L�[�R���t�B�O�𗧂��グ��
	void EtcConfig(Input&);//���̑�(���͉������Ȃ��B���邾��)
	void BackTitle(Input&);//�^�C�g���ɖ߂�

	unsigned int currentMenuIndex_ = 0;//���ǂ̃��j���[���ڂ�I��ł��邩�C���f�b�N�X
	//���j���[����ׂ邽�߂�vector
	std::vector<std::wstring> menuTexts_;

	//���j���[�ƑΉ�����֐����Z�b�g�ɂ����e�[�u��
	std::map<std::wstring, MenuFunc_t> functionTable_;

	void DrawMenu();



public:
	PauseScene(SceneController& controller);
	/// <summary>
	/// �����ϐ������X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(Input& input)override;
	virtual void Draw()override;
};

