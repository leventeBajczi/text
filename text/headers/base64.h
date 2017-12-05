#ifndef BASE64_H
#define BASE64_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* encode_base64(uint8_t*, int);
uint8_t* decode_base64(char*);
uint8_t get_char(char);
char to_base64_char(uint8_t);
#endif