#pragma once

struct Vector2 {
	float x;
	float y;
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Vector4 {
	float vector[4];
};

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
};