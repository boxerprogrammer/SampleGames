#include "Input.h"
#include<DxLib.h>
#include"StringUtility.h"

namespace {//�����l�[���X�y�[�X�ŁA���Ɣ��Ȃ��悤��
    const char* key_config_signature = "kcfg";
    const char* key_config_filename = "keyconfig.dat";
}

void Input::SetDafault()
{
    //���͂Ɩ��O�̑Ή��\�����܂�
    inputTable_["next"] = { {PeripheralType::keybd, KEY_INPUT_RETURN},
                            {PeripheralType::pad1, PAD_INPUT_A} //A�{�^��
    };

    inputTable_["ok"] = { {PeripheralType::keybd, KEY_INPUT_RETURN},
                        {PeripheralType::pad1, PAD_INPUT_A} //A�{�^��
    };
    inputTable_["pause"] = { {PeripheralType::keybd, KEY_INPUT_P},
                            {PeripheralType::pad1, PAD_INPUT_R} //START�{�^��
    };

    //�Q�[�����̃{�^���e�[�u��
    inputTable_["action"] = { {PeripheralType::keybd, KEY_INPUT_Z},
                            {PeripheralType::pad1, PAD_INPUT_X} //Y�{�^��
    };
    inputTable_["jump"] = { {PeripheralType::keybd, KEY_INPUT_X},
                        {PeripheralType::pad1, PAD_INPUT_C} //X�{�^��
    };

    //�㉺���E
    inputTable_["up"] = { {PeripheralType::keybd, KEY_INPUT_UP},
                            {PeripheralType::pad1, PAD_INPUT_UP}
    };
    inputTable_["down"] = { {PeripheralType::keybd, KEY_INPUT_DOWN},
                            {PeripheralType::pad1, PAD_INPUT_DOWN} 
    };
    inputTable_["left"] = { {PeripheralType::keybd, KEY_INPUT_LEFT},
                            {PeripheralType::pad1, PAD_INPUT_LEFT} 
    };
    inputTable_["right"] = { {PeripheralType::keybd, KEY_INPUT_RIGHT},
                            {PeripheralType::pad1, PAD_INPUT_RIGHT} 
    };
    tempInputTable_ = inputTable_;//�ꎞ�e�[�u���ɃR�s�[

}

void Input::CommitEdittedInputTable()
{
    inputTable_ = tempInputTable_;
    SaveInputTable();
}

void Input::RollbackEdittedInputTable()
{
    tempInputTable_ = inputTable_;
}

int Input::GetKeyboradState() const
{
    //�S�`�F�b�N���܂�(�ǂꂩ��ł����������炻���ŏ�����ł��؂�܂�)
    for (int i = 0; i < _countof(currentRawKeybdState_); ++i) {
        if (currentRawKeybdState_[i] && !lastRawKeybdState_[i]) {
            return i;
        }
    }
    return -1;
}

int Input::GetPadState(int padno) const
{
    //�w��ԍ��̃p�b�h�����擾���܂�
    //padstate=00000000000000000010000111001100��2�i���ł���Ȋ����̏��ɂȂ��Ă���
    uint32_t bit = 0b00000000000000000000000000000001;
    for (int i = 0; i < 32; ++i) {
        auto value = (bit << i);
        if ((value & currentRawPadState_) && !(value&lastRawPadState_) )
        {
            return value;
        }
    }
    return -1;
}


void Input::SaveInputTable()
{
    FILE* fp = nullptr;
    auto err = fopen_s(&fp, key_config_filename, "wb");//�o�C�i���Łu�������݁v
    //���ʎq��������(�S�o�C�g)
    std::string signature = key_config_signature;//�t�@�C�������ʂ��邽�߂̎��ʎq
    fwrite(signature.data(), signature.size(), 1, fp);//���ʎq�̏�������
    
    //�o�[�W�����̏�������
    const float version = 1.0f;
    fwrite(&version, sizeof(version), 1, fp);//4�o�C�g

    //�f�[�^�T�C�Y�̏�������
    auto size=inputTable_.size();
    fwrite(&size, sizeof(size), 1, fp);//4�o�C�g

    //�f�[�^�{�̂̏�������
    for (const auto& record : inputTable_) {
        //�L�[�̏�������(�C�x���g��)
        byte nameSize = static_cast<byte>(record.first.length());//������
        fwrite(&nameSize, sizeof(nameSize), 1, fp);//������̃T�C�Y
        fwrite(record.first.data(), record.first.size(), 1, fp);//������{��
        //���f�[�^�l�̏�������(�u���͎�ʁ{����ID�v�̔z��)
        //�܂��́A���f�[�^�z��̃T�C�Y���擾���܂�
        byte inputDataSize = static_cast<byte>(record.second.size());
        fwrite(&inputDataSize, sizeof(inputDataSize), 1, fp);
        fwrite(record.second.data(), //�������ގ��f�[�^�̐擪�A�h���X
                sizeof(record.second[0]), //�������ރf�[�^�P������̃T�C�Y
                record.second.size(), //�������ރf�[�^��
                fp);
    }

    fclose(fp);

}

