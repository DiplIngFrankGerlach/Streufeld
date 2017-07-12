/* Pruefprogramm fuer StreufeldKette-Klasse */




#include <assert.h>
#include <iostream>
#include <stdint.h>
#include "StreufeldKette.h"
#include "HashAdapterZK.h"

using namespace std;



int main(int argc,char** argv)
{
   bool erfolg=false;
   StreufeldKette<Zeichenkette,Zeichenkette,SchlAdapterZK,WertAdapterZK> sf;

    
   Zeichenkette schluessel(1,erfolg);
   Zeichenkette wert(1,erfolg);

   schluessel = "AnfangderZeichenkette_";
   schluessel.dazuZahl(0);

   assert( schluessel == "AnfangderZeichenkette_0");

   schluessel.dazuZahl(0);
   assert( schluessel == "AnfangderZeichenkette_00");

   schluessel.dazuZahl(17771);
   assert( schluessel == "AnfangderZeichenkette_0017771");

   const uint64_t anzahlPruefungen = 1000000;
     
   for(uint64_t i=0; i < anzahlPruefungen; i++)
   {
      schluessel = "AnfangderZeichenkette_";
      schluessel.dazuZahl(i);
      wert       = "Wert_";
      wert.dazuZahl(i+1000);
      
      sf.trageEin(schluessel,wert); 
   }
   assert(sf.anzahl() == anzahlPruefungen);

   for(int64_t i=anzahlPruefungen-1; i >=0; i--)
   {
      schluessel = "AnfangderZeichenkette_";
      schluessel.dazuZahl(i);
      wert       = "Wert_";
      wert.dazuZahl(i+1000);

      Zeichenkette gefunden;
      assert( sf.finde(schluessel,gefunden) );
      assert( wert == gefunden );
   }
   assert(sf.anzahl() == anzahlPruefungen);
 
   
   uint64_t loeschZaehler(0);
   for(int64_t i=anzahlPruefungen-1; i >=0; i--)
   {
      if( (i % 3) == 0)
      {
         schluessel = "AnfangderZeichenkette_";
         schluessel.dazuZahl(i);
         wert       = "Wert_";
         wert.dazuZahl(i+1000);

         assert( sf.loesche(schluessel) );
         loeschZaehler++;
      }   
   }
   assert(sf.anzahl() == (anzahlPruefungen-loeschZaehler) );


   for(int64_t i=anzahlPruefungen-1; i >=0; i--)
   {
      if( (i % 3) != 0)
      {
         schluessel = "AnfangderZeichenkette_";
         schluessel.dazuZahl(i);
         wert       = "Wert_";
         wert.dazuZahl(i+1000);

         Zeichenkette gefunden;
         assert( sf.finde(schluessel,gefunden) );
         assert( wert == gefunden );
      }   
   }
   
   cout << "Pruefung der Streufeld-Klasse erfolgreich" << endl;
}
