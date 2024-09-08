#include <Windows.h>

#include "DirectXScene.h"
#include "Window.h"

DirectXScene* g_directX = NULL;

Window::Window(HINSTANCE hInstance, int width, int height, const char* name) :
   width(width), height(height), hInstance(hInstance)
{
   WNDCLASSEX wc;
   ZeroMemory(&wc, sizeof(WNDCLASSEX));

   wc.cbSize = sizeof(WNDCLASSEX);
   wc.style = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc = WindowProc;
   wc.hInstance = hInstance;
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.lpszClassName = windowName;

   RegisterClassEx(&wc);

   DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
   RECT winRect;
   winRect.left = 100;
   winRect.right = width + winRect.left;
   winRect.top = 100;
   winRect.bottom = height + winRect.top;
   AdjustWindowRect(&winRect, style, FALSE);

   this->hWnd = CreateWindowEx(NULL, 
      windowName, name,
      style,
      GetSystemMetrics(SM_CXSCREEN)/2 - width/2, 
      GetSystemMetrics(SM_CYSCREEN)/2 - height/2,
      width, height,
      nullptr, nullptr, hInstance, this
   );

   ShowWindow(hWnd, SW_SHOWDEFAULT);

   // initialize directX 3D
   this->dx = new DirectXScene();
   this->dx->InitializeD3D(this->hWnd);
   this->dx->InitializeGraphic();
   this->dx->InitializeCamera();
   g_directX = dx;
}

Window::~Window()
{
   if (this->dx != NULL) {
      delete this->dx;
   }

   UnregisterClass(windowName, hInstance);
   DestroyWindow(hWnd);
}

int Window::ProcessMessages()
{
   MSG msg = {};

   bool exitProgram = false;
   while (!exitProgram)
   {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
      else
      {
         // TODO: wait for render
         if (g_directX != NULL) {
            g_directX->RenderFrame();
         }
      }

      exitProgram = (msg.message == WM_QUIT);
   }

   return (int)msg.wParam;
}

LRESULT __stdcall Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch (msg)
   {
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   }

   return DefWindowProc(hWnd, msg, wParam, lParam);
}
