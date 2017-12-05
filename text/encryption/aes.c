#include "headers/aes.h"

gcry_cipher_hd_t aes_ciph;
char* aes_key;

extern char* password;

void handle_aes_key(char* key)
{
    char* decoded = decode_base64(key);
    aes_key = decoded;
    int err = gcry_cipher_open(&aes_ciph, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_ECB, 0);
    if (err) {
        printf("gcrypt: failed to create aes handle");
    }

    err = gcry_cipher_setkey(aes_ciph, (const void*) decoded, 32);
    if (err) {
        printf("gcrypt: could not set cipher key");
    }
}
char* get_aes_key()
{
    return aes_key;
}
void encrypt_aes(char** content)
{
    size_t size = ((strlen(*content) + 1) / gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256) + 1)*gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);
    char* encrypted = (char*)malloc(size);
    int err = gcry_cipher_encrypt(aes_ciph, (unsigned char*)encrypted, size, *content, size);
    if (err) {
        printf("gcrypt: could not encrypt with AES");
        abort();
    }
    *content = encode_base64(encrypted, size);
}
void decrypt_aes(char** content)
{
    size_t size = strlen(*content)*3/4 - strlen(*content)*3/4 % gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);
    *content = decode_base64(*content);
    int err = gcry_cipher_decrypt(aes_ciph, (unsigned char*) *content, size, NULL, 0);
    if (err) {
        printf("gcrypt: could not decrypt with AES");
        abort();
    }
}

void encrypt_private(char** rsa, int len)
{
    gcry_cipher_hd_t aes_hd;
    char passwd_hash[32];
    char passwd[60];
    scanf("%s", passwd);
    password = passwd;
    size_t pass_len = passwd[0] ? strlen(passwd) : 0;

    if(!pass_len)
    {
        printf("Invalid password, quitting");
        abort();
    }

    int err = gcry_cipher_open(&aes_hd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_ECB, 0);
    if (err) {
        printf("gcrypt: failed to create aes handle");
        abort();
    }

    gcry_md_hash_buffer(GCRY_MD_SHA256, (void*) &passwd_hash, (const void*) passwd, pass_len);

    err = gcry_cipher_setkey(aes_hd, (const void*) &passwd_hash, 32);
    if (err) {
        printf("gcrypt: could not set cipher key");
        abort();
    }

    err = gcry_cipher_setiv(aes_hd, (const void*) &passwd_hash, 16);
    if (err) {
        printf("gcrypt: could not set cipher initialization vector");
        abort();
    }

    size_t size = (len / gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256) + 1)*gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);

    err = gcry_cipher_encrypt(aes_hd, (unsigned char*) *rsa, size, NULL, 0);
    if (err) {
        printf("gcrypt: could not encrypt with AES");
        abort();
    }
    gcry_cipher_close(aes_hd);

}

void decrypt_private(char* passwd, char* rsa, int len)
{
    gcry_cipher_hd_t aes_hd;
    char passwd_hash[32];
    size_t pass_len = passwd[0] ? strlen(passwd) : 0;
    if(!pass_len)
    {
        printf("Invalid password, quitting");
        abort();
    }

    int err = gcry_cipher_open(&aes_hd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_ECB, 0);
    if (err) {
        printf("gcrypt: failed to create aes handle");
        abort();
    }

    gcry_md_hash_buffer(GCRY_MD_SHA256, (void*) &passwd_hash, (const void*) passwd, pass_len);

    err = gcry_cipher_setkey(aes_hd, (const void*) &passwd_hash, 32);
    if (err) {
        printf("gcrypt: could not set cipher key");
        abort();
    }

    err = gcry_cipher_setiv(aes_hd, (const void*) &passwd_hash, 16);
    if (err) {
        printf("gcrypt: could not set cipher initialization vector");
        abort();
    }

    size_t size = (len / gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256) + 1)*gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);


    err = gcry_cipher_decrypt(aes_hd, (unsigned char*)rsa, size, NULL, 0);

    if (err) {
        printf("gcrypt: could not encrypt with AES");
        abort();
    }
    gcry_cipher_close(aes_hd);

}

void generate_aes()
{
    uint8_t *rand;
#if MODE == 0
    rand = genrand_rdrand(32);
#elif MODE == 1
    rand = genrand_urandom(32);
#elif MODE == 2
    rand = genrand_random(32);
#endif

    aes_key = encode_base64(rand, 32);

    int err = gcry_cipher_open(&aes_ciph, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_ECB, 0);
    if (err) {
        printf("gcrypt: failed to create aes handle");
    }

    err = gcry_cipher_setkey(aes_ciph, (const void*) rand, 32);
    if (err) {
        printf("gcrypt: could not set cipher key");
    }

}