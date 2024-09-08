#include "DirectXScene.h"
#include "DirectXUtil.h"

const DWORD CUSTOMVERTEX::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;


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

   D3DVIEWPORT9 viewPort;
   ZeroMemory(&viewPort, sizeof(D3DVIEWPORT9));
   viewPort.X = 0;
   viewPort.Y = 0;
   viewPort.Width = _width;
   viewPort.Height = _height;
   viewPort.MinZ = 0;
   viewPort.MaxZ = 1;

   d3dDevice->SetViewport(&viewPort);

   return NOERROR;
}

void DirectXScene::InitializeGraphic()
{
   // create the vertices using the CUSTOMVERTEX struct
   CUSTOMVERTEX vertices[] = {
      { 0.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
      { 1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0), },
      { -1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0), },
   };
   // create a vertex buffer interface called v_buffer
   this->d3dDevice->CreateVertexBuffer(
      3 * sizeof(CUSTOMVERTEX),
      0,
      CUSTOMVERTEX::FVF,
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

void DirectXScene::InitializeCamera() {
   D3DXMATRIX view;
   D3DXMATRIX proj;

   D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
   D3DXVECTOR3 target = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
   D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

   D3DXMatrixLookAtLH(&view, &position, &target, &up);
   this->d3dDevice->SetTransform(D3DTS_VIEW, &view);

   D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, static_cast<float>(_width) / _height, 1.0f, 1000.0f);
   this->d3dDevice->SetTransform(D3DTS_PROJECTION, &proj);

   this->d3dDevice->SetRenderState(D3DRS_LIGHTING, false);
   this->d3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
}


void DirectXScene::RenderFrame()
{
   d3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, DXColors::CornflowerBlue, 1.0f, 0);

   d3dDevice->BeginScene();

   d3dDevice->SetStreamSource(0, vertectBuffer, 0, sizeof(CUSTOMVERTEX));
   d3dDevice->SetFVF(CUSTOMVERTEX::FVF);
   d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

   d3dDevice->EndScene();

   d3dDevice->Present(0, 0, 0, 0);
}

void DirectXScene::CleanD3D()
{

}