#pragma once

namespace Utility {
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