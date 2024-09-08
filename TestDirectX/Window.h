#pragma once




class Window
{
public:
   Window(HINSTANCE hInstance, int width, int height, const char* name);
   ~Window();
   static int ProcessMessages();

private:
   static LRESULT __stdcall WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
   int width;
   int height;
   HWND hWnd;
   const char* windowName = "Test DirectX";
   HINSTANCE hInstance;

   DirectXScene* dx;
};
