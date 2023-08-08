#pragma once
#include "Vector.h"
#include "CreateTriangle.h"
#include "CreateSprite.h"
#include "CreateSphere.h"
#include "CJEngine.h"
#include "Model.h"

class GameScene{
public:
	void Initialize(CitrusJunosEngine* engine, DirectXCommon* direct);
	void Update();
	void Draw();
	void Finalize();

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;

	CreateTriangle* triangle_[2];
	TriangleData triangleData_[2];
	Transform triangleTransform_[2];

	CreateSprite* sprite_[2];
	SpriteData spriteData_;
	Transform spriteTransform_;
	Transform SpriteuvTransform_;
	
	CreateSphere* sphere_;
	Transform sphereTransform_;
	Vector4 sphereMaterial_;

	Model* model_;
	Transform modelTransform_;
	Vector4 modelMaterial_;

	Transform cameraTransform_;

	DirectionalLight directionalLight_;

	uint32_t uvResourceNum_;
	uint32_t monsterBallResourceNum_;
	uint32_t modelResourceNum_;

	int texture_;

	bool isTriangleDraw1_;
	bool isTriangleDraw2_;
	int isSphereDraw_;
	int isSpriteDraw_;
	int isModelDraw_;
};