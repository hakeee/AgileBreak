//
// Created by hakeee on 2015-09-06.
//

#ifndef DISCOBREAKWINDOWS_WINDOW_H
#define DISCOBREAKWINDOWS_WINDOW_H
#ifdef _WIN32

#include <Windows.h>
#include <functional>
#include <iostream>
#include "../generic/graphics.h"

namespace discobreak {
	class Wdgraphics;
	class Wggraphics;
    class Wwindow {
    public:
        Wwindow(std::string name, GRAPHICS graphics);
        ~Wwindow();


        void Run();


        void SetDrawCallback(std::function<void(double milliseconds)> callback) { draw_callback_ = callback; };

        void SetUpdateCallback(std::function<void(double milliseconds)> callback) { update_callback_ = callback; };

		Graphics* GetGraphics() { return graphics_; };

		int GetScreenHeight();
		int GetScreenWidth();

    private:
        HINSTANCE hInstance_ = 0;
        HWND hWnd_ = 0;


        std::function<void(double milliseconds)> draw_callback_;
        std::function<void(double milliseconds)> update_callback_;

        static LRESULT CALLBACK Wwindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		//Wdgraphics* wdgraphics = nullptr;
		Graphics* graphics_;

        //Temp variables
        LONG windowWidth_ = 1280;
        LONG windowHeight_ = 720;

		friend class Wdgraphics;
		friend class Wggraphics;
    };
};
#endif _WIN32
#endif //DISCOBREAKWINDOWS_WINDOW_H
