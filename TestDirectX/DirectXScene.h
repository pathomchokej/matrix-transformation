#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

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

   D3DXMATRIX world;

   float xRotation;
   float yRotation;
   float zRotation;

   float xScale;
   float yScale;
   float zScale;

   float xTranslation;
   float yTranslation;
   float zTranslation;

   float _fps;
   float _elapsedTime;
   int _count;

   UINT _width;
   UINT _height;
   HWND _hWnd;

   ID3DXFont* _font;
   RECT _fRect;
   std::string _message;

   std::string PrintMatrix(D3DXMATRIX* matrix);

public:
   DirectXScene();
   ~DirectXScene();

   HRESULT InitializeD3D(HWND hWnd);
   void InitializeGraphic();
   void InitializeCamera();
   void Process(float interval);
   void CalculateFPS(float interval);
   void RenderFrame();
   void CleanD3D();

};

