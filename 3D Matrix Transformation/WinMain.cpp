#include <Windows.h>

#include "Window.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   Window window(hInstance, 1280, 720, "Test DirectX");

   //bool exitProgram = false;
   //while (!exitProgram)
   //{
   //   if (const auto msgCode = Window::ProcessMessages()) 
   //      return *msgCode;
   //}

   return window.Start();

   //return 0;
}