#include <Windows.h>
#include <sstream>

#include "Window.h"
#include "WindowUtil.h"
#include "DirectXWindow.h"
#include "DirectXUtil.h"

struct CUSTOMVERTEX
{
   float X, Y, Z;
   DWORD COLOR;
   static const DWORD FVF;
};
const DWORD CUSTOMVERTEX::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;


std::string DirectXWindow::PrintMatrix(D3DXMATRIX* matrix) {
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

std::string DirectXWindow::PrintTranslation() {
   std::string result;

   result = "Translation   X[" + std::format("{:.2f}", xTranslation) + "] ";
   result += "  Y[" + std::format("{:.2f}", yTranslation) + "] ";
   result += "  Z[" + std::format("{:.2f}", zTranslation) + "] \n";

   return result;
}

std::string DirectXWindow::PrintRotation()
{
   std::string result;

   result = "Rotation      X[" + std::format("{:.2f}", xRotation) + "] ";
   result += "  Y[" + std::format("{:.2f}", yRotation) + "] ";
   result += "  Z[" + std::format("{:.2f}", zRotation) + "] \n";

   return result;
}

std::string DirectXWindow::PrintScale()
{
   std::string result;

   result = "Scale          X[" + std::format("{:.2f}", xScale) + "] ";
   result += "  Y[" + std::format("{:.2f}", yScale) + "] ";
   result += "  Z[" + std::format("{:.2f}", zScale) + "] \n";

   return result;
}

HRESULT DirectXWindow::InitializeDX()
{
   this->d3d = Direct3DCreate9(D3D_SDK_VERSION);
   if (this->d3d == NULL)
   {
      MessageBox(GetHandleWindow(), "Direct3DCreate9 failed", NULL, NULL);
      return E_FAIL;
   }

   return S_OK;
}


HRESULT DirectXWindow::InitializeDXDevice()
{
   D3DCAPS9 d3dDeviceCaps;
   if (FAILED(this->d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dDeviceCaps)))
   {
      MessageBox(GetHandleWindow(), "GetDeviceCaps failed", NULL, NULL);
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
   d3dpp.BackBufferWidth = GetWidth();
   d3dpp.BackBufferHeight = GetHeight();
   d3dpp.Windowed = TRUE;
   d3dpp.BackBufferCount = 1;
   d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
   d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
   d3dpp.MultiSampleQuality = 0;
   d3dpp.hDeviceWindow = GetHandleWindow();
   d3dpp.Flags = 0;
   d3dpp.EnableAutoDepthStencil = true;
   d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
   d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
   d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
   d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

   if (FAILED(this->d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetHandleWindow(),
      vp,
      &d3dpp, &this->d3dDevice))) {
      MessageBox(GetHandleWindow(), "CreateDevice failed", NULL, NULL);
      return E_FAIL;
   }

   if (this->d3dDevice == NULL) {
      MessageBox(GetHandleWindow(), "d3dDevice is null", NULL, NULL);
      return E_FAIL;
   }

   D3DVIEWPORT9 viewPort;
   ZeroMemory(&viewPort, sizeof(D3DVIEWPORT9));
   viewPort.X = 0;
   viewPort.Y = 0;
   viewPort.Width = GetWidth();
   viewPort.Height = GetHeight();
   viewPort.MinZ = 0;
   viewPort.MaxZ = 1;

   d3dDevice->SetViewport(&viewPort);

   return S_OK;
}


HRESULT DirectXWindow::InitializeDXFont()
{
   // Create font
   if (FAILED(D3DXCreateFont(this->d3dDevice, 20, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
      FF_DONTCARE, "Arial", &_font))) {
      MessageBox(GetHandleWindow(), "D3DXCreateFont failed", NULL, NULL);
      return E_FAIL;
   }

   SetRect(&_fRect, 0, 0, 400, 200);
   _message = "| 1, 1, 1, 1 |\n";
   _message += "| 1, 1, 1, 1 |\n";
   _message += "| 1, 1, 1, 1 |\n";
   _message += "| 1, 1, 1, 1 |\n";

   return S_OK;
}


HRESULT DirectXWindow::InitializeDXCube()
{
   xScale = 1.0;
   yScale = 1.0;
   zScale = 1.0;

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

   return S_OK;
}

HRESULT DirectXWindow::InitializeDXCamera() {
   D3DXMATRIX view;
   D3DXMATRIX proj;

   D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
   D3DXVECTOR3 target = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
   D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

   D3DXMatrixLookAtLH(&view, &position, &target, &up);
   this->d3dDevice->SetTransform(D3DTS_VIEW, &view);

   D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, static_cast<float>(GetWidth()) / GetHeight(), 1.0f, 1000.0f);
   this->d3dDevice->SetTransform(D3DTS_PROJECTION, &proj);

   this->d3dDevice->SetRenderState(D3DRS_LIGHTING, false);
   this->d3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

   return S_OK;
}

