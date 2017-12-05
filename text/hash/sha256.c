#include <libtext.h>

void sha256_text(char** text)
{
    char* hash = (char*)malloc(sizeof(char)*32);
    gcry_md_hash_buffer(GCRY_MD_SHA256, (void*) hash, (const void*) *text, strlen(*text)+1);
    free(*text);
    *text = hash;
}