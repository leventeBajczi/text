#include <libtext.h>

void generate_aes256_key(char** key)
{
    const static int keysize = 32;
    uint8_t *rand;
#if MODE == 0
    rand = genrand_rdrand(keysize);
#elif MODE == 1
    rand = genrand_urandom(keysize);
#elif MODE == 2
    rand = genrand_random(keysize);
#endif
    *key = encode_base64(rand, 32);
}

void encrypt_aes256_text(char** plain, int* length, char* key)
{
    const static int keysize = 32;
    size_t ciphlen = (1 + *length / gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256)) * gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);
    char* ciph = (char*)malloc(sizeof(char)*ciphlen);
    gcry_cipher_hd_t aes_ciph;
    gcry_error_t err = gcry_cipher_open(&aes_ciph, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_ECB, 0);
    if (err) {
#ifdef ENABLE_DEBUGGING
        printf ("Failure: %s/%s\n", gcry_strsource (err), gcry_strerror (err));
#endif
        abort();
    }

    err = gcry_cipher_setkey(aes_ciph, (const void*)key, keysize);
    if (err) {
#ifdef ENABLE_DEBUGGING
        printf ("Failure: %s/%s\n", gcry_strsource (err), gcry_strerror (err));
#endif
        abort();
    }

    err = gcry_cipher_encrypt(aes_ciph, (void*)ciph, ciphlen, (void*)*plain, ciphlen);
    if (err) {
#ifdef ENABLE_DEBUGGING
        printf ("Failure: %s/%s\n", gcry_strsource (err), gcry_strerror (err));
#endif
        abort();
    }

    free(*plain);
    *plain = ciph;
    *length = ciphlen;

    gcry_cipher_close(aes_ciph);
}


void decrypt_aes256_text(char** ciph, int* length, char* key)
{
    const static int keysize = 32;
    size_t plainlen = (1 + *length / gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256)) * gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES256);
    char* plain = (char*)malloc(sizeof(char)*plainlen);
    gcry_cipher_hd_t aes_ciph;

    gcry_error_t err = gcry_cipher_open(&aes_ciph, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_ECB, 0);
    if (err) {
#ifdef ENABLE_DEBUGGING
        printf ("Failure: %s/%s\n", gcry_strsource (err), gcry_strerror (err));
#endif
        abort();
    }

    err = gcry_cipher_setkey(aes_ciph, (const void*)key, keysize);
    if (err) {
#ifdef ENABLE_DEBUGGING
        printf ("Failure: %s/%s\n", gcry_strsource (err), gcry_strerror (err));
#endif
        abort();
    }

    err = gcry_cipher_decrypt(aes_ciph, (void*)plain, plainlen, (void*)*ciph, plainlen);
    if (err) {
#ifdef ENABLE_DEBUGGING
        printf ("Failure: %s/%s\n", gcry_strsource (err), gcry_strerror (err));
#endif
        abort();
    }

    free(*ciph);
    *ciph = plain;
    *length = plainlen;

    gcry_cipher_close(aes_ciph);



}