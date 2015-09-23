//
// Created by hakeee on 2015-09-06.
//

#ifndef DISCOBREAKLINUX_INPUT_H
#define DISCOBREAKLINUX_INPUT_H

#ifdef _WIN32
#include <Windows.h>
#endif //WIN32


#include <array>
namespace discobreak {
    class Input {
    public:

        static void SetKeyState(unsigned char key, bool value);

        static bool IsKeyDown(unsigned char key);

        static bool IsKeyPressed(unsigned char key);

        static bool IsKeyReleased(unsigned char key);

		static void PostUpdate();

    private:

        Input();
        ~Input();

        static std::array<bool, 255> previousKeyboardState_;
        static std::array<bool, 255> keyboardState_;
    };
};
#endif //DISCOBREAKLINUX_INPUT_H
