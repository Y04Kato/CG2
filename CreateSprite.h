#pragma once
#include "DirectXCommon.h"
#include "Vector.h"
#include "MyEngine.h"

class Sprite{
public:
	void Initialize(DirectXCommon* dxCommon, MyEngine* engine);

	void Draw(const Vector4& a, const Vector4& b, const Transform& transform, const Vector4& material);
	void Finalize();

private:
	DirectXCommon* dxCommon_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ID3D12Resource* vertexResourceSprite_;
	VertexData* vertexData_;
	ID3D12Resource* transformationMatrixResource_;
	Matrix4x4* transformationMatrixdata_;
	ID3D12Resource* materialResource_;
	Vector4* materialData_;
	MyEngine* myEngine_;

private:
	void CreateVartexData();
	void CreateTransform();
	void SetColor();
};