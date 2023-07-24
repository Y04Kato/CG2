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

	CreateTriangle* triangle_[3];
	TriangleData triangleData_[3];
	Transform transform_;
	Matrix4x4 worldMatrix_;

	Transform cameraTransform_;
};