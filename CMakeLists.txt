cmake_minimum_required(VERSION 3.3)

set(SOURCE_FILES main.cpp generic/window.h generic/window.cpp generic/input.h generic/input.cpp generic/graphics.h generic/graphics.cpp 
        generic/shader.cpp generic/shader.h generic/buffer.cpp generic/buffer.h 
        generic/opengl/glbuffer.cpp generic/opengl/glbuffer.h generic/opengl/glshader.cpp generic/opengl/glshader.h
        generic/texture.h generic/texture.cpp generic/opengl/gltexture.cpp generic/opengl/gltexture.h)

if(UNIX)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DGLM_FORCE_RADIANS")# -Wall -Wextra")


project(DiscobreakLinux)
set(SOURCE_LINUX linux/lwindow.cpp linux/lwindow.h linux/display.cpp linux/display.h linux/lggraphics.cpp linux/lggraphics.h)
link_directories(/usr/local/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
add_executable(DiscobreakLinux ${SOURCE_FILES} ${SOURCE_LINUX})
target_link_libraries(DiscobreakLinux X11 GLEW GLU GL  pthread)

elseif(WIN32)#unix

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 -DGLM_FORCE_RADIANS")

project(DiscobreakWindows)
add_definitions(-D_crt_secure_no_warnings)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/windows/glew/include ${CMAKE_CURRENT_SOURCE_DIR}/generic)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/windows/glew/lib/ReleaseLibs/Win32/)
set(SOURCE_WINDOWS windows/wwindow.h windows/wwindow.cpp windows/wdgraphics.h windows/wdgraphics.cpp windows/wggraphics.h
        windows/wggraphics.cpp windows/directx/dxbuffer.cpp windows/directx/dxbuffer.h windows/directx/dxshader.cpp 
        windows/directx/dxshader.h windows/directx/dxconstantbuffer.h windows/directx/dxconstantbuffer.cpp
        windows/directx/shaders/textureShaderv.hlsl windows/directx/shaders/textureShaderp.hlsl
		windows/directx/dxtexture.cpp windows/directx/dxtexture.h windows/directx/ddstextureloader.h windows/directx/ddstextureloader.cpp)
		
SET_SOURCE_FILES_PROPERTIES(windows/directx/shaders/textureShaderv.hlsl PROPERTIES VS_SHADER_TYPE Vertex VS_SHADER_MODEL 5.0 VS_SHADER_ENTRYPOINT main)
SET_SOURCE_FILES_PROPERTIES(windows/directx/shaders/textureShaderp.hlsl PROPERTIES VS_SHADER_TYPE Pixel VS_SHADER_MODEL 5.0 VS_SHADER_ENTRYPOINT main)

add_executable(DiscobreakWindows ${SOURCE_FILES} ${SOURCE_WINDOWS})
target_link_libraries(DiscobreakWindows d3dcompiler d3d11 dxguid glew32 glu32 opengl32)

endif()#win32
