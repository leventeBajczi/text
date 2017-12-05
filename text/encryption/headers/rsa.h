#ifndef RSA_H
#define RSA_H

#include <unistd.h>
#include <gcrypt.h>
#include "../../../file/headers/io.h"
#include "../../headers/base64.h"
#include "aes.h"
#include "../../../defines.h"

char* load_public_key();
char* encrypt_rsa(char*, char*, int);
void decrypt_rsa(char*, int);
void generate_keypair();

#endif