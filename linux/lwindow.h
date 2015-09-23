//
// Created by hakeee on 2015-09-06.
//

#ifndef DISCOBREAKLINUX_WINDOW_H
#define DISCOBREAKLINUX_WINDOW_H

#include <GL/glew.h>
#include <GL/glx.h>
#include <string.h>
#include <functional>
#include <sys/time.h>

#include "display.h"
#include "lggraphics.h"
#include "../generic/graphics.h"

namespace discobreak {
    class LGgraphics;
    class Lwindow {
        Display* display_;
        int height_, width_, x_, y_;
        unsigned int boarder_width_;

        GLint att_[5];
        XVisualInfo *vi_;
        Colormap cmap_;
        XSetWindowAttributes swa_;
        ::Window win_;
        XWindowAttributes gwa_;
        XEvent xev_;


        int x11_fd_;
        fd_set in_fds_;
        struct timeval tv_;

        timeval t1_, t2_;
        double elapsedTime_;

		Graphics* graphics_;


        std::function<void(double milliseconds)> draw_callback_;
        std::function<void(double milliseconds)> update_callback_;

    public:
        Lwindow(const std::string &name, GRAPHICS graphics);
        ~Lwindow();

        void SetDrawCallback(std::function<void(int milliseconds)> callback);

        void SetUpdateCallback(std::function<void(int milliseconds)> callback);

        void Run();

		Graphics* GetGraphics() { return graphics_; };

        int GetScreenHeight();
        int GetScreenWidth();

        friend class LGgraphics;
    };
};


#endif //DISCOBREAKLINUX_WINDOW_H
