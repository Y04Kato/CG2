#include "gameScene.h"

void GameScene::Initialize(CitrusJunosEngine* engine, DirectXCommon* dxCommon) {
	CJEngine_ = engine;
	dxCommon_ = dxCommon;

	triangleData_[0].position[0] = { -0.5f,-0.5f,0.0f,1.0f };
	triangleData_[0].position[1] = { 0.0f,0.5f,0.0f,1.0f };
	triangleData_[0].position[2] = { 0.5f,-0.5f,0.0f,1.0f };
	triangleData_[0].material = { 1.0f,1.0f,1.0f,1.0f };

	triangleData_[1].position[0] = { -0.5f,-0.5f,0.5f,1.0f };
	triangleData_[1].position[1] = { 0.0f,0.0f,0.0f,1.0f };
	triangleData_[1].position[2] = { 0.5f,-0.5f,-0.5f,1.0f };
	triangleData_[1].material = { 1.0f,1.0f,1.0f,1.0f };

	for (int i = 0; i < 2; i++) {
		transform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	}
	isTriangleDraw1_ = false;
	isTriangleDraw2_ = false;

	spriteData_.positionLeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.positionRightDown[0] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.positionLeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.positionRightDown[1] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	isSpriteDraw_ = false;

	sphereTransform_ = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	isSphereDraw_ = false;

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	texture_ = 0;
	uvResourceNum_ = 0;
	CJEngine_->SettingTexture("resources/uvChecker.png", uvResourceNum_);

	monsterBallResourceNum_ = 1;
	CJEngine_->SettingTexture("resources/monsterBall.png", monsterBallResourceNum_);

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

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
}

void GameScene::Update() {

	directionalLight_.direction = Normalise(directionalLight_.direction);

	ImGui::Begin("Settings");
	if (ImGui::TreeNode("Triangle")) {
		if (ImGui::Button("DrawTriangle1")) {
			if (isTriangleDraw1_ == false) {
				isTriangleDraw1_ = true;
			}
			else {
				isTriangleDraw1_ = false;
			}
		}
		if (ImGui::Button("DrawTriangle2")) {
			if (isTriangleDraw2_ == false) {
				isTriangleDraw2_ = true;
			}
			else {
				isTriangleDraw2_ = false;
			}
		}
		if (isTriangleDraw1_ == true) {
			if (ImGui::TreeNode("Triangle1")) {
				ImGui::DragFloat3("Translate", transform_[0].translate.num, 0.05f);
				ImGui::DragFloat3("Rotate", transform_[0].rotate.num, 0.05f);
				ImGui::DragFloat3("Scale", transform_[0].scale.num, 0.05f);
				ImGui::ColorEdit4("", triangleData_[0].material.num, 0);
				ImGui::TreePop();
			}
		}
		if (isTriangleDraw2_ == true) {
			if (ImGui::TreeNode("Triangle2")) {
				ImGui::DragFloat3("Translate2", transform_[1].translate.num, 0.05f);
				ImGui::DragFloat3("Rotate2", transform_[1].rotate.num, 0.05f);
				ImGui::DragFloat3("Scale2", transform_[1].scale.num, 0.05f);
				ImGui::ColorEdit4("", triangleData_[1].material.num, 0);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sphere")) {
		if (ImGui::Button("DrawSphere")) {
			if (isSphereDraw_ == false) {
				isSphereDraw_ = true;
			}
			else {
				isSphereDraw_ = false;
			}
		}
		ImGui::DragFloat3("Translate", sphereTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", sphereTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", sphereTransform_.scale.num, 0.05f);
		ImGui::ColorEdit4("", sphereMaterial_.num, 0);
		ImGui::SliderInt("ChangeTexture", &texture_, 0, 1);
		ImGui::DragFloat4("LightColor", directionalLight_.color.num, 1.0f);
		ImGui::DragFloat3("lightDirection", directionalLight_.direction.num, 0.1f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sprite")) {
		if (ImGui::Button("DrawSprite")) {
			if (isSpriteDraw_ == false) {
				isSpriteDraw_ = true;
			}
			else {
				isSpriteDraw_ = false;
			}
		}
		ImGui::DragFloat3("Translate", spriteTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", spriteTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", spriteTransform_.scale.num, 0.05f);
		ImGui::ColorEdit4("", spriteData_.material.num, 0);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Camera")) {
		ImGui::DragFloat3("Translate", cameraTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", cameraTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", cameraTransform_.scale.num, 0.05f);
		ImGui::TreePop();
	}
	ImGui::End();

}

void GameScene::Draw() {
#pragma region 3Dオブジェクト描画
	if (isTriangleDraw1_) {//Triangle描画
		triangle_[0]->Draw(triangleData_[0].position[0], triangleData_[0].position[1], triangleData_[0].position[2], triangleData_[0].material, transform_[0], cameraTransform_, uvResourceNum_, directionalLight_);
	}
	if (isTriangleDraw2_) {//Triangle描画
		triangle_[1]->Draw(triangleData_[1].position[0], triangleData_[1].position[1], triangleData_[1].position[2], triangleData_[1].material, transform_[1], cameraTransform_, uvResourceNum_, directionalLight_);
	}

	if (isSphereDraw_) {
	sphere_->Draw(sphereMaterial_, spriteTransform_, texture_, cameraTransform_, directionalLight_);
	}
#pragma endregion

#pragma region 前景スプライト描画
	if (isSpriteDraw_) {
		for (int i = 0; i < 1; i++) {//Sprite描画
			sprite_[i]->Draw(spriteData_.positionLeftTop[i], spriteData_.positionRightDown[i], spriteTransform_, spriteData_.material, uvResourceNum_, directionalLight_);
		}
	}
#pragma endregion
}

void GameScene::Finalize() {
	for (int i = 0; i < 2; i++) {
		triangle_[i]->Finalize();
	}

	for (int i = 0; i < 2; i++) {
		sprite_[i]->Finalize();
	}

	sphere_->Finalize();
}