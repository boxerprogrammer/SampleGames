#include<DxLib.h>
#include"Application.h"


int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int) {
	
	Application& app = Application::GetInstance();
	//�A�v���P�[�V�����̏�����
	if (!app.Init()) {
		return -1;
	}

	//���C�����[�v
	app.Run();

	//�㏈��
	app.Terminate();
	return 0;
}