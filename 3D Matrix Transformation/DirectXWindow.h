#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <string>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

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
   std::string PrintTranslation();
   std::string PrintRotation();
   std::string PrintScale();

   HRESULT InitializeDX();
   HRESULT InitializeDXDevice();
   HRESULT InitializeDXFont();
   HRESULT InitializeDXCamera();
   HRESULT InitializeDXCube();
   HRESULT InitializeDXKeyboard();

   void DXProcess(float interval);
   void DXCalculateFPS(float interval);
   void DXRender();
   void DXPollKeyboard();
   void DXKeyboardProcess();


   bool IsKeyPressed(BYTE key);

private:
   LPDIRECT3D9 d3d;
   LPDIRECT3DDEVICE9 d3dDevice;
   LPDIRECT3DVERTEXBUFFER9 vertectBuffer;
   LPDIRECT3DINDEXBUFFER9 indexBuffer;

   D3DXMATRIX world;

   bool autoRotation;
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

   LPDIRECTINPUT8 dInput = NULL;
   LPDIRECTINPUTDEVICE8 dKeyboard = NULL;
   BYTE keyboardState[256];
};