HRESULT DirectXWindow::InitializeDXKeyboard()
{
   // Step 1: Create the DirectInput object
   if (FAILED(DirectInput8Create(GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL))) {
      return E_FAIL;
   }

   // Step 2: Create a device for the keyboard
   if (FAILED(dInput->CreateDevice(GUID_SysKeyboard, &dKeyboard, NULL))) {
      return E_FAIL;
   }

   // Step 3: Set the data format (we want the keyboard data format)
   if (FAILED(dKeyboard->SetDataFormat(&c_dfDIKeyboard))) {
      return E_FAIL;
   }

   // Step 4: Set the cooperative level (we'll set it to exclusive and foreground mode)
   if (FAILED(dKeyboard->SetCooperativeLevel(GetHandleWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
      return E_FAIL;
   }

   // Step 5: Acquire the keyboard
   dKeyboard->Acquire();

   return S_OK;
}


void DirectXWindow::DXKeyboardProcess()
{
   float translateSize = 0.001;
   if (IsKeyPressed(DIK_Q)) {
      xTranslation += translateSize;
   }
   if (IsKeyPressed(DIK_A)) {
      yTranslation += translateSize;
   }
   if (IsKeyPressed(DIK_Z)) {
      zTranslation += translateSize;
   }
   if (IsKeyPressed(DIK_W)) {
      xTranslation -= translateSize;
   }
   if (IsKeyPressed(DIK_S)) {
      yTranslation -= translateSize;
   }
   if (IsKeyPressed(DIK_X)) {
      zTranslation -= translateSize;
   }

   float rotationSize = 0.001;
   if (IsKeyPressed(DIK_E)) {
      xRotation += rotationSize;
   }
   if (IsKeyPressed(DIK_D)) {
      yRotation += rotationSize;
   }
   if (IsKeyPressed(DIK_C)) {
      zRotation += rotationSize;
   }
   if (IsKeyPressed(DIK_R)) {
      xRotation -= rotationSize;
   }
   if (IsKeyPressed(DIK_F)) {
      yRotation -= rotationSize;
   }
   if (IsKeyPressed(DIK_V)) {
      zRotation -= rotationSize;
   }


   float scaleSize = 0.001;
   if (IsKeyPressed(DIK_T)) {
      xScale += scaleSize;
   }
   if (IsKeyPressed(DIK_G)) {
      yScale += scaleSize;
   }
   if (IsKeyPressed(DIK_B)) {
      zScale += scaleSize;
   }
   if (IsKeyPressed(DIK_Y)) {
      xScale -= scaleSize;
   }
   if (IsKeyPressed(DIK_H)) {
      yScale -= scaleSize;
   }
   if (IsKeyPressed(DIK_N)) {
      zScale -= scaleSize;
   }

   if (IsKeyPressed(DIK_1)) {
      xScale = 1.0;
      yScale = 1.0;
      zScale = 1.0;
      xTranslation = 0.0;
      yTranslation = 0.0;
      zTranslation = 0.0;
      xRotation = 0.0;
      yRotation = 0.0;
      zRotation = 0.0;
   }
   if (IsKeyPressed(DIK_2)) {
      autoRotation = true;
   }
   if (IsKeyPressed(DIK_3)) {
      autoRotation = false;
   }
}

void DirectXWindow::DXProcess(float interval)
{
   if (autoRotation)
   {
      this->xRotation += interval;
      this->yRotation += interval;
      this->zRotation += interval;
   }

   D3DXMATRIX matrixRX, matrixRY, matrixRZ;
   D3DXMATRIX scale, translation;

   D3DXMatrixRotationX(&matrixRX, this->xRotation);
   D3DXMatrixRotationY(&matrixRY, this->yRotation);
   D3DXMatrixRotationZ(&matrixRZ, this->zRotation);
   D3DXMatrixScaling(&scale, this->xScale, this->yScale, this->zScale);
   D3DXMatrixTranslation(&translation, this->xTranslation, this->yTranslation, this->zTranslation);
   this->world = matrixRX * matrixRY * matrixRZ * translation * scale;


   _message = PrintMatrix(&world);
   _message += "\n\n";
   _message += PrintTranslation();
   _message += PrintRotation();
   _message += PrintScale();

}

void DirectXWindow::DXCalculateFPS(float interval)
{
   _count++;
   _elapsedTime += interval;

   if (_elapsedTime > 1.0f) {
      _fps = (float)_count;
      _elapsedTime = 0.0f;
      _count = 0;

      std::stringstream ss;
      ss << "Matrix Transform FPS : " << _fps;
      SetWindowText(GetHandleWindow(), ss.str().c_str());
   }
}

void DirectXWindow::DXRender()
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

void DirectXWindow::DXPollKeyboard() {
   HRESULT hr = dKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

   // If the device lost focus or is not acquired, try to re-acquire it
   if (FAILED(hr)) {
      if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) {
         dKeyboard->Acquire();
      }
   }
}

bool DirectXWindow::IsKeyPressed(BYTE key) {
   // Check if the key is pressed in the state buffer
   return (keyboardState[key] & 0x80) != 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Base Window

DirectXWindow::DirectXWindow(HINSTANCE hInstance, int width, int height, const char* name) : Window(hInstance, width, height, name)
{

}

bool DirectXWindow::Initialize()
{
   if (FAILED(InitializeDX())) return false;
   if (FAILED(InitializeDXDevice())) return false;
   if (FAILED(InitializeDXFont())) return false;
   if (FAILED(InitializeDXCamera())) return false;
   if (FAILED(InitializeDXCube())) return false;
   if (FAILED(InitializeDXKeyboard())) return false;

   return true;
}


int DirectXWindow::Process(float elapsedTime)
{
   DXPollKeyboard();
   DXKeyboardProcess();
   DXProcess(elapsedTime);
   DXCalculateFPS(elapsedTime);
   DXRender();
   return 0;
}


void DirectXWindow::Destroy()
{
   DX::ReleaseUnacquire(this->dKeyboard);
   DX::Release(this->dInput);

   DX::Release(this->vertectBuffer);
   DX::Release(this->indexBuffer);
   DX::Release(this->_font);
}