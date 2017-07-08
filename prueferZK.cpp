/* Pruefprogramm fuer Streufeld-Klasse */




#include <assert.h>
#include <iostream>
#include "Streufeld.h"
#include "HashAdapterZK.h"
#include "HashAdapterZK_cstring.h"
#include "Nuetzlich.h"

using namespace std;



int main(int argc,char** argv)
{
   assert ( naechstGroessereZweierPotenz(0) == 0 );
   assert ( naechstGroessereZweierPotenz(13) == 16 );

   assert ( naechstGroessereZweierPotenz(2) == 2 );
   assert ( naechstGroessereZweierPotenz(1) == 1 );
   assert ( naechstGroessereZweierPotenz(4) == 4 );
   assert ( naechstGroessereZweierPotenz(5) == 8 );

   


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


   const uint64_t c_anzahlEintrage = 100;
   
   for(uint64_t i=0; i < c_anzahlEintrage; i++)
   {
      schluessel = "AnfangderZeichenkette_";
      schluessel.dazuZahl(1000+i,10);

      wert = "wiesel";
      wert.dazuZahl(1500+i,10);
      
      assert( sf.trageEin(schluessel,wert) );
   }
   for(int64_t i=c_anzahlEintrage-1; i >=0; i--)
   {
      schluessel = "AnfangderZeichenkette_";
      schluessel.dazuZahl(1000+i,10);

      wert = "wiesel";
      wert.dazuZahl(1500+i,10);
  
      Zeichenkette wertGefunden;
      assert( sf.finde(schluessel,wertGefunden) );
      assert( wert == wertGefunden );
   }
 
    uint64_t anzGeloescht(0);
    for(int64_t i=c_anzahlEintrage-1; i >=0; i--)
    {
      if( (i & 1) )
      {
         anzGeloescht++;
         schluessel = "AnfangderZeichenkette_";
         schluessel.dazuZahl(1000+i,10);

         assert( sf.loesche(schluessel) );
      }      
   }
   assert( sf.anzahl() == c_anzahlEintrage-anzGeloescht);


   schluessel = "AnfangderZeichenkette_1064";
   Zeichenkette wertGefunden;
   assert( sf.finde(schluessel,wertGefunden) );

   for(int64_t i=c_anzahlEintrage-1; i >=0; i--)
   {
      if( (i & 1) == 0)
      {
         schluessel = "AnfangderZeichenkette_";
         schluessel.dazuZahl(1000+i,10);

         wert = "wiesel";
         wert.dazuZahl(1500+i,10);


         Zeichenkette wertGefunden;
         assert( sf.finde(schluessel,wertGefunden) );
         assert( wert == wertGefunden );
      }
   }

   
   //Pruefung C Strings
   Streufeld<char*,char*,SchlAdapterZK_cstring,SchlAdapterZK_cstring> sfcs(1,erfolg);
   for(uint64_t i=0; i < c_anzahlEintrage; i++)
   {
      schluessel = "AnfangderZeichenkette_";
      schluessel.dazuZahl(1000+i,10);

      wert = "wiesel";
      wert.dazuZahl(1500+i,10);

      char* schlCS = new char[schluessel.laenge()+1];
      strcpy(schlCS,schluessel.zkNT());

      char* wertCS = new char[wert.laenge()+1];
      strcpy(wertCS,wert.zkNT());
      
      assert( sfcs.trageEin(schlCS,wertCS) );
   }
   for(int64_t i=c_anzahlEintrage-1; i >=0; i--)
   {
      schluessel = "AnfangderZeichenkette_";
      schluessel.dazuZahl(1000+i,10);

      wert = "wiesel";
      wert.dazuZahl(1500+i,10);
  
      char* wertGefunden(NULL);
      assert( sfcs.finde( (char*)schluessel.zkNT(),wertGefunden) );
      assert( wert == wertGefunden );
   }
   

   cout << "Pruefung der Streufeld-Klasse erfolgreich" << endl;
}
