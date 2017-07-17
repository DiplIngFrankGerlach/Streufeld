
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

bool istZiffer(char z)
{
   switch(z)
   {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':return true;
      default: return false;
   }
   return false;
}


bool leseZahl(Zeichenkette& zk,uint64_t& zahl)
{
    zahl = 0;
    if( zk.laenge() == 0)
    {
       return false;
    }
    char zeichen;
    uint8_t stelle(0);
    do
    {
       zeichen = zk[stelle++];
       if( istZiffer(zeichen) )
       {
          zahl += (zeichen - '0');
       }
       else return false;
       if( stelle < zk.laenge() )
       {
           zahl *= 10;
       }
       else break;
    }
    while( true );
    return true;
}
