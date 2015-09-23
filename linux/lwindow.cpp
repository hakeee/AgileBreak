//
// Created by hakeee on 2015-09-06.
//

#include <assert.h>
#include <X11/Xlib.h>
#include "lwindow.h"
#include "../generic/window.h"
#include "../generic/input.h"

discobreak::Lwindow::Lwindow(const std::string &name, GRAPHICS graphics) : display_(new Display(":0")) {

    assert(graphics != GRAPHICS::DIRECTX);

    XSetErrorHandler([](::Display *disp, XErrorEvent *error)->int {
        char errorText[512];
        //char errorData[512];
        XGetErrorText(disp, error->error_code, errorText, 512);
        //XGetErrorDatabaseText(disp, "wng_exec", "idk","A error occourd", errorData, 512);
        fprintf(stderr, "ErrorText: %s\n", errorText);
        return 0;
    });

    this->width_ = 1280;
    this->height_ = 720;
    this->x_ = 0;
    this->y_ = 0;
    this->boarder_width_ = 0;
    int tatt[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    memcpy(this->att_, tatt , sizeof(int) * 5);

    vi_ = glXChooseVisual(display_->xdisplay_, 0, tatt);

    if(vi_ == nullptr) {
        fprintf(stderr, "No appropiete visual found");
        exit(EXIT_FAILURE);
    }

    cmap_ = XCreateColormap(display_->xdisplay_, display_->root_window_, vi_->visual, AllocNone);
    swa_.colormap = cmap_;
    swa_.event_mask = /*ExposureMask | */KeyPressMask | KeyReleaseMask;/* |
                             ButtonPress | ButtonRelease | PointerMotionMask;*/

    win_ = XCreateWindow(display_->xdisplay_, display_->root_window_, x_, y_, (unsigned)width_, (unsigned)height_, boarder_width_,
                         vi_->depth, InputOutput, vi_->visual, CWColormap | CWEventMask | CWBorderPixel, &swa_);

    XStoreName(display_->xdisplay_, win_, name.c_str());

    // Can shange to be used over network with GL_FALSE instead.
	graphics_ = new Graphics(new LGgraphics(this));
    glXMakeCurrent(display_->xdisplay_, win_, graphics_->gg_->glc_);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        exit(EXIT_FAILURE);

    glEnable(GL_DEPTH_TEST);
}

discobreak::Lwindow::~Lwindow() {
    delete graphics_;
    XDestroyWindow(display_->xdisplay_, win_);
    delete display_;
}

void discobreak::Lwindow::SetDrawCallback(std::function<void(int milliseconds)> callback) {
    draw_callback_ = callback;
}

void discobreak::Lwindow::SetUpdateCallback(std::function<void(int milliseconds)> callback) {
    update_callback_ = callback;
}

void discobreak::Lwindow::Run() {
    XMapWindow(display_->xdisplay_, win_);

    x11_fd_ = ConnectionNumber(display_->xdisplay_);
    gettimeofday(&t1_, NULL);

    glXMakeCurrent(display_->xdisplay_, win_, graphics_->gg_->glc_);
    Atom wmDeleteMessage = XInternAtom(display_->xdisplay_, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display_->xdisplay_, win_, &wmDeleteMessage, 1);
    bool running = true;
    while (running) {
        // Create a File Description Set containing x11_fd
        FD_ZERO(&in_fds_);
        FD_SET(x11_fd_, &in_fds_);

        // Set our timer.  One second sounds good.
        tv_.tv_usec = 16660;
        tv_.tv_sec = 0;

        // Wait for X Event or a Timer
        if (select(x11_fd_+1, &in_fds_, 0, 0, &tv_)) {
            if(XPending(display_->xdisplay_))
                XNextEvent(display_->xdisplay_, &xev_);
            if (xev_.xclient.data.l[0] == static_cast<long>(wmDeleteMessage)) {
                running = false;
            } else if (xev_.type == KeyPress || xev_.type == KeyRelease) {
                Input::SetKeyState((char)XLookupKeysym(&xev_.xkey, xev_.xkey.state), xev_.type == KeyPress);
                //printf("%i\n", (int)XLookupKeysym(&xev_.xkey, xev_.xkey.state));
            } else if(xev_.type == DestroyNotify)
                running = false;
            xev_ = XEvent();
        }
        else {
            // Handle timer here
            XGetWindowAttributes(display_->xdisplay_, win_, &gwa_);
            glViewport(0, 0, gwa_.width, gwa_.height);
            gettimeofday(&t2_, NULL);
            elapsedTime_ = (t2_.tv_sec - t1_.tv_sec) * 1000.0;      // sec to ms
            elapsedTime_ += (t2_.tv_usec - t1_.tv_usec) / 1000.0;   // us to ms
            if(update_callback_) update_callback_(elapsedTime_);
            if(draw_callback_) draw_callback_(elapsedTime_);
			Input::PostUpdate();
            gettimeofday(&t1_, NULL);
            glXSwapBuffers(display_->xdisplay_, win_);
        }
    }
    return;
}

int discobreak::Lwindow::GetScreenHeight() {
    return gwa_.height;
}

int discobreak::Lwindow::GetScreenWidth() {
    return gwa_.width;
}
