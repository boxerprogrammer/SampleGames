#pragma once
#include<map>
#include<string>
#include<vector>

//���Ƃ��������v���g�^�C�v�錾
class KeyconfigScene;

/// <summary>
/// ���͌n���R���g���[������N���X
/// </summary>
class Input
{
	friend KeyconfigScene;//KeyconfigScene�����F�B�F��cprivate�ł�protected�ł��A�N�Z�X�\
private:
	///���Ӌ@����
	enum class PeripheralType {
		keybd,//�L�[�{�[�h
		pad1//�p�b�h�P
	};
	///���͏���`�p
	struct InputState {
		PeripheralType type;//���Ӌ@����
		int id;//���ۂ̓��͂�\���萔
	};

	//Table=�\
	using InputTable_t = std::map<std::string, std::vector<InputState>>;
	InputTable_t inputTable_;//���O�Ǝ��ۂ̓��͂̑Ή��\
	InputTable_t tempInputTable_;//���O�Ǝ��ۂ̓��͂̑Ή��\(�ꎞ�I�ȃR�s�[)
	
	void SetDafault();

	/// <summary>
	/// �L�[�R���t�B�O�p(�s�����ύX���m�肵�܂�)
	/// </summary>
	void CommitEdittedInputTable();

	/// <summary>
	/// �L�[�R���t�B�O�p(�s�����ύX���Ȃ��������Ƃɂ��܂�)
	/// </summary>
	void RollbackEdittedInputTable();

	//Record=�L�^
	//�����ꂽ���ǂ������L�^���Ă�������
	using InputRecord_t = std::map<std::string, bool>;
	InputRecord_t current_;//���݉����Ă邩�ǂ���
	InputRecord_t last_;//���O�ɉ�����Ă����ǂ���

	std::vector<std::string> orderForDisplay_;//�\���p(map�̏����̓R���g���[���ł��Ȃ�����)

	//�L�[�R���t�B�O�p
	int currentRawPadState_;//���̐��p�b�h�X�e�[�g
	int lastRawPadState_;//���O�̐��p�b�h�X�e�[�g
	char currentRawKeybdState_[256];//���̃L�[�{�[�h���
	char lastRawKeybdState_[256];//���O�̃L�[�{�[�h���

	/// <summary>
	/// ���݂̃L�[�{�[�h��Ԃ𒲂ׂčł�
	/// �Ⴂ�L�[�R�[�h��Ԃ��܂�
	/// �ЂƂ�������ĂȂ����-1��Ԃ�
	/// </summary>
	/// <returns></returns>
	int GetKeyboradState()const;

	/// <summary>
	/// ���݂�PAD��Ԃ𒲂ׂčł�
	/// �Ⴂ�L�[���͂�Ԃ��܂�
	/// �ЂƂ�������ĂȂ����-1��Ԃ�
	/// </summary>
	int GetPadState(int padno)const;

	/// <summary>
	/// ���݂̓��̓e�[�u�����Z�[�u���܂�
	/// </summary>
	void SaveInputTable();

	/// <summary>
	/// ���̓e�[�u�������[�h���Ĕ��f���܂�
	/// </summary>
	void LoadInputTable();

public:
	Input();

	/// <summary>
	/// ���͏�Ԃ��X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �������u�Ԃ����o����
	/// </summary>
	/// <param name="key">�C�x���g��</param>
	/// <returns>true:�������u�� false:������ĂȂ�or�������ςȂ�</returns>
	bool IsTrigger(const char* key)const;

	/// <summary>
	/// �����݉�����Ă��邩�ǂ��������o����
	/// </summary>
	/// <param name="key">�C�x���g��</param>
	/// <returns>true:������Ă��� / false:������Ă��Ȃ�</returns>
	bool IsPressed(const char* key) const;
};

