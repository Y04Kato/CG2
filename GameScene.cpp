#include "gameScene.h"

void GameScene::Initialize(CitrusJunosEngine* engine, DirectXCommon* dxCommon) {
	CJEngine_ = engine;
	dxCommon_ = dxCommon;

	data1_[0] = { -0.5f,-0.5f,0.5f,1.0f };
	data2_[0] = { 0.0f,0.0f,0.0f,1.0f };
	data3_[0] = { 0.5f,-0.5f,-0.5f,1.0f };
	material_[0] = { 1.0f,0.0f,0.0f,1.0f };

	data1_[1] = { -0.5f,-0.5f,0.0f,1.0f };
	data2_[1] = { 0.0f,0.5f,0.0f,1.0f };
	data3_[1] = { 0.5f,-0.5f,0.0f,1.0f };
	material_[1] = { 0.0f,1.0f,0.0f,1.0f };

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	spriteDataLeftTop_[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteDataRightDown_[0] = { 320.0f,180.0f,0.0f,1.0f };
	spriteDataLeftTop_[1] = { 960.0f,540.0f,0.0f,1.0f };
	spriteDataRightDown_[1] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteMaterial_[0] = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_[0] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	sphereTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sphereMaterial_ = { 0.0f,0.0f,1.0f,1.0f };

	for (int i = 0; i < 2; i++) {
		triangle_[i] = new CreateTriangle();
		triangle_[i]->Initialize(dxCommon_, CJEngine_);
	}

	for (int i = 0; i < 2; i++) {
		sprite_[i] = new CreateSprite();
		sprite_[i]->Initialize(dxCommon_, CJEngine_);
	}

	sphere_ = new CreateSphere();
	sphere_->Initialize(dxCommon_, CJEngine_);

	CJEngine_->SettingTexture("resources/uvChecker.png");
}

void GameScene::Update() {
	transform_.rotate.num[1] += 0.01f;
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	
	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	sphereTransform_.rotate.num[1] += 0.05f;

	ImGui::Begin("test");
	ImGui::DragFloat3("TransformSprite", spriteTransform_[0].translate.num, 0.5f);
	ImGui::DragFloat3("spheretranslate", sphereTransform_.translate.num, 0.5f);
	ImGui::DragFloat3("sphererotate", sphereTransform_.rotate.num, 0.5f);
	ImGui::DragFloat3("spherescale", sphereTransform_.scale.num, 0.5f);
	ImGui::End();
}

void GameScene::Draw3D() {
	for (int i = 0; i < 2; i++) {//Triangle描画
		//triangle_[i]->Draw(data1_[i], data2_[i], data3_[i], material_[i], worldMatrix_);
	}

	sphere_->Draw(sphereMaterial_, sphereMatrix_);
}

void GameScene::Draw2D() {
	for (int i = 0; i < 1; i++) {//Sprite描画
		sprite_[i]->Draw(spriteDataLeftTop_[i], spriteDataRightDown_[i], spriteTransform_[i], spriteMaterial_[0]);
	}
}
void GameScene::Finalize() {
	sphere_->Finalize();

	for (int i = 0; i < 2; i++) {
		triangle_[i]->Finalize();
	}

	for (int i = 0; i < 2; i++) {
		sprite_[i]->Finalize();
	}
}