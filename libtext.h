#ifndef LIBTEXT_H
#define LIBTEXT_H

#define KEYLEN 256

#define ENABLE_DEBUGGING


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

#ifndef HTTP_H
#define HTTP_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void build_http(char*, char*);
char* get_http(char*);

#endif

#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void build_json(char*, char*, char*);
void add_child(char*, char*, char*);
int get_value(char*, char*, char*);
void finalize(char*);
void replace_str(char*, char, char*);

#endif

#ifndef AES_H
#define AES_H

#include <unistd.h>
#include <gcrypt.h>

void handle_aes_key(char* );
char* get_aes_key();
void encrypt_aes(char** );
void decrypt_aes(char** );
void encrypt_private(char**, int);
void decrypt_private(char*, char*, int);
void generate_aes();


#endif

#ifndef RANDOMDATA_H
#define RANDOMDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

uint8_t* genrand_rdrand(long);
uint8_t* genrand_random(long);
uint8_t* genrand_urandom(long);

#endif

#ifndef RSA_H
#define RSA_H

#include <unistd.h>
#include <gcrypt.h>

char* load_public_key();
int encrypt_text(char**, int*, char*, int);
void decrypt_rsa(char*, int);
int generate_keypair(int, char**, int*, char**, int*);
int get_length(int);
size_t public_size(int);
size_t private_size(int);

#endif


#endif