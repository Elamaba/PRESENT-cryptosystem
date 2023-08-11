#include <stdio.h>
#include <stdlib.h>  
#include <time.h>    
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "param.h"
#include "present.h"


uint8_t * state;
uint8_t * register_key;
uint8_t *round_key;
op_mode mode;


void init()
{
  state=(uint8_t *)calloc(PRESENT_CRYPT_SIZE,sizeof(uint8_t));
  round_key=(uint8_t *)calloc(PRESENT_CRYPT_SIZE,sizeof(uint8_t));
  register_key=(uint8_t *)calloc(PRESENT_KEY_SIZE,sizeof(uint8_t));
}


void keygen()
{
  FILE *fp;
  for(int i=0;i<PRESENT_KEY_SIZE;i++)
  {
     fp = fopen("/dev/random", "rb");
     register_key[i] = fgetc(fp);
  }
}


void sub_key()
{
  for (int i=0;i<PRESENT_CRYPT_SIZE;i++) round_key[i] =register_key[i+2];
}


void add_round_key()
{

  for(int i=0;i<PRESENT_CRYPT_SIZE;i++) state[i] ^= round_key[i];

}


uint8_t two_sbox(uint8_t input)
{
  
  uint8_t tmp=0;
  uint8_t output=0;
  
  output=sbox[input & 0x0F]; 
  tmp=sbox[(input>>4) & 0x0F];  
  output |= (tmp<<4) & 0xF0; 
    
  return output;
}


void sBoxLayer()
{
  
  for(int i=0;i<PRESENT_CRYPT_SIZE;i++) state[i] = two_sbox(state[i]);

}


void pLayer(op_mode mode)
{
    uint8_t bit = 0 ,position = 0;
    uint8_t inverse_permutation_state[PRESENT_CRYPT_SIZE]={0}; 
    int N=(mode==ENCRYPT_MODE)?16:4;
    for(int i=0; i < 63; i++)
    {
        bit = (state [i/ 8] >> (i % 8)) & 1;
        position = (i * N) % 63;
        inverse_permutation_state[position / 8] |= bit << (position % 8);
    }
    inverse_permutation_state[7] |= state[7]&0x80;
    memcpy(state, inverse_permutation_state,PRESENT_CRYPT_SIZE );

}

void right_circle_shift()
{
    uint8_t bit = 0 ,position = 0;
    uint8_t permutation_state[PRESENT_KEY_SIZE]={0};
        for(int i=0; i < 80; i++)
        {
            bit = (register_key[i/8]>>(i%8))&1;
            position = (i + 19) % 80;
            permutation_state[position/ 8] |= bit << (position % 8);
        }
        
        memcpy(register_key, permutation_state,PRESENT_KEY_SIZE );
}


void left_circle_shift()
{
    uint8_t bit = 0 ,position = 0;
    uint8_t permutation_state[PRESENT_KEY_SIZE]={0};
        for(int i=0; i < 80; i++)
        {
            bit = (register_key[i/8]>>(i%8))&1;
            position = (i + 61) % 80;
            permutation_state[position/ 8] |= bit << (position % 8);
        }
        memcpy(register_key, permutation_state,PRESENT_KEY_SIZE );
}

void simple_sbox(uint8_t input)
{
   if(mode==ENCRYPT_MODE)
   {
       uint8_t tmp=sbox[input&0x0F];
       input &= 0xF0;
       input |= tmp;
   }
   else
   {
       uint8_t tmp=sbox_inv[input&0x0F];
       input &= 0xF0;
       input |= tmp;
   }

}
void update(uint8_t round_counter, op_mode mode)
{
    uint8_t five_bits=0;
    uint8_t tmp=0;
    if (mode==ENCRYPT_MODE)
    {
	left_circle_shift();
        tmp=sbox[(register_key[9]>>4)&0x0F];
        register_key[9] = (register_key[9] & 0x0F)|((tmp<<4)& 0xF0);
	    five_bits = (register_key[1] >>7)&1; // Récupération de k15
	    five_bits |= (register_key[2]&0x0F)<<1; // Récupération de k16k17k18k19
	    five_bits ^= round_counter; 
	    register_key[1] =(register_key[1] & 0x7F) | (five_bits & 0x01)<<7;
	    register_key[2] = (register_key[2] & 0xF0) | ((five_bits>>1) & 0x0F);
    }
    
    else
    {
        five_bits = (register_key[1] >>7)&1; // Récupération de k15
	    five_bits |= (register_key[2]&0x0F)<<1; // Récupération de k16k17k18k19
	    five_bits ^= round_counter; 
	    register_key[1] =(register_key[1] & 0x7F) | (five_bits & 0x01)<<7;
	    register_key[2] = (register_key[2] & 0xF0) | ((five_bits>>1) & 0x0F);
        tmp=sbox_inv[(register_key[9]>>4)&0x0F];
        register_key[9] = (register_key[9] & 0x0F)|((tmp<<4)& 0xF0);
        right_circle_shift();
    }
}

uint8_t two_sbox_inv(uint8_t input)
{
	uint8_t tmp=0;
  	uint8_t output=0;  
  	output=sbox_inv[input&15]; 
  	tmp=sbox_inv[input>>4];  
  	output = (tmp<<4)|output; 
    
  return output;

}

void sBoxLayer_inv()
{
	for(int i=0;i<PRESENT_CRYPT_SIZE;i++)
     		state[i] = two_sbox_inv(state[i]);

}

void print_byte(uint8_t * bytes, int size)
{
    int i=0;
    printf("\n");
    for(i=size-1; i >= 0; i--)
        printf("%d ", bytes[i]);
    printf("\n");
}
void printHex(uint8_t * bytes, int size)
{
    int i;
    for(i=size-1; i >= 0; i--)
    {
        if (i < size-1 ) printf(":");
        printf("%02X", bytes[i]);
    }
    printf("\n");
}
void toBinary(uint8_t a)
{
    uint8_t i;

    for(i=0x80;i!=0;i>>=1) {
        printf("%c",(a&i)?'1':'0');
        if (i==0x10)
            printf(" ");
    }
}

void printBinary(uint8_t * bytes, int size)
{
    int i;
    for (i=size-1; i>=0 ; i--)
    {
        toBinary(bytes[i]);
        printf(" ");
    }
    printf("\n");
}
 
void encrypt(uint8_t * msg, uint8_t const * key)
{
    memcpy(register_key, key,PRESENT_KEY_SIZE );
    memcpy(state, msg,PRESENT_CRYPT_SIZE );
    for (uint8_t round_counter=1;round_counter<=PRESENT_ROUND_COUNT;round_counter++)
    {
	    sub_key();
	    add_round_key();
	    sBoxLayer();
	    pLayer(ENCRYPT_MODE);
	    update(round_counter, ENCRYPT_MODE);
    }
    sub_key();
    add_round_key();

}

void decrypt(uint8_t * msg, uint8_t const * key)
{
    memcpy(register_key, key,PRESENT_KEY_SIZE );
    memcpy(state, msg,PRESENT_CRYPT_SIZE );
    sub_key();
    add_round_key();
    for (uint8_t round_counter=PRESENT_ROUND_COUNT;round_counter>=1;round_counter--)
    {
        update(round_counter,DECRYPT_MODE);
        pLayer(DECRYPT_MODE);
        sBoxLayer_inv();
	    sub_key();
        add_round_key();	
    }
    
}

