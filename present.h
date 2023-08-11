#ifndef PRESENT_H_INCLUDED
#define PRESENT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef enum {
    ENCRYPT_MODE,
    DECRYPT_MODE,
} op_mode;


extern  uint8_t * state;
extern  uint8_t * register_key;
extern  uint8_t *round_key;

/**
 * Lookup table for PRESENT substitution process.
 */
static uint8_t const sbox[] = {0x0Cu, 0x05u, 0x06u, 0x0Bu, \
                                 0x09u, 0x00u, 0x0Au, 0x0Du, \
                                 0x03u, 0x0Eu, 0x0Fu, 0x08u, \
                                 0x04u, 0x07u, 0x01u, 0x02u};

/**
 * Lookup table for PRESENT inverse substitution process.
 */
static uint8_t const sbox_inv[] = {0x05u, 0x0Eu, 0x0Fu, 0x08u, \
                                     0x0Cu, 0x01u, 0x02u, 0x0Du, \
                                     0x0Bu, 0x04u, 0x06u, 0x03u, \
                                     0x00u, 0x07u, 0x09u, 0x0Au};
/**
 * @brief cette méthode petmet d'initialiser les variables globales
 * Ici on fait l'allocation dynamique de la m�moire avec calloc pour mettre les valeur � z�ro
 * state, register_key, round_key
 */
void init();
/**
 * @brief Pour la génération de la clé de registre: register_key
 *  
 * 
 */
void keygen();
/**
 * @brief Permet de faire un right shift circulaire sur le register_key
 * 
 */
void right_circle_shift();
/**
 * @brief Permet de faire la ronde sur la clé de registre pour dérivée une nouvelle clée.
 * 
 * @param round_counter : le round
 * @param mode :ENCRYPT_MODE or DECRYPT_MODE
 */
void update(uint8_t round_counter, op_mode mode);

/**
 * @brief Permet de recuperer la sous clé à partir de la clé de registre
 * 
 */
void sub_key();

/**
 * @brief Permet de faire state=state+round_key
 * 
 */
void add_round_key();
/**
 * @brief Permet d'enchainer deux sbox en même temps
 * 
 * @param input un bloc de 8 bit
 * @return uint8_t 
 */
uint8_t two_sbox(uint8_t input);
void simple_sbox(uint8_t input);

/**
 * @brief Appliquer la sbox sur tous les octets du state.
 * 
 */
void sBoxLayer();

/**
 * @brief La permutation de l'état interne
 * 
 * @param mode 
 */
void pLayer(op_mode mode);

/**
 * @brief Le chiffrement d'un message(state) de taille fixe.
 * 
 * @param msg : le message
 * @param key : la cle
 */
void encrypt(uint8_t * msg, uint8_t const * key);

/**
 * @brief Afficher Byte par byte une chaine
 * 
 * @param bytes 
 * @param size 
 */
void print_byte(uint8_t * bytes, int size);



void left_circle_shift();
/**
 * @brief Inverse de l'enchainnement de deux sbox en même temps
 * 
 * @param input 
 * @return uint8_t 
 */
uint8_t two_sbox_inv(uint8_t input);
/**
 * @brief Inverse de la sbox sur tous les octets du state.
 * 
 */
void sBoxLayer_inv();
/**
 * @brief Le dechiffrement d'un chiffré (state) de taille fixe.
 * 
 * @param msg : le message
 * @param key : la cle
 */
void decrypt(uint8_t * msg, uint8_t const * register_key);

/**
 * @brief Affuche les bytes en Hexa
 * 
 * @param bytes 
 * @param size 
 */
void printHex(uint8_t * bytes, int size);

/**
 * @brief Afficher un Byte en binaire
 * 
 * @param a 
 */
void toBinary(uint8_t a);
/**
 * @brief Afficher un tableau de bytes en binaire sur size bits=;
 * 
 * @param bytes 
 * @param size 
 */
void printBinary(uint8_t * bytes, int size);

#endif // PRESENT_H_INCLUDED
