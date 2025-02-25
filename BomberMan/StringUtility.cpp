#include "StringUtility.h"
#include<DxLib.h>
std::wstring 
StringUtility::GetWStringFromString(const std::string& str)
{
	//まずは変換後のサイズを測っておく(要は変換前と変換後でサイズが異なる場合があるため)
	auto size = MultiByteToWideChar(
		CP_ACP,//現在のコードページ
		0,
		str.c_str(),
		static_cast<int>(str.size()),
		nullptr,
		0
	);
	//そのサイズを元に返すべきwstringをリサイズします。
	std::wstring wstr;
	wstr.resize(size);
	//サイズが確定したのでもう一度MultiByteToWideCharを呼び出します(実際に変換)
	MultiByteToWideChar(
		CP_ACP,//現在のコードページ
		0,
		str.c_str(),
		static_cast<int>(str.size()),
		wstr.data(),//wstrのデータ本体の先頭アドレス
		static_cast<int>(wstr.size())//wstrのデータサイズ
	);
	return wstr;
}
