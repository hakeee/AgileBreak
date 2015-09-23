#include <iostream>
#include <vector>

#include "generic/window.h"
#include "generic/input.h"

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace std;

discobreak::GRAPHICS g = discobreak::GRAPHICS::OPENGL;

int main() {
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    cout << "Hello, World!" << endl;

    discobreak::Window win("Titel", g);
    win.GetGraphics()->PrintVersion();


    float fixms = 32;
    bool debugUpdate = false;
	win.SetUpdateCallback([&](float ms)->void {
        if(discobreak::Input::IsKeyPressed('E'))
            debugUpdate = true;
        else if(discobreak::Input::IsKeyPressed('R'))
            debugUpdate = false;

        if(discobreak::Input::IsKeyPressed('W'))
            fixms *= 1.2f;
        else if(discobreak::Input::IsKeyPressed('S'))
            fixms /= 1.2f;

		if (discobreak::Input::IsKeyDown('X'))
			exit(0);

        if(debugUpdate)
            ms = fixms;
	});
    
    win.SetDrawCallback([&](float ms)->void {
		win.GetGraphics()->Swap();
	});
    win.Run();

    return 0;
}

