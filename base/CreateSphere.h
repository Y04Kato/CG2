#pragma once
#include "DirectXCommon.h"
#include "Vector.h"
#include "MatrixCalculation.h"

class CitrusJunosEngine;

class CreateSphere{
public:
	void Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine);

	void Draw(const Vector4& material, const Transform& transform, uint32_t index, const Transform& cameraTransform, const DirectionalLight& light);
	
	void Finalize();

private:
	void SettingVertex();
	void SettingColor();
	void TransformMatrix();
	void SettingDictionalLight();

private:
	DirectXCommon* dxCommon_;
	CitrusJunosEngine* CJEngine_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ID3D12Resource* vertexResource_;
	VertexData* vertexData_;

	ID3D12Resource* wvpResource_;
	TransformationMatrix* wvpData_;

	ID3D12Resource* materialResource_;
	Material* materialData_;

	const float pi = 3.1415f;
	uint32_t kSubDivision_;
	uint32_t vertexCount_;

	DirectionalLight* directionalLight_;
	ID3D12Resource* directionalLightResource_;

	ID3D12Resource* indexResourceSphere_;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSphere_{};
	uint32_t* indexDataSphere_;
};