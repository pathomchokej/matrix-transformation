#include <Windows.h>
#include <sstream>

#include "DX3D.h"

namespace DX {

   template <class T> void ReleaseUnacquire(T t) {
      if (t)
      {
         t->Unacquire();
         t->Release();
         t = NULL;
      }
   }

   template <class T> void Release(T t) {
      if (t)
      {
         t->Release();
         t = NULL;
      }
   }

   template <class T> void Delete(T t) {
      if (t)
      {
         delete t;
         t = NULL;
      }
   }
}

const DWORD CUSTOMVERTEX::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;


DX3D::DX3D(HWND hWnd, int screenWidth, int screenHeight)
{
   _hWnd = hWnd;
   _screenWidth = screenWidth;
   _screenHeight = screenHeight;
}

DX3D::~DX3D()
{

}

bool DX3D::Initialize()
{
   if (FAILED(InitializeDX())) return false;
   if (FAILED(InitializeDXDevice())) return false;
   if (FAILED(InitializeDXFont())) return false;
   if (FAILED(InitializeDXCamera())) return false;
   if (FAILED(InitializeDXKeyboard())) return false;

   return true;
}

void DX3D::Process()
{
   KeyboardProcess();
}

void DX3D::Destroy()
{
   DX::ReleaseUnacquire(_dKeyboard);
   DX::Release(_dInput);
   DX::Release(_font);
   DX::Release(_d3dDevice);
   DX::Release(_d3d);
}

bool DX3D::IsKeyPressed(BYTE key) {
   // Check if the key is pressed in the state buffer
   return (_keyboardState[key] & 0x80) != 0;
}

bool DX3D::CreateLine(CUSTOMVERTEX vertices[], UINT sizeOfVertex, LPDIRECT3DVERTEXBUFFER9* vertectBuffer)
{
   // create a vertex buffer interface called v_buffer
   HRESULT result = _d3dDevice->CreateVertexBuffer(
      sizeOfVertex,
      0,
      CUSTOMVERTEX::FVF,
      D3DPOOL_DEFAULT,
      vertectBuffer,
      NULL
   );

   VOID* pVoid; // a void pointer
   // lock v_buffer and load the vertices into it
   result = (*vertectBuffer)->Lock(0, 0, (void**)&pVoid, 0);
   memcpy(pVoid, vertices, sizeOfVertex);
   result = (*vertectBuffer)->Unlock();

   return result == S_OK;
}

bool DX3D::CreatePolygon(
   CUSTOMVERTEX vertices[], UINT sizeOfVertex, 
   WORD indices[], UINT sizeofIndex, 
   LPDIRECT3DVERTEXBUFFER9* vertectBuffer, LPDIRECT3DINDEXBUFFER9* indexBuffer)
{

   // create a vertex buffer interface called v_buffer
   HRESULT result = _d3dDevice->CreateVertexBuffer(
      sizeOfVertex,
      0,
      CUSTOMVERTEX::FVF,
      D3DPOOL_MANAGED,
      vertectBuffer,
      NULL
   );
   // create indexing 
   result = _d3dDevice->CreateIndexBuffer(
      sizeofIndex,
      D3DUSAGE_WRITEONLY,
      D3DFMT_INDEX16,
      D3DPOOL_MANAGED,
      indexBuffer,
      NULL
   );

   VOID* pVoid; // a void pointer
   // lock v_buffer and load the vertices into it
   result = (*vertectBuffer)->Lock(0, 0, (void**)&pVoid, 0);
   memcpy(pVoid, vertices, sizeOfVertex);
   result = (*vertectBuffer)->Unlock();

   VOID* pIndices;
   result = (*indexBuffer)->Lock(0, 0, (void**)&pIndices, 0);
   memcpy(pIndices, indices, sizeofIndex);
   result = (*indexBuffer)->Unlock();

   return true;
}


void DX3D::StartDraw(D3DCOLOR backgroundColor, D3DXMATRIX* world)
{
   _d3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, backgroundColor, 1.0f, 0);
   _d3dDevice->BeginScene();
   _d3dDevice->SetTransform(D3DTS_WORLD, world);
}

void DX3D::EndDraw()
{
   _d3dDevice->EndScene();
   _d3dDevice->Present(0, 0, 0, 0);
}

void DX3D::DrawPolygon(LPDIRECT3DVERTEXBUFFER9 vertectBuffer, LPDIRECT3DINDEXBUFFER9 indexBuffer, UINT numberOfVertices )
{
   UINT primCount = numberOfVertices / 3;
   _d3dDevice->SetStreamSource(0, vertectBuffer, 0, sizeof(CUSTOMVERTEX));
   _d3dDevice->SetIndices(indexBuffer);
   _d3dDevice->SetFVF(CUSTOMVERTEX::FVF);
   _d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numberOfVertices, 0, primCount);
}

