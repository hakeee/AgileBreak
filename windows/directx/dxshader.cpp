//
// Created by hakeee on 2015-09-09.
//

#include "dxshader.h"
#include <d3dcompiler.h>
#include <comdef.h>
#include "../wdgraphics.h"
#include "dxconstantbuffer.h"
#include "../../generic/texture.h"


discobreak::DxShader::DxShader(Wdgraphics* graphics) {
	graphics_ = graphics;
	samplerState_ = nullptr;
}

discobreak::DxShader::~DxShader() {
	if (inputLayout_)
		inputLayout_->Release();
	if (vertexShader_)
		vertexShader_->Release();
	if (pixelShader_)
		pixelShader_->Release();
	if (samplerState_)
		samplerState_->Release();

	for (auto p : constantBuffers_) {
		delete p.second;
	}

	graphics_ = nullptr;
}
//loads vertex and pixelshader from precompiled shader file. 
//Gets information about constant buffers and texture resources bound in the different stages
bool discobreak::DxShader::Load(std::string& filename) {

	ID3DBlob* source;
	std::string s = filename + "v.cso";
	std::wstring stemp = std::wstring(s.begin(), s.end());
	if (HRESULT hr = D3DReadFileToBlob(stemp.c_str(), &source) != S_OK) {
		MessageBox(0, "Failed to read precompiled shader file, Vertexshader", "DxShader::Load Error", MB_OK);
		return false;
	}

	if (graphics_->device_->CreateVertexShader(source->GetBufferPointer(), source->GetBufferSize(), NULL, &vertexShader_) != S_OK) {
		MessageBox(0, "Failed to create vertexshader", "DxShader::Load Error", MB_OK);
		return false;
	}

	ID3D11ShaderReflection* reflection = nullptr;

	D3DReflect(source->GetBufferPointer(), source->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);

	D3D11_SHADER_DESC desc;
	reflection->GetDesc(&desc);

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
	for (UINT32 i = 0; i < desc.InputParameters; i++) {
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		reflection->GetInputParameterDesc(i, &paramDesc);
		D3D11_INPUT_ELEMENT_DESC inputElementDesc;
		inputElementDesc.SemanticName = paramDesc.SemanticName;
		inputElementDesc.SemanticIndex = paramDesc.SemanticIndex;
		inputElementDesc.InputSlot = 0;
		inputElementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;


		if (paramDesc.Mask == 1) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		inputDesc.push_back(inputElementDesc);
	}
	HRESULT hr = graphics_->device_->CreateInputLayout(&inputDesc[0], inputDesc.size(), source->GetBufferPointer(), source->GetBufferSize(), &inputLayout_);
	if (hr != S_OK)	{
		MessageBox(0, "Failed to create input layout", "DxShader::Load Error", MB_OK);
		source->Release();
		reflection->Release();
		return false;
	}
	//G� igenom alla constant buffers i shadern
	for (unsigned int i = 0; i < desc.ConstantBuffers; ++i)	{
		unsigned int registerIndex = 0;
		ID3D11ShaderReflectionConstantBuffer* buffer = NULL;
		buffer = reflection->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC bdesc;
		buffer->GetDesc(&bdesc);		
		
		//G� igenom alla bundna resurser i shadern f�r att hitta constant buffern,
		//N�r den hittas ta ut bindPoint, dv register indexet f�r constant buffern
		for (unsigned int k = 0; k < desc.BoundResources; ++k) {
			D3D11_SHADER_INPUT_BIND_DESC ibdesc;
			reflection->GetResourceBindingDesc(k, &ibdesc);

			if (!strcmp(ibdesc.Name, bdesc.Name))
				registerIndex = ibdesc.BindPoint;
		}

		DxConstantBuffer* constBuffer = new DxConstantBuffer(graphics_, registerIndex, buffer, bdesc, discobreak::ConstantBufferType::Vertexshader);
		constantBuffers_.push_back(std::pair<std::string, DxConstantBuffer*>(bdesc.Name, constBuffer));
	}
	source->Release();
	reflection->Release();
	 s = filename + "p.cso";
	stemp = std::wstring(s.begin(), s.end());
	if (D3DReadFileToBlob(stemp.c_str(), &source) != S_OK)	{
		MessageBox(0, "Failed to read precompiled shader file, Pixelshader", "DxShader::Load Error", MB_OK);
		return false;
	}
	hr = graphics_->device_->CreatePixelShader(source->GetBufferPointer(), source->GetBufferSize(), nullptr, &pixelShader_);
	if (hr != S_OK)	{
		_com_error error(hr);
		MessageBox(0, error.ErrorMessage(), "DxShader::Load Create Pixel Shader", MB_OK);
		source->Release();
		return false;
	}
	//Skapa reflection och leta upp constant buffers och shader resoruces s�som textures
	D3DReflect(source->GetBufferPointer(), source->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);

	reflection->GetDesc(&desc);

	//G� igenom alla constant buffers i shadern
	for (unsigned int i = 0; i < desc.ConstantBuffers; ++i) {
		unsigned int registerIndex = 0;
		ID3D11ShaderReflectionConstantBuffer* buffer = NULL;
		buffer = reflection->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC bdesc;
		buffer->GetDesc(&bdesc);

		//G� igenom alla bundna resurser i shadern f�r att hitta constant buffern,
		//N�r den hittas ta ut bindPoint, dv register indexet f�r constant buffern
		for (unsigned int k = 0; k < desc.BoundResources; ++k) {
			D3D11_SHADER_INPUT_BIND_DESC ibdesc;
			reflection->GetResourceBindingDesc(k, &ibdesc);

			if (!strcmp(ibdesc.Name, bdesc.Name))
				registerIndex = ibdesc.BindPoint;
		}

		DxConstantBuffer* constBuffer = new DxConstantBuffer(graphics_, registerIndex, buffer, bdesc, discobreak::ConstantBufferType::Pixelshader);
		constantBuffers_.push_back(std::pair<std::string, DxConstantBuffer*>(bdesc.Name, constBuffer));
	}
	//Titta resurser i pixelshadern och ta ut bind point och namn för alla texturer
	for (unsigned int i = 0; i < desc.BoundResources; ++i) {
		D3D11_SHADER_INPUT_BIND_DESC ibdesc;
		reflection->GetResourceBindingDesc(i, &ibdesc);
		//Texturer
		if (ibdesc.Type == D3D_SIT_TEXTURE) {
			textureRegisters_[ibdesc.Name] = ibdesc.BindPoint;
		}
		if (ibdesc.Type == D3D_SIT_SAMPLER) {
			samplerRegisters_[ibdesc.Name] = ibdesc.BindPoint;
			if (samplerState_ == nullptr) {
				D3D11_SAMPLER_DESC sDesc;
				sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				sDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				sDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				sDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
				sDesc.MinLOD = -FLT_MAX;
				sDesc.MaxLOD = FLT_MAX;
				sDesc.MipLODBias = 0.0f;
				sDesc.MaxAnisotropy = 1;
				sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

				graphics_->device_->CreateSamplerState(&sDesc, &samplerState_);
			}
		}
	}

	source->Release();
	reflection->Release();

	return true;
}

