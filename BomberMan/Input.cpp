#include "Input.h"
#include<DxLib.h>
#include"StringUtility.h"

namespace {//無名ネームスペースで、他と被らないように
    const char* key_config_signature = "kcfg";
    const char* key_config_filename = "keyconfig.dat";
}

void Input::SetDafault()
{
    //入力と名前の対応表を作ります
    inputTable_["next"] = { {PeripheralType::keybd, KEY_INPUT_RETURN},
                            {PeripheralType::pad1, PAD_INPUT_A} //Aボタン
    };

    inputTable_["ok"] = { {PeripheralType::keybd, KEY_INPUT_RETURN},
                        {PeripheralType::pad1, PAD_INPUT_A} //Aボタン
    };
    inputTable_["pause"] = { {PeripheralType::keybd, KEY_INPUT_P},
                            {PeripheralType::pad1, PAD_INPUT_R} //STARTボタン
    };

    //ゲーム中のボタンテーブル
    inputTable_["action"] = { {PeripheralType::keybd, KEY_INPUT_Z},
                            {PeripheralType::pad1, PAD_INPUT_X} //Yボタン
    };
    inputTable_["jump"] = { {PeripheralType::keybd, KEY_INPUT_X},
                        {PeripheralType::pad1, PAD_INPUT_C} //Xボタン
    };

    //上下左右
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
    tempInputTable_ = inputTable_;//一時テーブルにコピー

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
    //全チェックします(どれか一つでも見つかったらそこで処理を打ち切ります)
    for (int i = 0; i < _countof(currentRawKeybdState_); ++i) {
        if (currentRawKeybdState_[i] && !lastRawKeybdState_[i]) {
            return i;
        }
    }
    return -1;
}

int Input::GetPadState(int padno) const
{
    //指定番号のパッド情報を取得します
    //padstate=00000000000000000010000111001100←2進数でこんな感じの情報になっている
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
    auto err = fopen_s(&fp, key_config_filename, "wb");//バイナリで「書き込み」
    //識別子書き込み(４バイト)
    std::string signature = key_config_signature;//ファイルを識別するための識別子
    fwrite(signature.data(), signature.size(), 1, fp);//識別子の書き込み
    
    //バージョンの書き込み
    const float version = 1.0f;
    fwrite(&version, sizeof(version), 1, fp);//4バイト

    //データサイズの書き込み
    auto size=inputTable_.size();
    fwrite(&size, sizeof(size), 1, fp);//4バイト

    //データ本体の書き込み
    for (const auto& record : inputTable_) {
        //キーの書き込み(イベント名)
        byte nameSize = static_cast<byte>(record.first.length());//文字列数
        fwrite(&nameSize, sizeof(nameSize), 1, fp);//文字列のサイズ
        fwrite(record.first.data(), record.first.size(), 1, fp);//文字列本体
        //実データ値の書き込み(「入力種別＋入力ID」の配列)
        //まずは、実データ配列のサイズを取得します
        byte inputDataSize = static_cast<byte>(record.second.size());
        fwrite(&inputDataSize, sizeof(inputDataSize), 1, fp);
        fwrite(record.second.data(), //書き込む実データの先頭アドレス
                sizeof(record.second[0]), //書き込むデータ１つあたりのサイズ
                record.second.size(), //書き込むデータ数
                fp);
    }

    fclose(fp);

}

void Input::LoadInputTable()
{
    std::string filename = key_config_filename;
    auto wfilename = StringUtility::GetWStringFromString(filename);
    auto handle = FileRead_open(wfilename.c_str());
    if (handle == 0) {//何頭の原因で読み込み失敗したら読み込まない
        return;
    }
    struct Header {
        char signature[4];//シグネチャ
        float version;//バージョン
        size_t dataNum;//データ数
    };
    Header header = {};//ヘッダーの読み込み
    FileRead_read(&header, sizeof(header), handle);
    //データ数の分だけ読み込んでいく
    for (int i = 0; i < header.dataNum; ++i) {
        //データはまず、イベント名を読み込む
        //名前のサイズ
        byte nameSize = 0;
        FileRead_read(&nameSize, sizeof(nameSize), handle);
        //実際に名前文字列を取得
        std::string strEventName;
        strEventName.resize(nameSize);//名前のサイズ分確保
        FileRead_read(strEventName.data(), nameSize * sizeof(char), handle);

        //名前が終わった後は実データなので、まず実データ個数を取得
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
    tempInputTable_ = inputTable_;//一時テーブルにコピー
    orderForDisplay_ = { "ok","next","action","jump"};//キーコンフィグではこの順番で表示してね

}

void Input::Update()
{
    last_ = current_;//直前の情報をコピーしておく
    
    //直前の情報をコピー
    std::copy(std::begin(currentRawKeybdState_), std::end(currentRawKeybdState_), std::begin(lastRawKeybdState_));
    lastRawPadState_ = currentRawPadState_;
    //ハードウェア(周辺機器)の状態を取得
    GetHitKeyStateAll(currentRawKeybdState_);//ハードウェアから現在の入力状態を取得
    currentRawPadState_=GetJoypadInputState(DX_INPUT_PAD1);//パッド１の状態を取得

    //入力チェック(生の入力をゲームのイベントに変換していく)
    for (const auto& inputRow : inputTable_) {
        const auto& key = inputRow.first;//行のキー
        current_[key] = false;//一旦「押してる状態」を初期化する
        for (const auto& hardInput : inputRow.second) {//入力定義vectorのループ
            if (hardInput.type == PeripheralType::keybd) {
                current_[key] = currentRawKeybdState_[hardInput.id];
            }
            else if (hardInput.type == PeripheralType::pad1) {
                current_[key] = hardInput.id & currentRawPadState_;
            }
            //どれか一つでも「押されている」状態ならもう調べない
            //別に機器の情報が欲しいわけではない為、誰かが押されていればもうOK
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
    //前のフレームでは押されてない かつ 今のフレームで押されている
    //→押した瞬間を検出する
    return (current_.at(key) && !last_.at(key));
}

bool Input::IsPressed(const char* key) const
{
    if (!current_.contains(key)) {
        return false;
    }
    return current_.at(key);
}
