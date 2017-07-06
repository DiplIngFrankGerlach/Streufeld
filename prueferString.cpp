/* Pruefprogramm fuer Streufeld-Klasse */




#include <assert.h>
#include <iostream>
#include <std_int.h>
#include "Streufeld.h"
#include "HashAdapterZK.h"

using namespace std;



int main(int argc,char** argv)
{
   bool erfolg=false;
   Streufeld<Zeichenkette,Zeichenkette,SchlAdapterZK,WertAdapterZK> sf(0,erfolg);

   bool erfolg;
   Zeichenkette schluessel(1,erfolg);
   Zeichenkette wert(1,erfolg);

   schluessel = "AnfangderZeichenkette_";
   schluessel.dazu(0);

   assert( schluessel == "AnfangderZeichenkette_0");

   schluessel.dazu(0);
   assert( schluessel == "AnfangderZeichenkette_00");

   schluessel.dazu(17771);
   assert( schluessel == "AnfangderZeichenkette_00177771");


   /*  
   for(uint64_t i=0; i < 100; i++)
   {
      schluessel = "AnfangderZeichenkette_";
      
      assert( sf.trageEin(i,i+1000) );
   }
   for(int64_t i=99; i >=0; i--)
   {
      uint64_t wert;
      assert( sf.finde(i,wert) );
      assert( wert == (i+1000) );
   }
   */
   cout << "Pruefung der Streufeld-Klasse erfolgreich" << endl;
}
