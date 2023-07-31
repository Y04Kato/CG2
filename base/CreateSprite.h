#pragma once
#include "DirectXCommon.h"
#include "Vector.h"
#include "CJEngine.h"

class CreateSprite{
public:
	void Initialize(DirectXCommon* dxCommon, CitrusJunosEngine* engine);

	void Draw(const Vector4& a, const Vector4& b, const Transform& transform, const Vector4& material, uint32_t index, const DirectionalLight& light);
	void Finalize();

private:
	DirectXCommon* dxCommon_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ID3D12Resource* vertexResourceSprite_;
	VertexData* vertexData_;
	ID3D12Resource* transformationMatrixResource_;
	TransformationMatrix* transformationMatrixdata_;
	ID3D12Resource* materialResource_;
	Material* materialData_;
	CitrusJunosEngine* CJEngine_;
	DirectionalLight* directionalLight_;
	ID3D12Resource* directionalLightResource_;

private:
	void SettingVartex();
	void TransformMatrix();
	void SettingColor();
	void SettingDictionalLight();
};