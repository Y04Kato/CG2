#pragma once
#include "Vector.h"
#include "CreateTriangle.h"
#include "CreateSprite.h"
#include "CreateSphere.h"
#include "CJEngine.h"

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
	Transform transform_[2];
	Matrix4x4 worldMatrix_[2];

	CreateSprite* sprite_[2];
	SpriteData spriteData_;
	Transform spriteTransform_;
	Transform SpriteuvTransform_;
	
	CreateSphere* sphere_;
	Transform sphereTransform_;
	Vector4 sphereMaterial_;
	Matrix4x4 sphereMatrix_;

	Transform cameraTransform_;

	uint32_t uvResourceNum_;
	uint32_t monsterBallResourceNum_;

	DirectionalLight directionalLight_;

	int texture_;

	bool isTriangleDraw1_;
	bool isTriangleDraw2_;
	int isSphereDraw_;
	int isSpriteDraw_;
};