#pragma once
#include"DirectXCommon.h"
#include"Vector.h"
#include "MatrixCalculation.h"

class CitrusJunosEngine;

class CreateTriangle {
public:
	void Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine);

	void Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& material, const Matrix4x4& wvpdata);

	void Finalize();

private:
	void SettingVertex();

	void SettingColor();

	void TransformMatrix();

private:
	CitrusJunosEngine* CJEngine_;

	DirectXCommon* dxCommon_;

	VertexData* vertexData_;

	Vector4* materialData_;

	ID3D12Resource* vertexResource_;

	ID3D12Resource* materialResource_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//WVP用のリソース
	ID3D12Resource* wvpResource_;

	Matrix4x4* wvpData_;

};
