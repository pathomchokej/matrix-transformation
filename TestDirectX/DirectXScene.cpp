#include "DirectXScene.h"
#include "DirectXUtil.h"

DirectXScene::DirectXScene() 
{

}

DirectXScene::~DirectXScene()
{
   CleanD3D();
}

HRESULT DirectXScene::InitializeD3D(HWND hWnd)
{
   this->d3d = Direct3DCreate9(D3D_SDK_VERSION);
   if (this->d3d == NULL)
   {
      MessageBox(hWnd, "Direct3DCreate9 failed", NULL, NULL);
      return E_FAIL;
   }

   D3DCAPS9 d3dDeviceCaps;
   if (FAILED(this->d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dDeviceCaps)))
   {
      MessageBox(hWnd, "GetDeviceCaps failed", NULL, NULL);
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

   RECT rect;
   if (GetWindowRect(hWnd, &rect)) {
      _width = rect.right - rect.left;
      _height = rect.bottom - rect.top;
   }
   else {
      _width = 800;
      _height = 600;
   }

   D3DPRESENT_PARAMETERS d3dpp;

   ZeroMemory(&d3dpp, sizeof(d3dpp));
   d3dpp.BackBufferWidth = _width;
   d3dpp.BackBufferHeight = _height;
   d3dpp.Windowed = TRUE;
   d3dpp.BackBufferCount = 1;
   d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
   d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
   d3dpp.MultiSampleQuality = 0;
   d3dpp.hDeviceWindow = hWnd;
   d3dpp.Flags = 0;
   d3dpp.EnableAutoDepthStencil = true;
   d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
   d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
   d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
   d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

   if (FAILED(this->d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
      vp,
      &d3dpp, &this->d3dDevice))) {
      MessageBox(hWnd, "CreateDevice failed", NULL, NULL);
      return E_FAIL;
   }

   if (this->d3dDevice == NULL) {
      MessageBox(hWnd, "d3dDevice is null", NULL, NULL);
      return E_FAIL;
   }

   return NOERROR;
}

void DirectXScene::InitializeGraphic()
{
   // create the vertices using the CUSTOMVERTEX struct
   CUSTOMVERTEX vertices[] = {
      { 400.0f, 62.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
      { 650.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
      { 150.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
   };
   // create a vertex buffer interface called v_buffer
   this->d3dDevice->CreateVertexBuffer(
      3 * sizeof(CUSTOMVERTEX),
      0,
      D3DFVF_XYZRHW| D3DFVF_DIFFUSE,
      D3DPOOL_MANAGED,
      &this->vertectBuffer,
      NULL
   );
   VOID* pVoid; // a void pointer
   // lock v_buffer and load the vertices into it
   this->vertectBuffer->Lock(0, 0, (void**)&pVoid, 0);
   memcpy(pVoid, vertices, sizeof(vertices));
   this->vertectBuffer->Unlock();
}

void DirectXScene::RenderFrame()
{
   d3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, DXColors::CornflowerBlue, 1.0f, 0);

   d3dDevice->Present(0, 0, 0, 0);
}

void DirectXScene::CleanD3D()
{

}