void discobreak::DxShader::Draw() {

	graphics_->deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	graphics_->deviceContext_->IASetInputLayout(inputLayout_);

	graphics_->deviceContext_->VSSetShader(vertexShader_, NULL, 0); 
	graphics_->deviceContext_->HSSetShader(NULL, NULL, 0);
	graphics_->deviceContext_->DSSetShader(NULL, NULL, 0);
	graphics_->deviceContext_->GSSetShader(NULL, NULL, 0);
	graphics_->deviceContext_->PSSetShader(pixelShader_, NULL, 0);
	//Should take buffer instead
	/*
	object.GetBuffer()->Use();

	auto uniforms = object.GetUniforms();

	for (auto it = constantBuffers_.begin(); it != constantBuffers_.end(); ++it) {
		auto uniform = uniforms.find(it->first);
		if (uniform != uniforms.end())	{
			it->second->SetBufferData(uniform->second);
			it->second->SetConstantBuffer();
		}
		else
			printf("Object doesn't have uniform %s", it->first.c_str());
	}
	auto textures = object.GetTextures();
	for (auto it : textureRegisters_) {
		auto texture = textures.find(it.first);
		if (texture != textures.end()) {
			texture->second->SetTexture(it.second);
		}
	}
	for (auto it : samplerRegisters_) {
		graphics_->deviceContext_->PSSetSamplers(it.second, 1, &samplerState_);
	}
	graphics_->deviceContext_->DrawIndexed(object.GetBuffer()->GetIndicesCount(), 0, 0);	
	*/
}