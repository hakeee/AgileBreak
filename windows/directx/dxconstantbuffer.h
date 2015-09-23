#ifndef DISCOBREAKWINDOWS_DXCONSTANTBUFFER_H_
#define DISCOBREAKWINDOWS_DXCONSTANTBUFFER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <map>
#include <string>

namespace discobreak {
	enum ConstantBufferType {
		Vertexshader,
		Pixelshader
	};
	class Wdgraphics;
	class DxConstantBuffer	{
		

	public:
		DxConstantBuffer(Wdgraphics* graphics, 
			unsigned int registerIndex, 
			ID3D11ShaderReflectionConstantBuffer* reflectionBuffer, 
			D3D11_SHADER_BUFFER_DESC bdesc, 
			ConstantBufferType type);

		~DxConstantBuffer();


		void SetBufferData(void* data);
		//Kör en SetConstantBuffer beroende på typ
		void SetConstantBuffer();
		//Returns a pointer to the constant buffer object
		ID3D11Buffer* GetBuffer();
		std::string GetName();

	private:
		ID3D11Buffer* constantBuffer_;
		unsigned int registerIndex_;

		unsigned int size_;
		std::string name_;

		Wdgraphics* graphics_;

		ConstantBufferType type_;

		//       Variable Name,         Size,         Offset(Within Constant Buffer)
		std::map<std::string, std::pair<unsigned int, unsigned int>> constBufferVariables_;


	};
}



#endif //DISCOBREAKWINDOWS_DXCONSTANTBUFFER_H_