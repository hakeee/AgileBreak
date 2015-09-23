//
// Created by hakeee on 2015-09-06.
//

#include <stdlib.h>
#include <stdio.h>
#include "display.h"

discobreak::Display::Display(const char *dname): name_(dname) {
    xdisplay_ = XOpenDisplay(dname);
    if(xdisplay_ == nullptr) {
        fprintf(stderr, "Cannot connect to X server %s\n", dname);
        exit(-1);
    }

    /* check the number of the default screen for our X server. */
    screen_num_ = DefaultScreen(xdisplay_);

    /* find the width of the default screen of our X server, in pixels. */
    screen_width_ = DisplayWidth(xdisplay_, screen_num_);

    /* find the height of the default screen of our X server, in pixels. */
    screen_height_ = DisplayHeight(xdisplay_, screen_num_);

    /* find the ID of the root window of the screen. */
    root_window_ = RootWindow(xdisplay_, screen_num_);

    /* find the value of a white pixel on this screen. */
    white_pixel_ = WhitePixel(xdisplay_, screen_num_);

    /* find the value of a black pixel on this screen. */
    black_pixel_ = BlackPixel(xdisplay_, screen_num_);
}

discobreak::Display::~Display() {
    XCloseDisplay(xdisplay_);
}

