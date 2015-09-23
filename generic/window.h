//
// Created by hakeee on 2015-09-06.
//

#ifndef DISCOBREAK_WINDOW_H
#define DISCOBREAK_WINDOW_H

#include <string>
#include <functional>
#include "graphics.h"

#ifdef _WIN32
    #include "../windows/wwindow.h"
#else
    #include "../linux/lwindow.h"
#endif

namespace discobreak {
    class Window {
#ifdef _WIN32
        Wwindow window_;
#else
        Lwindow window_;
#endif
    public:
        // name = title, graphics = GRAPHICS::OPENGL or GRAPHICS::DIRECTX
        Window(const std::string &name, GRAPHICS graphics): window_(name, graphics) { };
        ~Window() { };

        // Sets the callback for each drawcall, callback = void function that takes a float as a parameter
        void SetDrawCallback(std::function<void(double milliseconds)> callback);

        // Sets the callback for each updatecall, callback = void function that takes a float as a parameter
        void SetUpdateCallback(std::function<void(double milliseconds)> callback);

        // shows the window and starts calling each callback
        void Run();

        // gets the graphics class pointer
		Graphics* GetGraphics() { return window_.GetGraphics(); };

		int GetScreenHeight();
		int GetScreenWidth();
    };
};
#endif //DISCOBREAK_WINDOW_H

