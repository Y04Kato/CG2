#pragma once
#include <stdint.h>
#include "Matrix4x4.h"

struct Vector2 {
	float num[2];
};

struct Vector3 {
	float num[3];
};

struct Vector4 {
	float num[4];
};

struct TriangleData {
	Vector4 position[3];
	Vector4 material;
};

struct SpriteData {
	Vector4 positionLeftTop[2];
	Vector4 positionRightDown[2];
	Vector4 material;
};

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
};

struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};