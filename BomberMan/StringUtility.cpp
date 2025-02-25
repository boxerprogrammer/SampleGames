#include "StringUtility.h"
#include<DxLib.h>
std::wstring 
StringUtility::GetWStringFromString(const std::string& str)
{
	//�܂��͕ϊ���̃T�C�Y�𑪂��Ă���(�v�͕ϊ��O�ƕϊ���ŃT�C�Y���قȂ�ꍇ�����邽��)
	auto size = MultiByteToWideChar(
		CP_ACP,//���݂̃R�[�h�y�[�W
		0,
		str.c_str(),
		static_cast<int>(str.size()),
		nullptr,
		0
	);
	//���̃T�C�Y�����ɕԂ��ׂ�wstring�����T�C�Y���܂��B
	std::wstring wstr;
	wstr.resize(size);
	//�T�C�Y���m�肵���̂ł�����xMultiByteToWideChar���Ăяo���܂�(���ۂɕϊ�)
	MultiByteToWideChar(
		CP_ACP,//���݂̃R�[�h�y�[�W
		0,
		str.c_str(),
		static_cast<int>(str.size()),
		wstr.data(),//wstr�̃f�[�^�{�̂̐擪�A�h���X
		static_cast<int>(wstr.size())//wstr�̃f�[�^�T�C�Y
	);
	return wstr;
}
