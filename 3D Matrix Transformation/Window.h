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

