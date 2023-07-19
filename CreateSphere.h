#pragma once
#include "DirectXCommon.h"
#include "Vector.h"
#include "CJEngine.h"

class CreateSphere{
public:
	void Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine);

	void Draw(const Vector4& material, const Matrix4x4& wvpdata);
	void Finalize();
private:
	DirectXCommon* dxCommon_;
	CitrusJunosEngine* CJEngine_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource;
	VertexData* vertexData_;

	ID3D12Resource* wvpResource_;
	Matrix4x4* wvpData_;

	ID3D12Resource* materialResource_;
	Vector4* materialData_;

	const float pi = 3.1415f;
	uint32_t kSubDivision;
	uint32_t vertexCount;

private:
	void CreateVartexData();
	void SetColor();
	void TransformMatrix();
};