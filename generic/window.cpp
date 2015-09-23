//
// Created by hakeee on 2015-09-10.
//

#include "window.h"

void discobreak::Window::SetDrawCallback(std::function<void(double milliseconds)> callback) { window_.SetDrawCallback(callback); }

void discobreak::Window::SetUpdateCallback(std::function<void(double milliseconds)> callback) { window_.SetUpdateCallback(callback); }

void discobreak::Window::Run() { window_.Run(); }

int discobreak::Window::GetScreenHeight() { return window_.GetScreenHeight(); }
int discobreak::Window::GetScreenWidth() { return window_.GetScreenWidth(); }