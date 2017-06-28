/* Pruefprogramm fuer Streufeld-Klasse */




#include <assert.h>
#include <iostream>
#include "Streufeld.h"

using namespace std;


class SchlAdapterInt
{
public:
   static bool gleich(const uint64_t a,const uint64_t b)
   {
     return a == b;
   }

   static uint64_t randomisiere(const uint64_t schluessel)
   {
      return schluessel*98487831+83847871;
   }
   static void loesche(uint64_t& schluessel)
   {
      schluessel = 0;
   }
};

class WertAdapterInt
{
public:
  static void loesche(uint64_t& wert)
  {
      wert = 0;
  }
  
};


int main(int argc,char** argv)
{
   bool erfolg=false;
   Streufeld<uint64_t,uint64_t,SchlAdapterInt,WertAdapterInt> sf(0,erfolg);

   for(uint64_t i=0; i < 100; i++)
   {
      assert( sf.trageEin(i,i+1000) );
   }
   for(int64_t i=99; i >=0; i--)
   {
      uint64_t wert;
      assert( sf.finde(i,wert) );
      assert( wert == (i+1000) );
   }
   cout << "Pruefung der Streufeld-Klasse erfolgreich" << endl;
}
