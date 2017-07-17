#ifndef HASH_ADAPTER_ZK
#define HASH_ADAPTER_ZK

#include "zk.h"
#include "SuchoiHash.h"

/* Adapter-Klasse fuer Schluessel vom Typ Zeichenkette */
class SchlAdapterZK
{
public:
   static bool gleich(const Zeichenkette& a,const Zeichenkette& b)
   {
      return a == b;
   }

   static uint64_t randomisiere(const Zeichenkette& schluessel)
   {
      return suchoi(schluessel.zk(),schluessel.laenge());
   }
   static void loesche(Zeichenkette& schluessel )
   {
      schluessel.leere(true);
   }

   static void loescheEndgueltig(Zeichenkette& schluessel)
   {
      schluessel.leere(true);
   }
};

/* Adapter-Klasse fuer Wert-Objekte vom Typ Zeichenkette */
class WertAdapterZK
{
public:
  static void loesche(Zeichenkette& wert)
  {
      wert.leere(true);
  }

  static void loescheEndgueltig(Zeichenkette& wert)
  {
      wert.leere(true);
  }
};


typedef Streufeld<Zeichenkette,Zeichenkette,SchlAdapterZK,WertAdapterZK> SFzkzk;

#endif
