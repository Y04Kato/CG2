#include "gameScene.h"

void GameScene::Initialize(CitrusJunosEngine* engine, DirectXCommon* dxCommon) {
	CJEngine_ = engine;
	dxCommon_ = dxCommon;

	triangleData_[0].position[0] = {-0.5f,-0.5f,0.5f,1.0f};
	triangleData_[0].position[1] = {0.0f,0.0f,0.0f,1.0f};
	triangleData_[0].position[2] = { 0.5f,-0.5f,-0.5f,1.0f };
	triangleData_[0].material = { 1.0f,1.0f,1.0f,1.0f };

	triangleData_[1].position[0] = { -0.5f,-0.5f,0.0f,1.0f };
	triangleData_[1].position[1] = { 0.0f,0.5f,0.0f,1.0f };
	triangleData_[1].position[2] = { 0.5f,-0.5f,0.0f,1.0f };
	triangleData_[1].material = { 1.0f,1.0f,1.0f,1.0f };

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	spriteData_.positionLeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.positionRightDown[0] = {640.0f,360.0f,0.0f,1.0f};
	spriteData_.positionLeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.positionRightDown[1] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	sphereTransform_ = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

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

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
}

void GameScene::Update() {
	transform_.rotate.num[1] += 0.01f;
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	
	sphereTransform_.rotate.num[1] += 0.02f;
	sphereMatrix_ = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);

	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix_, Multiply(viewMatrix, projectionMatrix));
	
	worldMatrix_ = worldViewProjectionMatrix;
	sphereMatrix_ = Multiply(sphereAffine, Multiply(viewMatrix, projectionMatrix));

	ImGui::Begin("test");
	ImGui::DragFloat3("CameraTranslate", cameraTransform_.translate.num, 0.01f);
	ImGui::DragFloat3("SpriteTranslate", spriteTransform_.translate.num, 0.5f);
	ImGui::DragFloat3("SphereTranslate", sphereTransform_.translate.num, 0.5f);
	ImGui::DragFloat3("SphereRotate", sphereTransform_.rotate.num, 0.5f);
	ImGui::DragFloat3("SphereScale", sphereTransform_.scale.num, 0.5f);
	ImGui::End();
}

void GameScene::Draw3D() {
	for (int i = 0; i < 2; i++) {//Triangle描画
		triangle_[i]->Draw(triangleData_[i].position[0], triangleData_[i].position[1], triangleData_[i].position[2], triangleData_[i].material, worldMatrix_);
	}

	sphere_->Draw(sphereMaterial_, sphereMatrix_);
}

void GameScene::Draw2D() {
	for (int i = 0; i < 1; i++) {//Sprite描画
		sprite_[i]->Draw(spriteData_.positionLeftTop[i], spriteData_.positionRightDown[i], spriteTransform_, spriteData_.material);
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