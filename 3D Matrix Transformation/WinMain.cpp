#include <Windows.h>

#include "Window.h"
#include "DirectXWindow.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   DirectXWindow window(hInstance, 1280, 720, "Test DirectX");

   return window.Start();
}