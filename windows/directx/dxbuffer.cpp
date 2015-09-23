//
// Created by hakeee on 2015-09-09.
//

#include "dxbuffer.h"
#include "../wdgraphics.h"


discobreak::DxBuffer::DxBuffer(Wdgraphics* graphics) {
	graphics_ = graphics;
}

discobreak::DxBuffer::~DxBuffer() {
	if (vertexBuffer_)
		vertexBuffer_->Release();
	if (indexBuffer_)
		indexBuffer_->Release();
	graphics_ = nullptr;
}



void discobreak::DxBuffer::Use(unsigned int stride) {
	UINT offset = 0;
	graphics_->deviceContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	graphics_->deviceContext_->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
}

void discobreak::DxBuffer::SetVertexData(unsigned int count, float * array) {
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(float)*count;
	vertexDesc.CPUAccessFlags = 0;
	vertexDesc.MiscFlags = 0;
	vertexDesc.StructureByteStride = 0;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = array;


	HRESULT hr = graphics_->device_->CreateBuffer(&vertexDesc, &subData, &vertexBuffer_);
	if (hr != S_OK) {
		MessageBox(NULL, "Failed to create vertex buffer, DxBuffer::SetVertexData()", "Error", MB_OK);
	}
}

void discobreak::DxBuffer::SetIndices(unsigned int count, unsigned int * array) {
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = sizeof(unsigned int)*count;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = array;

	HRESULT hr = graphics_->device_->CreateBuffer(&desc, &subData, &indexBuffer_);
	if (hr != S_OK) {
		MessageBox(NULL, "Failed to create index buffer, DxBuffer::SetIndices()", "Error", MB_OK);
	}

}
