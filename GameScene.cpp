#include "gameScene.h"

void GameScene::Initialize(MyEngine* engine, DirectXCommon* direct) {
	myEngine_ = engine;
	dxCommon_ = direct;

	data1_[0] = { -0.5f,-0.5f,0.0f,1.0f };
	data2_[0] = { 0.0f,0.5f,0.0f,1.0f };
	data3_[0] = { 0.5f,-0.5f,0.0f,1.0f };
	material_[0] = { 1.0f,0.0f,0.0f,1.0f };

	data1_[1] = { -0.5f,-0.5f,0.5f,1.0f };
	data2_[1] = { 0.0f,0.0f,0.0f,1.0f };
	data3_[1] = { 0.5f,-0.5f,-0.5f,1.0f };
	material_[1] = { 0.0f,1.0f,0.0f,1.0f };

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	spriteDataLeftTop_[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteDataRightDown_[0] = { 320.0f,180.0f,0.0f,1.0f };
	spriteDataLeftTop_[1] = { 960.0f,540.0f,0.0f,1.0f };
	spriteDataRightDown_[1] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteMaterial[0] = { 1.0f,1.0f,1.0f,1.0f };

	spriteTransform_[0] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	myEngine_->SettingTexture("resources/uvChecker.png");

	for (int i = 0; i < 2; i++) {
		triangle_[i] = new CreateTriangle();
		triangle_[i]->Initialize(dxCommon_, myEngine_);
	}

	for (int i = 0; i < 2; i++) {
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon_, myEngine_);
	}

}

void GameScene::Update() {
	transform_.rotate.num[1] += 0.01f;
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	ImGui::Begin("Sprite");
	ImGui::SliderFloat3("TransformSprite", spriteTransform_[0].translate.num, 0.0f, 600.0f);
	ImGui::End();
}

void GameScene::Draw() {
	for (int i = 0; i < 2; i++) {//Triangle描画
		triangle_[i]->Draw(data1_[i], data2_[i], data3_[i], material_[i], worldMatrix_);
	}

	for (int i = 0; i < 1; i++) {//Sprite描画
		sprite_[i]->Draw(spriteDataLeftTop_[i], spriteDataRightDown_[i], spriteTransform_[i], spriteMaterial[0]);
	}
}
void GameScene::Finalize() {
	for (int i = 0; i < 2; i++) {
		triangle_[i]->Finalize();
	}

	for (int i = 0; i < 2; i++) {
		sprite_[i]->Finalize();
	}
}