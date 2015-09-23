#include "dxconstantbuffer.h"
#include "../wdgraphics.h"

discobreak::DxConstantBuffer::DxConstantBuffer(Wdgraphics* graphics,
	unsigned int registerIndex, ID3D11ShaderReflectionConstantBuffer * reflectionBuffer, 
	D3D11_SHADER_BUFFER_DESC bdesc,
	ConstantBufferType type) {

	graphics_ = graphics;
	registerIndex_ = registerIndex;
	type_ = type;

	name_ = bdesc.Name;
	size_ = bdesc.Size;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = size_;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	HRESULT hr = graphics_->device_->CreateBuffer(&bufferDesc, nullptr, &constantBuffer_);
	if (FAILED(hr))
		MessageBox(NULL, "Failed to CreateConstantBuffer in DxConstantBuffer::DxConstantBuffer(...)", "Error", MB_OK);

	for (unsigned int i = 0; i < bdesc.Variables; ++i) {
		ID3D11ShaderReflectionVariable* var = NULL;
		var = reflectionBuffer->GetVariableByIndex(i);
		D3D11_SHADER_VARIABLE_DESC varDesc;
		var->GetDesc(&varDesc);

		constBufferVariables_[varDesc.Name] = std::pair<unsigned int, unsigned int>(varDesc.Size, varDesc.StartOffset);
	}

}

discobreak::DxConstantBuffer::~DxConstantBuffer() {
	graphics_ = nullptr;
	constantBuffer_->Release();
}

void discobreak::DxConstantBuffer::SetBufferData(void* data) {
	graphics_->deviceContext_->UpdateSubresource(constantBuffer_, 0, nullptr, data, 0, 0);
}

void discobreak::DxConstantBuffer::SetConstantBuffer() {
	if(type_ == ConstantBufferType::Vertexshader)
		graphics_->deviceContext_->VSSetConstantBuffers(registerIndex_, 1, &constantBuffer_);
	else if(type_ == ConstantBufferType::Pixelshader)
		graphics_->deviceContext_->PSSetConstantBuffers(registerIndex_, 1, &constantBuffer_);

}

ID3D11Buffer* discobreak::DxConstantBuffer::GetBuffer() {
	return constantBuffer_;
}

std::string discobreak::DxConstantBuffer::GetName() {
	return name_;
}
