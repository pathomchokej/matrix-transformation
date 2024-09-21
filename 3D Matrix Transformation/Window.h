#pragma once

class Window
{
public:
   Window(HINSTANCE hInstance, int width, int height, const char* name);
   ~Window();
   int Start();

protected:
   virtual bool Initialize();
   virtual int Process(float elapsedTime);
   virtual void Destroy();

   int GetWidth() const { return _width; }
   int GetHeight() const{ return _height; }
   HWND GetHandleWindow() const { return _hWnd; }
   HINSTANCE GetInstance() const { return _hInstance; }

private:
   int _width;
   int _height;
   char* _title;
   HWND _hWnd;
   HINSTANCE _hInstance;

private:
   void CreateWindowBluePrint(HINSTANCE hInstance, int width, int height, const char* name);
   void CreateWindowInstance();
   int StartListener();
};

