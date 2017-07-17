#ifndef HASH_ADAPTER
#define HASH_ADAPTER

/* Adapter fuer verschiedene Schluessel vom Typ Ganzzahl
 
   (C) Frank Gerlach 2017

   Nichtkommerzielle Nutzung ist lizenz- und kostenfrei.

   Kommerzielle Nutzung erfordert eine kostenpflichtige Lizenz.
   Nutzungsplaene fangen an ab 150 Euro pro Produkt.
 
   Einzelheiten bitte erfragen bei frankgerlach.tai@gmx.de

*/

/* Adapter-Klasse fuer Schluessel vom Typ Ganzzahl */
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


/* Adapter-Klasse fuer Wert-Objekte vom Typ uint64_t */
class WertAdapterInt
{
public:
  static void loesche(const uint64_t& wert)
  {
      wert = 0;
  }

  static void loescheEndgueltig(const uint64_t& wert)
  {
      wert = 0;
  }
};

#endif
