//
// Created by hakeee on 2015-09-06.
//

#ifndef DISCOBREAKLINUX_DISPLAY_H
#define DISCOBREAKLINUX_DISPLAY_H


#include <X11/Xlib.h>
#include <string>

namespace discobreak {
    class Lwindow;
    class LGgraphics;
    class Display {
        /* this variable will be used to store the "default" screen of the  */
        /* X server. usually an X server has only one screen, so we're only */
        /* interested in that screen.                                       */
        int screen_num_;

        /* these variables will store the size of the screen, in pixels.    */
        int screen_width_;
        int screen_height_;

        /* this variable will be used to store the ID of the root window of our */
        /* screen. Each screen always has a root window that covers the whole   */
        /* screen, and always exists.                                           */
        Window root_window_;

        /* these variables will be used to store the IDs of the black and white */
        /* colors of the given screen. More on this will be explained later.    */
        unsigned long white_pixel_;
        unsigned long black_pixel_;

        std::string name_;

        ::Display *xdisplay_;

    public:
        Display(const char *display_name);

        ~Display();

        friend class Lwindow;
        friend class LGgraphics;
    };
};

#endif //DISCOBREAKLINUX_DISPLAY_H
