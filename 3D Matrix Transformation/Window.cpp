#include <Windows.h>
#include <cstring>

#include "Window.h"
#include "WindowUtil.h"

Window::Window(HINSTANCE hInstance, int width = 1280, int height = 720, const char* name = "My Window")
{
   this->CreateWindowBluePrint(hInstance, width, height, name);
   this->CreateWindowInstance();
}

Window::~Window()
{
   Utility::Delete(_title);
}

int Window::Start()
{
   if (Initialize())
   {
      ShowWindow(_hWnd, SW_SHOWDEFAULT);
      UpdateWindow(_hWnd);

      return StartListener();
   }
   else
      return -1;
}


bool Window::Initialize()
{
   return true;
}


int Window::Process(float elapsedTime)
{
   return 0;
}


void Window::Destroy()
{
}

/////////////////////////////////////////////////////
LRESULT __stdcall WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
   Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

   switch (msg)
   {
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
   }

   return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::CreateWindowBluePrint(HINSTANCE hInstance, int width, int height, const char* name)
{
   _width = width;
   _height = height;
   _hInstance = hInstance;
   size_t lenght = strlen(name) + 1;
   if (lenght > 1)
   {
      _title = new char[lenght];
      strcpy_s(_title, lenght, name);
   }

   WNDCLASSEX wc;
   ZeroMemory(&wc, sizeof(WNDCLASSEX));

   wc.cbSize = sizeof(WNDCLASSEX);
   wc.style = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc = WindowProc;
   wc.hInstance = _hInstance;
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.lpszClassName = _title;

   RegisterClassEx(&wc);
}

void Window::CreateWindowInstance()
{
   DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
   RECT winRect;
   winRect.left = 100;
   winRect.right = _width + winRect.left;
   winRect.top = 100;
   winRect.bottom = _height + winRect.top;
   AdjustWindowRect(&winRect, style, FALSE);

   _hWnd = CreateWindowEx(NULL,
      _title, _title,
      style,
      GetSystemMetrics(SM_CXSCREEN) / 2 - _width / 2,
      GetSystemMetrics(SM_CYSCREEN) / 2 - _height / 2,
      _width, _height,
      nullptr, nullptr, _hInstance, this
   );

   SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

int Window::StartListener()
{
   MSG msg = {};

   __int64 countsPerSec = 0;
   QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
   //if (countsPerSec == 0) countsPerSec = 1;
   float secPerCount = 1.0f / countsPerSec;

   __int64 previous = 0;
   QueryPerformanceCounter((LARGE_INTEGER*)&previous);


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
         __int64 current = 0;
         QueryPerformanceCounter((LARGE_INTEGER*)&current);
         // interval
         float interval = (current - previous) * secPerCount;

         Process(interval);

         previous = current;
      }

      exitProgram = (msg.message == WM_QUIT);
   }

   Destroy();

   return (int)msg.wParam;
}