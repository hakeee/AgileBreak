//
// Created by hakeee on 2015-09-09.
//

#ifndef DISCOBREAKWINDOWS_DXBUFFER_H
#define DISCOBREAKWINDOWS_DXBUFFER_H

#include <d3d11.h>


namespace discobreak {
	class Wdgraphics;
	class DxBuffer {

	private:
		ID3D11Buffer* vertexBuffer_;
		ID3D11Buffer* indexBuffer_;

		DxBuffer(Wdgraphics* graphics);


		Wdgraphics* graphics_;


	public:
		~DxBuffer();

		void Use(unsigned int stride);

		void SetVertexData(unsigned int count, float* array);

		void SetIndices(unsigned int count, unsigned int* array);

		friend class Buffer;
		friend class Wdgraphics;
	};

}
#endif //DISCOBREAKWINDOWS_DXBUFFER_H
