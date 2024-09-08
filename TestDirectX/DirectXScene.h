#pragma once

#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")

struct CUSTOMVERTEX
{
   float X, Y, Z, RHW;
   DWORD COLOR;
};

class DirectXScene
{
private:
   LPDIRECT3D9 d3d;
   LPDIRECT3DDEVICE9 d3dDevice;
   LPDIRECT3DVERTEXBUFFER9 vertectBuffer;

   UINT _width;
   UINT _height;

public:
   DirectXScene();
   ~DirectXScene();

   HRESULT InitializeD3D(HWND hWnd);
   void InitializeGraphic();
   void RenderFrame();
   void CleanD3D();

};

