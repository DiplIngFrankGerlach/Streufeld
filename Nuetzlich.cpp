
#include "Nuetzlich.h"

uint64_t naechstGroessereZweierPotenz(uint64_t zahl)
{
  uint8_t anzBits(0);
  uint8_t maxBit(0);
  while( zahl > 0 )
  {
     if( zahl & 1 )
     {
        anzBits++;
     }
     maxBit++;
     zahl >>= 1;
  }
  switch( anzBits )
  {
      case 0: return 0;
      case 1: return 1 << (maxBit-1);
      default:  return 1 << maxBit;
  }
}
