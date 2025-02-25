#include "SceneController.h"
#include"TitleScene.h"
#include<cassert>

SceneController::SceneController()
{
	//一番最初のシーンだけは割り当てておいてあげる
	ChangeScene(std::make_shared<TitleScene>(*this));
}

void
SceneController::Update(Input& input)
{
	//先頭(実は末尾)のUpdateしか実行しない
	//例えばゲーム中のポーズの場合、ポーズが上に乗ってる状態
	//なので、ポーズのタイムは進めるが、ゲームのタイムは進めない
	scenes_.back()->Update(input);//一番先頭だけ更新する
}

void SceneController::Draw()
{
	for (auto& scene : scenes_) {
		//最初に積まれたものから描画する。これによって
		//最後に積まれたものが一番手前に表示される
		scene->Draw();
	}
}

void SceneController::ChangeScene(std::shared_ptr<Scene> scene)
{
	//この「ただの代入」に、いくつかの操作が含まれている
	//①入ってきたシーンを実行中シーンに代入する
	//②もともと実行中シーンに入っていたシーン実体は参照先が０になる(参照カウンタが0)
	//③もともと実行中だったシーンは②により自動で削除される
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
		return;//実行すべきシーンがなくなるため許可しない
	}
	scenes_.pop_back();//末尾を取り除く
}
