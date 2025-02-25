#include "SceneController.h"
#include"TitleScene.h"
#include<cassert>

SceneController::SceneController()
{
	//��ԍŏ��̃V�[�������͊��蓖�ĂĂ����Ă�����
	ChangeScene(std::make_shared<TitleScene>(*this));
}

void
SceneController::Update(Input& input)
{
	//�擪(���͖���)��Update�������s���Ȃ�
	//�Ⴆ�΃Q�[�����̃|�[�Y�̏ꍇ�A�|�[�Y����ɏ���Ă���
	//�Ȃ̂ŁA�|�[�Y�̃^�C���͐i�߂邪�A�Q�[���̃^�C���͐i�߂Ȃ�
	scenes_.back()->Update(input);//��Ԑ擪�����X�V����
}

void SceneController::Draw()
{
	for (auto& scene : scenes_) {
		//�ŏ��ɐς܂ꂽ���̂���`�悷��B����ɂ����
		//�Ō�ɐς܂ꂽ���̂���Ԏ�O�ɕ\�������
		scene->Draw();
	}
}

void SceneController::ChangeScene(std::shared_ptr<Scene> scene)
{
	//���́u�����̑���v�ɁA�������̑��삪�܂܂�Ă���
	//�@�����Ă����V�[�������s���V�[���ɑ������
	//�A���Ƃ��Ǝ��s���V�[���ɓ����Ă����V�[�����͎̂Q�Ɛ悪�O�ɂȂ�(�Q�ƃJ�E���^��0)
	//�B���Ƃ��Ǝ��s���������V�[���͇A�ɂ�莩���ō폜�����
	if (scenes_.empty()) {
		scenes_.push_back(scene);
	}
	else {
		scenes_.back() = scene;
	}
}

void SceneController::ChangeBaseScene(std::shared_ptr<Scene> scene)
{
	assert(!scenes_.empty());
	scenes_.front() = scene;
}

void SceneController::PushScene(std::shared_ptr<Scene> scene)
{
	scenes_.push_back(scene);
}

void SceneController::PopScene()
{
	if (scenes_.size() == 1) {
		return;//���s���ׂ��V�[�����Ȃ��Ȃ邽�ߋ����Ȃ�
	}
	scenes_.pop_back();//��������菜��
}
