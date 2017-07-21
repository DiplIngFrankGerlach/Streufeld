#ifndef NUETZLICH
#define NUETZLICH

#include <stdint.h>
#include "zk.h"

uint64_t naechstGroessereZweierPotenz(uint64_t zahl);

bool leseZahl(Zeichenkette& zk,uint64_t& zahl);

bool istNormalZeichen(char zeichen);

bool istBst(char zeichen);

bool istKopfzeilenWertZeichen(char zeichen);

bool istHostBst(char z);

bool istZiffer(char z);

#endif
