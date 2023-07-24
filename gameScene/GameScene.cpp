#include "gameScene.h"

void GameScene::Initialize(CitrusJunosEngine* engine, DirectXCommon* dxCommon) {
	CJEngine_ = engine;
	dxCommon_ = dxCommon;

	triangleData_[0].position[0] = { -0.2f,-0.2f,0.0f,1.0f };
	triangleData_[0].position[1] = { 0.0f,0.2f,0.0f,1.0f };
	triangleData_[0].position[2] = { 0.2f,-0.2f,0.0f,1.0f };
	triangleData_[0].material = { 1.0f,0.0f,0.0f,1.0f };

	triangleData_[1].position[0] = { -0.8f,-0.8f,0.0f,1.0f };
	triangleData_[1].position[1] = { -0.6f,-0.4f,0.0f,1.0f };
	triangleData_[1].position[2] = { -0.4f,-0.8f,0.0f,1.0f };
	triangleData_[1].material = { 0.0f,1.0f,0.0f,1.0f };

	triangleData_[2].position[0] = { 0.4f,-0.8f,0.0f,1.0f };
	triangleData_[2].position[1] = { 0.6f,-0.4f,0.0f,1.0f };
	triangleData_[2].position[2] = { 0.8f,-0.8f,0.0f,1.0f };
	triangleData_[2].material = { 0.0f,0.0f,1.0f,1.0f };

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	for (int i = 0; i < 3; i++) {
		triangle_[i] = new CreateTriangle();
		triangle_[i]->Initialize(dxCommon_, CJEngine_);
	}

	CJEngine_->SettingTexture("resources/uvChecker.png");

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
}

void GameScene::Update() {
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix_, Multiply(viewMatrix, projectionMatrix));

	worldMatrix_ = worldViewProjectionMatrix;
}

void GameScene::Draw() {
#pragma region 3Dオブジェクト描画
	for (int i = 0; i < 3; i++) {//Triangle描画
		triangle_[i]->Draw(triangleData_[i].position[0], triangleData_[i].position[1], triangleData_[i].position[2], triangleData_[i].material, worldMatrix_);
	}
#pragma endregion

#pragma region 前景スプライト描画

#pragma endregion
}

void GameScene::Finalize() {
	for (int i = 0; i < 3; i++) {
		triangle_[i]->Finalize();
	}
}