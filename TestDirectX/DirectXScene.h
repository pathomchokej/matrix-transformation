#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

struct CUSTOMVERTEX
{
   float X, Y, Z;
   DWORD COLOR;
   static const DWORD FVF;
};

class DirectXScene
{
private:
   LPDIRECT3D9 d3d;
   LPDIRECT3DDEVICE9 d3dDevice;
   LPDIRECT3DVERTEXBUFFER9 vertectBuffer;
   LPDIRECT3DINDEXBUFFER9 indexBuffer;

   UINT _width;
   UINT _height;

public:
   DirectXScene();
   ~DirectXScene();

   HRESULT InitializeD3D(HWND hWnd);
   void InitializeGraphic();
   void InitializeCamera();
   void RenderFrame();
   void CleanD3D();

};

