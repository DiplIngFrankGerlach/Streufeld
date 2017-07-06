#include <stdint.h>

/* this file is an excerpt from https://github.com/FrankGerlach/suchoi_hash.git 
   
   Code in this file can be used in free and in commercial projects free of charge.
*/

//typedef uint32_t (*hashFuncType)(const char*,uint32_t);


// the first digits of PI, use as an s-box (see DES Standard to see what an s-box is)
//  (in short: a highly non-linear function)
const uint32_t c_pi[]=
{
    0x243F6A88,
    0x85A308D3,
    0x3198A2E0,
    0x3707344A,
    0x40938222,
    0x99F31D00,
    0x82EFA98E,
    0xC4E6C894,
    0x52821E63,
    0x8D01377B,
    0xE5466CF3,
    0x4E90C6CC,
    0x0AC29B7C,
    0x97C50DD3,
    0xF84D5B5B,
    0x54709179 };

uint32_t shapashnik(uint32_t input,uint32_t key);
    
/* efficient hash function. See head of file for details 
 * 
 * Explanation of "inner workings":
 * The S-box, as wide as the state, ensures that a single
 * bit change in the input will on average flip half the
 * bits of the output ("total avalanche effect").
 * 
 * Rotating the state will make sure that successive
 * identical input octets will not cancel each other out.
 * 
 * After 16 octets of input, rotation of the state can no
 * longer ensure that identical input octets will not cancel 
 * each other out. Therefore, we apply the shapashnik permutation
 * function on the state.
 * 
 * (For a 64 bit state, self-hashing would be needed after
 *  32 octets of input)
 *
 * Named after Pavel Suchoi, engineer.
 * */
uint32_t suchoi(const char* input, uint32_t input_size)
{
    
    uint32_t state = 0;
    uint32_t i;
    for(i=0; i < input_size; i++)
    {
        char octet = input[i];// ^ (uint8_t)state;
        uint8_t upperNibble = octet >>4;
        uint8_t lowerNibble = octet & 0xF;
        state ^= c_pi[upperNibble];
        state = (state << 31) | (state >> 1);//rotate state
        state ^= c_pi[lowerNibble];
        state = (state << 31) | (state >> 1);//rotate state
        if( (i & 0xf) == 0xf )//danger of "xor-cancellation" -> permute the state
        {
            state = shapashnik(state,state);             
        }
    }
    return state;
}

void int2octets(uint32_t input,uint8_t* output)
{
   uint8_t i=0;
   for(i=0; i < 4;i++)
   {
       output[i] = input & 0xFF;
       input >>= 8;
   }
}

uint32_t octets2int(uint8_t* input)
{
   uint32_t output = 0;
   int8_t i;
   for(i=3; i > 0 ;i--)
   {
       output |= input[i];
       output <<= 8;
   }
   output |= input[i];
   return output;
}

/*
void printBin(uint32_t input)
{
   int32_t i;
   for(i=31; i >=0; i--)
   {
      if( input & (1 << i) )
      {
        printf("1");
      }
      else
      {
        printf("0");
      }
      if( i && ((i & 0x7) == 0) )
      {
         printf(".");
      }
   }
   printf("\n");
}*/


uint32_t rotate(uint32_t input,uint8_t count)
{
   return (input << count) | (input >> (32 - count));
}

uint32_t  swapBits(uint32_t input, uint8_t pos1, uint8_t pos2)
{
   uint32_t stencil1 = ( 1 << pos1 );
   uint32_t stencil2 = ( 1 << pos2 );
   uint32_t v1 = input & stencil1;
   uint32_t v2 = input & stencil2;
   uint32_t output = input;
   output &= (~stencil1);
   output &= (~stencil2);
   if( v2 )
   {
     output |= stencil1;
   }
   if( v1 )
   {
     output |= stencil2;
   }
   return output;
}

uint32_t shapashnik_lp(uint32_t input,uint32_t key)
{
  uint32_t output = 0;
  uint8_t i;
  for(i=0; i < 32;i++)
  {
    uint8_t index = key & 0x1F;
    key = rotate(key,5);
  
    swapBits(input,i,index); 
  }
  return output;      
} 

/*high performance permutation/diffusion function 

 Named after Barys Shapashnik, engineer. 
*/
uint32_t shapashnik(uint32_t input,uint32_t key)
{
   //printf("shapashnik i:%x\n",input);
   uint8_t i;
   uint32_t output;
   for(i=0; i < 10;i++)
   {
     uint32_t upper = input >> 16;
     uint32_t lower = input & 0xFFFF;
     output = (lower << 16) | upper;
     output = rotate(output,key & 0x7);
     key = rotate(key,3); 
     input = output;
   }
   //printf("shapashnik o:%x\n",output);
   return output;
}




