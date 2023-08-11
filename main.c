#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>
#include "param.h"
#include "present.h"


int main()
{   
    init();
    uint8_t msg[]={255,255,255,255,255,255,255,255};
    uint8_t key[]={0,0,0,0,0,0,0,0,0,0};
    encrypt(msg,key);
    printHex(state,PRESENT_CRYPT_SIZE);
    printf("Clé de déchiffrement\n");
    printHex(register_key,PRESENT_KEY_SIZE);
    decrypt(state,register_key);
    printHex(state,PRESENT_CRYPT_SIZE);
    printf("Clé de chiffrement\n");
    printHex(register_key,PRESENT_KEY_SIZE);
    return 0;

}