void Input::LoadInputTable()
{
    std::string filename = key_config_filename;
    auto wfilename = StringUtility::GetWStringFromString(filename);
    auto handle = FileRead_open(wfilename.c_str());
    if (handle == 0) {//�����̌����œǂݍ��ݎ��s������ǂݍ��܂Ȃ�
        return;
    }
    struct Header {
        char signature[4];//�V�O�l�`��
        float version;//�o�[�W����
        size_t dataNum;//�f�[�^��
    };
    Header header = {};//�w�b�_�[�̓ǂݍ���
    FileRead_read(&header, sizeof(header), handle);
    //�f�[�^���̕������ǂݍ���ł���
    for (int i = 0; i < header.dataNum; ++i) {
        //�f�[�^�͂܂��A�C�x���g����ǂݍ���
        //���O�̃T�C�Y
        byte nameSize = 0;
        FileRead_read(&nameSize, sizeof(nameSize), handle);
        //���ۂɖ��O��������擾
        std::string strEventName;
        strEventName.resize(nameSize);//���O�̃T�C�Y���m��
        FileRead_read(strEventName.data(), nameSize * sizeof(char), handle);

        //���O���I�������͎��f�[�^�Ȃ̂ŁA�܂����f�[�^�����擾
        byte inputDataNum = 0;
        FileRead_read(&inputDataNum, sizeof(inputDataNum), handle);
        std::vector<InputState> inputStates;
        inputStates.resize(inputDataNum);
        FileRead_read(inputStates.data(), sizeof(inputStates[0])* inputDataNum, handle);
        inputTable_[strEventName] = inputStates;
    }

    FileRead_close(handle);
}

Input::Input()
{

    SetDafault();
    LoadInputTable();
    tempInputTable_ = inputTable_;//�ꎞ�e�[�u���ɃR�s�[
    orderForDisplay_ = { "ok","next","action","jump"};//�L�[�R���t�B�O�ł͂��̏��Ԃŕ\�����Ă�

}

void Input::Update()
{
    last_ = current_;//���O�̏����R�s�[���Ă���
    
    //���O�̏����R�s�[
    std::copy(std::begin(currentRawKeybdState_), std::end(currentRawKeybdState_), std::begin(lastRawKeybdState_));
    lastRawPadState_ = currentRawPadState_;
    //�n�[�h�E�F�A(���Ӌ@��)�̏�Ԃ��擾
    GetHitKeyStateAll(currentRawKeybdState_);//�n�[�h�E�F�A���猻�݂̓��͏�Ԃ��擾
    currentRawPadState_=GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h�P�̏�Ԃ��擾

    //���̓`�F�b�N(���̓��͂��Q�[���̃C�x���g�ɕϊ����Ă���)
    for (const auto& inputRow : inputTable_) {
        const auto& key = inputRow.first;//�s�̃L�[
        current_[key] = false;//��U�u�����Ă��ԁv������������
        for (const auto& hardInput : inputRow.second) {//���͒�`vector�̃��[�v
            if (hardInput.type == PeripheralType::keybd) {
                current_[key] = currentRawKeybdState_[hardInput.id];
            }
            else if (hardInput.type == PeripheralType::pad1) {
                current_[key] = hardInput.id & currentRawPadState_;
            }
            //�ǂꂩ��ł��u������Ă���v��ԂȂ�������ׂȂ�
            //�ʂɋ@��̏�񂪗~�����킯�ł͂Ȃ��ׁA�N����������Ă���΂���OK
            if (current_[key]) {
                break;
            }
        }
    }
}

bool Input::IsTrigger(const char* key)const
{
    if (!current_.contains(key)) {
        return false;
    }
    //�O�̃t���[���ł͉�����ĂȂ� ���� ���̃t���[���ŉ�����Ă���
    //���������u�Ԃ����o����
    return (current_.at(key) && !last_.at(key));
}

bool Input::IsPressed(const char* key) const
{
    if (!current_.contains(key)) {
        return false;
    }
    return current_.at(key);
}