void DX3D::DrawMessage(std::string message, int left, int top, int right, int bottom, D3DCOLOR textColor)
{
   RECT rect;
   SetRect(&rect, 0, 0, 400, 200);

   if (_font) {
      _font->DrawTextA(NULL, message.c_str(), -1, &rect, DT_LEFT, textColor);
   }
}

void DX3D::DrawLine(LPDIRECT3DVERTEXBUFFER9 vertectBuffer)
{
   _d3dDevice->SetStreamSource(0, vertectBuffer, 0, sizeof(CUSTOMVERTEX));
   _d3dDevice->SetFVF(CUSTOMVERTEX::FVF);
   _d3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
}

/////////////////////////////////////////////////////////////////////////////////

HRESULT DX3D::InitializeDX()
{
   _d3d = Direct3DCreate9(D3D_SDK_VERSION);
   if (_d3d == NULL)
   {
      MessageBox(_hWnd, "Direct3DCreate9 failed", NULL, NULL);
      return E_FAIL;
   }

   return S_OK;
}

HRESULT DX3D::InitializeDXDevice()
{
   D3DCAPS9 d3dDeviceCaps;
   if (FAILED(_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dDeviceCaps)))
   {
      MessageBox(_hWnd, "GetDeviceCaps failed", NULL, NULL);
      return E_FAIL;
   }

   // Check device capabilities
   int vp;
   if (d3dDeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
      vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
   }
   else {
      vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
   }

   D3DPRESENT_PARAMETERS d3dpp;

   ZeroMemory(&d3dpp, sizeof(d3dpp));
   d3dpp.BackBufferWidth = _screenWidth;
   d3dpp.BackBufferHeight = _screenHeight;
   d3dpp.Windowed = TRUE;
   d3dpp.BackBufferCount = 1;
   d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
   d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
   d3dpp.MultiSampleQuality = 0;
   d3dpp.hDeviceWindow = _hWnd;
   d3dpp.Flags = 0;
   d3dpp.EnableAutoDepthStencil = true;
   d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
   d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
   d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
   d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

   if (FAILED(_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd,
      vp,
      &d3dpp, &_d3dDevice))) {
      MessageBox(_hWnd, "CreateDevice failed", NULL, NULL);
      return E_FAIL;
   }

   if (_d3dDevice == NULL) {
      MessageBox(_hWnd, "d3dDevice is null", NULL, NULL);
      return E_FAIL;
   }

   D3DVIEWPORT9 viewPort;
   ZeroMemory(&viewPort, sizeof(D3DVIEWPORT9));
   viewPort.X = 0;
   viewPort.Y = 0;
   viewPort.Width = _screenWidth;
   viewPort.Height = _screenHeight;
   viewPort.MinZ = 0;
   viewPort.MaxZ = 1;

   _d3dDevice->SetViewport(&viewPort);

   return S_OK;
}

HRESULT DX3D::InitializeDXFont()
{
   // Create font
   if (FAILED(D3DXCreateFont(_d3dDevice, 20, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
      FF_DONTCARE, "Arial", &_font))) {
      MessageBox(_hWnd, "D3DXCreateFont failed", NULL, NULL);
      return E_FAIL;
   }

   return S_OK;
}

HRESULT DX3D::InitializeDXCamera() {
   D3DXMATRIX view;
   D3DXMATRIX proj;

   D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
   D3DXVECTOR3 target = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
   D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

   D3DXMatrixLookAtLH(&view, &position, &target, &up);
   _d3dDevice->SetTransform(D3DTS_VIEW, &view);

   D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, static_cast<float>(_screenWidth) / _screenHeight, 1.0f, 1000.0f);
   _d3dDevice->SetTransform(D3DTS_PROJECTION, &proj);

   _d3dDevice->SetRenderState(D3DRS_LIGHTING, false);
   _d3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

   return S_OK;
}

HRESULT DX3D::InitializeDXKeyboard()
{
   HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(_hWnd, GWLP_HINSTANCE);

   // Step 1: Create the DirectInput object
   if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_dInput, NULL))) {
      return E_FAIL;
   }

   // Step 2: Create a device for the keyboard
   if (FAILED(_dInput->CreateDevice(GUID_SysKeyboard, &_dKeyboard, NULL))) {
      return E_FAIL;
   }

   // Step 3: Set the data format (we want the keyboard data format)
   if (FAILED(_dKeyboard->SetDataFormat(&c_dfDIKeyboard))) {
      return E_FAIL;
   }

   // Step 4: Set the cooperative level (we'll set it to exclusive and foreground mode)
   if (FAILED(_dKeyboard->SetCooperativeLevel(_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
      return E_FAIL;
   }

   // Step 5: Acquire the keyboard
   _dKeyboard->Acquire();

   return S_OK;
}

void DX3D::KeyboardProcess() {
   HRESULT hr = _dKeyboard->GetDeviceState(sizeof(_keyboardState), (LPVOID)&_keyboardState);

   // If the device lost focus or is not acquired, try to re-acquire it
   if (FAILED(hr)) {
      if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) {
         _dKeyboard->Acquire();
      }
   }
}
