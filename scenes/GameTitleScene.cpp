#include"GameTitleScene.h"

void GameTitleScene::Initialize(){
	//Input
	input_ = Input::GetInstance();
}

void GameTitleScene::Update(){
	if (input_->PressKey(DIK_X)) {
		sceneNo = 1;
	}
	input_->Update();
}

void GameTitleScene::Draw(){

}

void GameTitleScene::Finalize() {

}