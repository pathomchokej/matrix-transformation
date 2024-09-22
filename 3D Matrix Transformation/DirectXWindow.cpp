#include <Windows.h>
#include <sstream>

#include "Window.h"
#include "WindowUtil.h"
#include "DX3D.h"
#include "DirectXWindow.h"
#include "DirectXUtil.h"


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

HRESULT DirectXWindow::InitializeXYZAxis()
{
   CUSTOMVERTEX verticesX[] = {
      { 3.0f, 0.0f, 0.0f, DXColors::Green, }, // 0
      { -3.0f, 0.0f, 0.0f, DXColors::Red, }, // 1
   };

   bool result = _dx3D->CreateLine(verticesX, sizeof(verticesX), &this->vertectBufferXLine);

   //VOID* pVoid; // a void pointer
   //// lock v_buffer and load the vertices into it
   //this->vertectBufferXLine->Lock(0, 0, (void**)&pVoid, 0);
   //memcpy(pVoid, verticesX, sizeof(verticesX));
   //this->vertectBufferXLine->Unlock();


   CUSTOMVERTEX verticesY[] = {
      { 0.0f, 3.0f, 0.0f, DXColors::Green, }, // 0
      { 0.0f, -3.0f, 0.0f, DXColors::Red, }, // 1
   };

   result = _dx3D->CreateLine(verticesY, sizeof(verticesY), &this->vertectBufferYLine);

   //VOID* pVoid; // a void pointer
   //// lock v_buffer and load the vertices into it
   //this->vertectBufferXLine->Lock(0, 0, (void**)&pVoid, 0);
   //memcpy(pVoid, verticesX, sizeof(verticesX));
   //this->vertectBufferXLine->Unlock();

   CUSTOMVERTEX verticesZ[] = {
   { 0.0f, 0.0f, 3.0f, DXColors::Green, }, // 0
   { 0.0f, 0.0f, -3.0f, DXColors::Red, }, // 1
   };

   result = _dx3D->CreateLine(verticesZ, sizeof(verticesZ), &this->vertectBufferZLine);

   return S_OK;
}

HRESULT DirectXWindow::InitializeDXCube()
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

   UINT sizeOfVertex = sizeof(vertices);
   UINT sizeofIndex = sizeof(indices);
   //UINT sizeOfVertex2 = 8 * sizeof(CUSTOMVERTEX);
   //UINT sizeofIndex2 = 36 * sizeof(WORD);
   bool result = _dx3D->CreatePolygon(vertices, sizeof(vertices), indices, sizeof(indices), &this->vertectBuffer, &this->indexBuffer);
   //return (result)? S_OK : E_FAIL;

   // create a vertex buffer interface called v_buffer
   //_dx3D->_d3dDevice->CreateVertexBuffer(
   //   8 * sizeof(CUSTOMVERTEX),
   //   0,
   //   CUSTOMVERTEX::FVF,
   //   D3DPOOL_MANAGED,
   //   &this->vertectBuffer,
   //   NULL
   //);
   //// create indexing 
   //_dx3D->_d3dDevice->CreateIndexBuffer(
   //   36 * sizeof(WORD),
   //   D3DUSAGE_WRITEONLY,
   //   D3DFMT_INDEX16,
   //   D3DPOOL_MANAGED,
   //   &this->indexBuffer,
   //   NULL
   //);

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

void DirectXWindow::MatrixCalculation(float interval)
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
   _dx3D->StartDraw(DXColors::Black, &world);
   if (drawAxis)
   {
      _dx3D->DrawLine(vertectBufferXLine);
      _dx3D->DrawLine(vertectBufferYLine);
      _dx3D->DrawLine(vertectBufferZLine);
   }
   _dx3D->DrawPolygon(vertectBuffer, indexBuffer, 36);
   _dx3D->DrawMessage(_message, 0, 0, 400, 200, DXColors::White);
   _dx3D->EndDraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Base Window

DirectXWindow::DirectXWindow(HINSTANCE hInstance, int width, int height, const char* name) : Window(hInstance, width, height, name)
{

}

bool DirectXWindow::Initialize()
{
   //drawAxis = true;

   xScale = 1.0;
   yScale = 1.0;
   zScale = 1.0;

   _dx3D = new DX3D(GetHandleWindow(), GetWidth(), GetHeight());
   if (!_dx3D->Initialize()) return false;

   if (FAILED(InitializeDXCube())) return false;
   if (FAILED(InitializeXYZAxis())) return false;

   return true;
}


int DirectXWindow::Process(float elapsedTime)
{
   if (_dx3D) _dx3D->Process();

   DXKeyboardProcess();
   MatrixCalculation(elapsedTime);
   DXCalculateFPS(elapsedTime);
   DXRender();
   return 0;
}


void DirectXWindow::Destroy()
{
   if (_dx3D)
   {
      DX::Release(this->vertectBufferXLine);
      DX::Release(this->vertectBufferYLine);
      DX::Release(this->vertectBufferZLine);
      DX::Release(this->vertectBuffer);
      DX::Release(this->indexBuffer);
      _dx3D->Destroy();
   }
}