#pragma once
#include "Vector.h"
#include "CreateTriangle.h"
#include "CreateSprite.h"
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
	Vector4 data1_[2];
	Vector4 data2_[2];
	Vector4 data3_[2];
	Vector4 material_[2];
	Transform transform_;
	Matrix4x4 worldMatrix_;

	Sprite* sprite_[2];
	Vector4 spriteDataLeftTop_[2];
	Vector4 spriteDataRightDown_[2];
	Transform spriteTransform_[1];
	Vector4 spriteMaterial[2];
	
};