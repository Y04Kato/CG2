#pragma once

struct Vector2 {
	float num[2];
};

struct Vector3 {
	float num[3];
};

struct Vector4 {
	float num[4];
};

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
};