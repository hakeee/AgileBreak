#include "dxtexture.h"
#include "ddstextureloader.h"
#include "../wdgraphics.h"

discobreak::DxTexture::DxTexture(Wdgraphics * graphics) {
	graphics_ = graphics;
}

discobreak::DxTexture::~DxTexture() {
	if (texture_)
		texture_->Release();
	graphics_ = nullptr;
}


bool discobreak::DxTexture::Load(const std::string& filename) {
	
	ID3D11Resource* resource;
	HRESULT test = DirectX::CreateDDSTextureFromFile(graphics_->device_, std::wstring(filename.begin(), filename.end()).c_str(), &resource, &texture_);
	if (FAILED(DirectX::CreateDDSTextureFromFile(graphics_->device_, std::wstring(filename.begin(), filename.end()).c_str(), &resource, &texture_))) {
		MessageBox(NULL, "DxTexture::Load()\nFailed to create texture from file", "Error", MB_OK);
		return false;
	}
	D3D11_TEXTURE2D_DESC desc;
	reinterpret_cast<ID3D11Texture2D*>(resource)->GetDesc(&desc);

	width_ = desc.Width;
	height_ = desc.Height;

	if (resource)
		resource->Release();
	return true;
}

ID3D11ShaderResourceView * discobreak::DxTexture::GetTextureResource() {
	return texture_;
}

void discobreak::DxTexture::SetTexture(unsigned int bindPoint) {
	graphics_->deviceContext_->PSSetShaderResources(bindPoint, 1, &texture_);
}
