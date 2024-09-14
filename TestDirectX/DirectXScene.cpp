#include <sstream>
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
   _hWnd = hWnd;

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

   // Create font
   if (FAILED(D3DXCreateFont(this->d3dDevice, 20, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
      FF_DONTCARE, "Arial", &_font))) {
      MessageBox(hWnd, "D3DXCreateFont failed", NULL, NULL);
      return E_FAIL;
   }

   SetRect(&_fRect, 0, 0, 400, 200);
   _message = "| 1, 1, 1, 1 |\n";
   _message += "| 1, 1, 1, 1 |\n";
   _message += "| 1, 1, 1, 1 |\n";
   _message += "| 1, 1, 1, 1 |\n";

   return NOERROR;
}

void DirectXScene::InitializeGraphic()
{
   // create the vertices using the CUSTOMVERTEX struct
   CUSTOMVERTEX vertices[] = {
      // triangle
      //{ 0.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
      //{ -1.0f, -1.0f, 0.0f, DXColors::Red, },
      //{ 1.0f, 1.0f, 0.0f, DXColors::Blue, },

      // X,Y
      // -1, 1 |  1, 1
      // -------------
      // -1,-1 |  1, -1

      // square 
      //{ -1.0f, 1.0f, 0.0f, DXColors::Green, },
      //{ 1.0f, 1.0f, 0.0f, DXColors::Blue, },
      //{ -1.0f, -1.0f, 0.0f, DXColors::Red, },
      //{ 1.0f, -1.0f, 0.0f, DXColors::Purple, },

      // cube
      // front
      { -1.0f, 1.0f, -1.0f, DXColors::Green, }, // 0
      { 1.0f, 1.0f, -1.0f, DXColors::Blue, },   // 1
      { -1.0f, -1.0f, -1.0f, DXColors::Red, },  // 2
      { 1.0f, -1.0f, -1.0f, DXColors::Purple, },// 3
      // back
      { -1.0f, 1.0f, 1.0f, DXColors::Green, },  // 4
      { 1.0f, 1.0f, 1.0f, DXColors::Blue, },    // 5
      { -1.0f, -1.0f, 1.0f, DXColors::Red, },   // 6
      { 1.0f, -1.0f, 1.0f, DXColors::Purple, }, // 7
   };
   
   // X,Y (front)
   // -1, 1 |  1, 1
   // -------------
   // -1,-1 |  1, -1

   // Z,Y (Right)
   // -1, 1 |  1, 1
   // -------------
   // -1,-1 |  1, -1

   // CW is outside

   WORD indices[]
   {
      // Front face (z = -1)
      0, 1, 2, // CW
      3, 2, 1,  // CW
      // Back face (z = 1)
      5, 4, 7,
      4, 6, 7,
      // Right face (x = 1)
      //1, 5, 7,
      1, 5, 3,
      3, 5, 7,
      // Left face ( x = -1)
      4, 0, 6,
      6, 0, 2,
      // top face ( y = 1)
      4, 5, 0,
      0, 5, 1,
      // bottom face (y = -1)
      2, 3, 6,
      6, 3, 7
   };

   // create a vertex buffer interface called v_buffer
   this->d3dDevice->CreateVertexBuffer(
      8 * sizeof(CUSTOMVERTEX),
      0,
      CUSTOMVERTEX::FVF,
      D3DPOOL_MANAGED,
      &this->vertectBuffer,
      NULL
   );
   // create indexing 
   this->d3dDevice->CreateIndexBuffer(
      36 * sizeof(WORD),
      D3DUSAGE_WRITEONLY,
      D3DFMT_INDEX16,
      D3DPOOL_MANAGED,
      &this->indexBuffer,
      NULL
   );

   VOID* pVoid; // a void pointer
   // lock v_buffer and load the vertices into it
   this->vertectBuffer->Lock(0, 0, (void**)&pVoid, 0);
   memcpy(pVoid, vertices, sizeof(vertices));
   this->vertectBuffer->Unlock();

   VOID* pIndices;
   this->indexBuffer->Lock(0, 0, (void**)&pIndices, 0);
   memcpy(pIndices, indices, sizeof(indices));
   this->indexBuffer->Unlock();
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

std::string DirectXScene::PrintMatrix(D3DXMATRIX* matrix) {
   std::string result;
   int maxRow = 4;
   int maxCol = 4;
   std::string space = "   ";

   for (int row = 0; row < maxRow; row++) {
      result += "|" + space;
      for (int col = 0; col < maxCol; col++) {
         if (col > 0)   result += "," + space;
         result += std::format("{:.2f}", matrix->m[row][col]);
      }
      result += space + "|\n";
   }

   return result;
}

void DirectXScene::Process(float interval)
{
   this->xRotation += interval;
   this->yRotation += interval;
   this->zRotation += interval;

   D3DXMATRIX matrixRX, matrixRY, matrixRZ;
   D3DXMATRIX scale, translation;

   D3DXMatrixRotationX(&matrixRX, this->xRotation);
   D3DXMatrixRotationY(&matrixRY, this->yRotation);
   D3DXMatrixRotationZ(&matrixRZ, this->zRotation);
   D3DXMatrixScaling(&scale, this->xScale, this->yScale, this->zScale);
   D3DXMatrixTranslation(&translation, this->xTranslation, this->yTranslation, this->zTranslation);
   this->world = matrixRX * matrixRY * matrixRZ;

   // TODO: here!!   
   _message = PrintMatrix(&world);

}

void DirectXScene::RenderFrame()
{
   d3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, DXColors::Black, 1.0f, 0);

   d3dDevice->BeginScene();

   d3dDevice->SetTransform(D3DTS_WORLD, &world);

   d3dDevice->SetStreamSource(0, vertectBuffer, 0, sizeof(CUSTOMVERTEX));
   d3dDevice->SetIndices(indexBuffer);
   d3dDevice->SetFVF(CUSTOMVERTEX::FVF);
   d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 36, 0, 12);

   if (_font) {
      _font->DrawTextA(NULL, _message.c_str(), -1, &_fRect, DT_LEFT, DXColors::White);
   }

   d3dDevice->EndScene();

   d3dDevice->Present(0, 0, 0, 0);
}

void DirectXScene::CalculateFPS(float interval) {
   _count++;
   _elapsedTime += interval;

   if (_elapsedTime > 1.0f) {
      _fps = (float)_count;
      _elapsedTime = 0.0f;
      _count = 0;
      
      std::stringstream ss;
      ss << "Matrix Transform FPS : " <<_fps;
      SetWindowText(_hWnd, ss.str().c_str());
   }
}


void DirectXScene::CleanD3D()
{
   DX::Release(this->vertectBuffer);
   DX::Release(this->indexBuffer);
   DX::Release(this->_font);
}