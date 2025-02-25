#pragma once
#include "Scene.h"
#include<vector>
#include<string>
#include<map>
/// <summary>
/// �L�[�R���t�B�O��ʃV�[��
/// </summary>
class KeyconfigScene : public Scene
{
private:
	int frame_ = 0;
	Input& input_;//Input�ւ̎Q��
	using UpdateFunc_t = void(KeyconfigScene::*)(Input& input);
	using DrawFunc_t = void(KeyconfigScene::*)();
	std::map<int, std::wstring> keyNameTable_;
	std::map<int, std::wstring> buttonNameTable_;

	unsigned int currentSelecterIndex_ = 0;//���ݑI�𒆂̍s�ԍ�

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	//�o�������
	void AppearUpdate(Input& input);
	//�m�[�}�����
	void NormalUpdate(Input& input);
	//���Œ����
	void DisappearUpdate(Input& input);
	//�ҏW�����
	void EdittingUpdate(Input& input);


	//�ʏ�`��
	void NormalDraw();
	//�r���`��
	void ShiftingDraw();
	
	/// <summary>
	/// Input�̏���\������
	/// </summary>
	void DrawInputInfo();

	void DrawStringWithShadow(int x, int y, const wchar_t* str, uint32_t color);

	void Close();
	using MenuFunc_t = void (KeyconfigScene::*)(Input&);
	//�ۑ������ɃL�[�R���t�B�O�����
	//�������_�ŁA�ҏW�������e�͔j������܂�
	void CloseKeyConfig(Input&);
	//�ҏW�������e���m�肵�܂�
	//�L�[�̓��e�͕ς��A�ύX�͎c�葱���܂�
	void CommitAndClose(Input&);
	//���S�Ƀf�t�H���g�ɖ߂�
	void SetDefault(Input&);
	void CancelEdit(Input&);


	std::map<std::wstring, MenuFunc_t> menuFuncTable_;//���j���[�ɑΉ������֐��e�[�u��
	std::vector<std::wstring> menuString_;//���j���[�̕�����

public:
	KeyconfigScene(SceneController& controller,Input& input);
	/// <summary>
	/// �����ϐ������X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(Input& input)override;
	virtual void Draw()override;
};

