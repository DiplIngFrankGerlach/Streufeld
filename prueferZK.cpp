/* Pruefprogramm fuer Streufeld-Klasse */




#include <assert.h>
#include <iostream>
#include "Streufeld.h"
#include "HashAdapterZK.h"

using namespace std;



int main(int argc,char** argv)
{
   bool erfolg=false;
   Streufeld<Zeichenkette,Zeichenkette,SchlAdapterZK,WertAdapterZK> sf(10,erfolg);

    
   Zeichenkette schluessel(1,erfolg);
   Zeichenkette wert(1,erfolg);

   schluessel = "AnfangderZeichenkette_";
   schluessel.dazuZahl(0);

   assert( schluessel == "AnfangderZeichenkette_0");

   schluessel.dazuZahl(0);
   assert( schluessel == "AnfangderZeichenkette_00");

   schluessel.dazuZahl(17771);
   assert( schluessel == "AnfangderZeichenkette_0017771");

   schluessel.dazuZahl(0xFAAD,16);
   assert( schluessel == "AnfangderZeichenkette_0017771FAAD");

   schluessel.leere(true);
   schluessel.dazuZahl(0xFAAD,16);
   assert( schluessel == "FAAD");

   schluessel.leere(true);
   schluessel.dazuZahl(1000,8);
   assert( schluessel == "1750");

   schluessel.leere(true);
   schluessel.dazuZahl(1024,2);
   assert( schluessel == "10000000000");

   //Zahl zur Basis 26 
   schluessel.leere(true);
   schluessel.dazuZahl(13*26*26+13*26+18,26);
   assert( schluessel == "DDI");


   
   for(uint64_t i=0; i < 100; i++)
   {
      schluessel = "AnfangderZeichenkette_";
      schluessel.dazuZahl(1000+i,10);

      wert = "wiesel";
      wert.dazuZahl(1500+i,10);
      
      assert( sf.trageEin(schluessel,wert) );
   }
   for(int64_t i=99; i >=0; i--)
   {
      schluessel = "AnfangderZeichenkette_";
      schluessel.dazuZahl(1000+i,10);

      wert = "wiesel";
      wert.dazuZahl(1500+i,10);
  
      Zeichenkette wertGefunden;
      assert( sf.finde(schluessel,wertGefunden) );
      assert( wert == wertGefunden );
   }
  
   cout << "Pruefung der Streufeld-Klasse erfolgreich" << endl;
}
