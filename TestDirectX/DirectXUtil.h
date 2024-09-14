#pragma once

namespace DXColors {
   D3DCOLOR Black = D3DCOLOR_XRGB(0, 0, 0);
   D3DCOLOR White = D3DCOLOR_XRGB(255, 255, 255);
   D3DCOLOR Red = D3DCOLOR_XRGB( 255, 0, 0);
   D3DCOLOR Green = D3DCOLOR_XRGB(0, 255, 0);
   D3DCOLOR Blue = D3DCOLOR_XRGB(0, 0, 255);
   D3DCOLOR Yellow = D3DCOLOR_XRGB(255, 255, 0);
   D3DCOLOR Purple = D3DCOLOR_XRGB(255, 0, 255);
   D3DCOLOR CornflowerBlue = D3DCOLOR_XRGB(100, 149, 237);
}

namespace DX {
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