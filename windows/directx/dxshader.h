//
// Created by hakeee on 2015-09-09.
//

#ifndef DISCOBREAKWINDOWS_DXSHADER_H
#define DISCOBREAKWINDOWS_DXSHADER_H

#include <d3d11.h>
#include <string>
#include <vector>
#include <map>


namespace discobreak {
	class DxConstantBuffer;
	class Wdgraphics;
	class GameObject;
	class DxShader {

	public:
		DxShader(Wdgraphics* graphics);
		~DxShader();

		bool Load(std::string& filename);

		void Draw();

	private:
		Wdgraphics* graphics_;

		ID3D11VertexShader* vertexShader_;
		ID3D11InputLayout* inputLayout_;
		ID3D11PixelShader* pixelShader_;
		ID3D11SamplerState* samplerState_;
		//						   Name    Buffer
		std::vector<std::pair<std::string, DxConstantBuffer*>> constantBuffers_;
		//		Name		  BindPoint
		std::map<std::string, unsigned int> textureRegisters_;
		//		Name		  BindPoint
		std::map<std::string, unsigned int> samplerRegisters_;
		

	};
}

#endif //DISCOBREAKWINDOWS_DXSHADER_H
