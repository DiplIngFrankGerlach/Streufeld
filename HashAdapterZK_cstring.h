#ifndef HASH_ADAPTER_ZK_CSTRING
#define HASH_ADAPTER_ZK_CSTRING

#include <string.h>
#include "zk.h"
#include "SuchoiHash.h"

/* Adapter-Klasse fuer Schluessel vom Typ "C Zeichenkette" */
class SchlAdapterZK_cstring
{
public:
   static bool gleich(const char* a,const char*  b)
   {
      return strcmp(a,b) == 0;
   }

   static uint64_t randomisiere(const char* schluessel)
   {
      return suchoi(schluessel,strlen(schluessel));
   }
   static void loesche(char*& schluessel )
   {
      schluessel = NULL;
   }

  static void loescheEndgueltig(char*& schluessel)
  {
      delete[] schluessel;
      schluessel = NULL;
  }
};

/* Adapter-Klasse fuer Wert-Objekte vom Typ Zeichenkette */
class WertAdapterZK_cstring
{
public:
  static void loesche(char*& wert)
  {
      wert = NULL;
  }

  static void loescheEndgueltig(char*& wert)
  {
      delete[] wert;
      wert = NULL;
  }
};

#endif
