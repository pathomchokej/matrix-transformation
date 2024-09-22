#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <string>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

struct CUSTOMVERTEX
{
   float X, Y, Z;
   DWORD COLOR;
   static const DWORD FVF;
};

class DX3D
{
public:
   DX3D(HWND hWnd, int screenWidth, int screenHeight);
   ~DX3D();

   bool Initialize();
   void Process();
   void Destroy();

   bool IsKeyPressed(BYTE key);

   bool CreateLine(CUSTOMVERTEX vertices[], UINT sizeOfVertex, LPDIRECT3DVERTEXBUFFER9* vertectBuffer);
   bool CreatePolygon(CUSTOMVERTEX vertices[], UINT sizeOfVertex, WORD indices[], UINT sizeofIndex, LPDIRECT3DVERTEXBUFFER9* vertectBuffer, LPDIRECT3DINDEXBUFFER9* indexBuffer);

   void StartDraw(D3DCOLOR backgroundColor, D3DXMATRIX* world);
   void EndDraw();
   void DrawPolygon(LPDIRECT3DVERTEXBUFFER9 vertectBuffer, LPDIRECT3DINDEXBUFFER9 indexBuffer, UINT numberOfVertices);
   void DrawMessage(std::string message, int left, int top, int right, int bottom, D3DCOLOR textColor);
   void DrawLine(LPDIRECT3DVERTEXBUFFER9 vertectBuffer);

private:
   HRESULT InitializeDX();
   HRESULT InitializeDXDevice();
   HRESULT InitializeDXFont();
   HRESULT InitializeDXCamera();
   HRESULT InitializeDXKeyboard();

   void KeyboardProcess();

private:
   HWND _hWnd;
   int _screenWidth;
   int _screenHeight;

   LPDIRECT3D9 _d3d;
   LPDIRECT3DDEVICE9 _d3dDevice;
   ID3DXFont* _font;
   LPDIRECTINPUT8 _dInput;
   LPDIRECTINPUTDEVICE8 _dKeyboard;

   LPDIRECT3DVERTEXBUFFER9 _vertectBuffer;
   LPDIRECT3DINDEXBUFFER9 _indexBuffer;
   BYTE _keyboardState[256];
};

