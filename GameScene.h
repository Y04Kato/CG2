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
	void Draw2D();
	void Draw3D();
	void Finalize();

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;

	CreateTriangle* triangle_[2];
	TriangleData triangleData_[2];
	Transform transform_;
	Matrix4x4 worldMatrix_;

	CreateSprite* sprite_[2];
	SpriteData spriteData_;
	Transform spriteTransform_;
	
	CreateSphere* sphere_;
	Transform sphereTransform_;
	Vector4 sphereMaterial_;
	Matrix4x4 sphereMatrix_;

	Transform cameraTransform_;
};