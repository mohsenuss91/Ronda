#ifndef BASIC_H
#define BASIC_H

#include "define.h"

/*
 * checks if  a <= x <= b
 */
inline bool between(unsigned short x, unsigned short a, unsigned short b);

/*
 * checks of a == b
 */ 
inline bool equal(int a, int b);

/*
 * mixes cards in array
 */ 
void mix(card_num array[], unsigned short size);

/*
 * swaps contents of a and b
 */ 
void swap(card_num *a, card_num *b);

/*
 * checks if value exists in tab
 */ 
short exist(card tab[], unsigned short size, card_num value);

/*
 * checks if tab is empty
 */ 
bool empty(card tab[], unsigned short size);

/*
 * counts cards in tab
 */ 
unsigned short nb_cards(card tab[], unsigned short size);

/*
 * returns path to file representing the card n
 */  
char* get_file(card_num n);

/*
 * writes error messages in stderr
 */ 
bool sdl_error();

/*
 * sets card attributes given in parameters
 */ 
bool set_card(card *c, card_num num, short posx, short posy, bool back);
#endif
