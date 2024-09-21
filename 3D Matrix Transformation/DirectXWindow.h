#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class DirectXWindow : public Window
{
public:
   DirectXWindow(HINSTANCE hInstance, int width, int height, const char* name);

protected:
   bool Initialize() override;
   int Process(float elapsedTime) override;
   virtual void Destroy() override;

private:
   std::string PrintMatrix(D3DXMATRIX* matrix);
   HRESULT InitializeDX();
   HRESULT InitializeDXDevice();
   HRESULT InitializeDXFont();
   HRESULT InitializeDXCamera();
   HRESULT InitializeDXCube();

   void DXProcess(float interval);
   void DXCalculateFPS(float interval);
   void DXRender();

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

   ID3DXFont* _font;
   RECT _fRect;
   std::string _message;
};

