#include <libtext.h>


int encrypt_text(char** plain, int* ciphlen, char* key_s, int keylen)
{
    gcry_sexp_t enkey;
    gcry_mpi_t msg;
    gcry_sexp_t data;   
    gcry_sexp_t ciph;
    void* rsa_buf = malloc(private_size(keylen));
    int len = 0, i = 0, j = 0;

    gcry_error_t err = gcry_sexp_new(&enkey, (void*)(key_s), keylen, 1);
    if(err)
    {
#ifdef ENABLE_DEBUGGING
        printf ("Failure: %s/%s\n", gcry_strsource (err), gcry_strerror (err));
#endif
        abort();
    }

    err = gcry_mpi_scan(&msg, GCRYMPI_FMT_USG, *plain, strlen(*plain), NULL);
    if (err) {
#ifdef ENABLE_DEBUGGING
        printf("failed to create a mpi from the message");
#endif
        abort();
    }

    err = gcry_sexp_build(&data, NULL,"(data (flags raw) (value %m))", msg);

    if (err) {
#ifdef ENABLE_DEBUGGING
        printf("failed to create a sexp from the message");
#endif
        abort();
    }

    err = gcry_pk_encrypt(&ciph, data, enkey);
    if (err) {
#ifdef ENABLE_DEBUGGING
        printf("gcrypt: encryption failed");
#endif
    }

    gcry_sexp_sprint(ciph, GCRYSEXP_FMT_CANON, rsa_buf, private_size(keylen));
    while(j || !len)
    {
        switch(((char*)rsa_buf)[len])
        {
            case '(': len++; j++; break;
            case ')': 
                len++;
                j--; break;
            case ':': len+=i+1; i = 0; break;
            default: i=i*10 + ((char*)rsa_buf)[len]-'0'; len++; break;
        }
    }
    *plain = (char*)realloc(*plain, len);

    memcpy(*plain, rsa_buf, len);
    *ciphlen = len;

    

}

int decrypt_text(char* cipher, char* key_s)
{
    
}

/*
Generates a keylen sized keypair to use with RSA. It's char* representation will be given back as two freshly reallocated strings.
*/

int generate_keypair(int keylen, char** key_pr, int* prlen, char** key_pub, int* publen)
{
    gcry_error_t err = 0;
    gcry_sexp_t rsa_parms;
    gcry_sexp_t rsa_keypair;
    gcry_sexp_t rsa_data; 
    char init[50];

#ifdef ENABLE_DEBUGGING
    printf("Generating a new keypair, please hold still....\n");
#endif
    sprintf(init, "(genkey (rsa (nbits %d:%d)))", get_length(keylen), keylen*8);
    err = gcry_sexp_build(&rsa_parms, NULL, init);


    if (err) {
#ifdef ENABLE_DEBUGGING
        printf("gcrypt: failed to create rsa params");
#endif
        abort();
    }

    err = gcry_pk_genkey(&rsa_keypair, rsa_parms);
    if (err) {
#ifdef ENABLE_DEBUGGING
        printf("gcrypt: failed to create rsa key pair");
#endif
        abort();
    }

#ifdef ENABLE_DEBUGGING
    printf("Key successfully generated\n");
#endif

//We have a key now, time to export it..

    int len = 0, i = 0, j = 0;
    rsa_data = gcry_sexp_nth(rsa_keypair, 2);
    void* rsa_buf = malloc(private_size(keylen));

    memset(rsa_buf, 0, private_size(keylen));

    gcry_sexp_sprint(rsa_data, GCRYSEXP_FMT_CANON, rsa_buf, private_size(keylen));
    while(j || !len)
    {
        switch(((char*)rsa_buf)[len])
        {
            case '(': len++; j++; break;
            case ')': 
                len++;
                j--; break;
            case ':': len+=i+1; i = 0; break;
            default: i=i*10 + ((char*)rsa_buf)[len]-'0'; len++; break;
        }
    }

    *key_pr  = (char*)realloc(*key_pr, len);
    memcpy(*key_pr, rsa_buf, len);
    *prlen = len;
    //Private key is already 'saved'
    memset(rsa_buf, 0, len);


//extracting the public key

    rsa_data = gcry_sexp_nth(rsa_keypair, 1);
    rsa_buf = realloc(rsa_buf, public_size(keylen));
    memset(rsa_buf, 0, public_size(keylen));
    i = 0;
    j = 0;
    len = 0;
    gcry_sexp_sprint(rsa_data, GCRYSEXP_FMT_CANON, rsa_buf, public_size(keylen));
    while(j || !len)
    {
        switch(((char*)rsa_buf)[len])
        {
            case '(': len++; j++; break;
            case ')': 
                len++;
                j--; break;
            case ':': len+=i+1; i = 0; break;
            default: i=i*10 + ((char*)rsa_buf)[len]-'0'; len++; break;
        }
    }

    *key_pub  = (char*)realloc(*key_pub, len);
    memcpy(*key_pub, rsa_buf, len);
    *publen = len;
    //We don't need anything anymore, cleaning up
    memset(rsa_buf, 0, len);
    free(rsa_buf);

#ifdef ENABLE_DEBUGGING
    printf("Done generating keys...\n");
#endif


    gcry_sexp_release(rsa_data);
    gcry_sexp_release(rsa_keypair);
    gcry_sexp_release(rsa_parms);

}

int get_length(int keylen)
{
    int i = 1;
    while(keylen)
    {
        keylen/=10;
        i++;
    }
    return i;
}

size_t public_size(int keylen)
{
    return 10*keylen;
}

size_t private_size(int keylen)
{
    return 10*keylen;
}


char* password = "abcd";

char* load_public_key()
{
    //return read_key("public.key"); 
}
char* encrypt_rsa(char* key, char* content, int len)
{
    gcry_sexp_t enkey;
    size_t length = strchr(key, '=') ? (strchr(key, '=') - key)*3/4 : strlen(key)*3/4;
    int err = gcry_sexp_new (&enkey, (void*)decode_base64(key), length, 1);
    if(err)
    {
        printf("Failed to build up key");
        abort();
    }
    gcry_mpi_t msg;
    gcry_sexp_t data;   

    err = gcry_mpi_scan(&msg, GCRYMPI_FMT_USG, content, len, NULL);
    if (err) {
        printf("failed to create a mpi from the message");
        abort();
    }
    err = gcry_sexp_build(&data, NULL,"(data (flags raw) (value %m))", msg);

    if (err) {
        printf("failed to create a sexp from the message");
        abort();
    }

    gcry_sexp_t ciph;
    err = gcry_pk_encrypt(&ciph, data, enkey);
    if (err) {
        printf("gcrypt: encryption failed");
    }

    void* rsa_buf = calloc(1, KEYLEN*3);
    int i =0,  j = 0;
    int slen = 0;
    gcry_sexp_sprint(ciph, GCRYSEXP_FMT_CANON, rsa_buf, KEYLEN*3);
    while(j || !slen)
    {
        switch(((char*)rsa_buf)[slen])
        {
            case '(': slen++; j++; break;
            case ')': 
                slen++;
                j--; break;
            case ':': slen+=i+1; i = 0; break;
            default: i=i*10 + ((char*)rsa_buf)[slen]-'0'; slen++; break;
        }
    }
    return encode_base64(rsa_buf, slen);

}
void decrypt_rsa(char* content, int len)
{
    char* key/* = read_key("private.key")*/;

    if(!password);//decrypt_aes

    gcry_sexp_t skey;
    size_t length = (strlen(key))*3/4;

    key = decode_base64(key);
    decrypt_private(password, key, length);
    int i = 0, j = 0, jump = 0;

    while(i<length)
    {
        switch(key[i])
        {
            case '(': i++; j++; break;
            case ')': 
                i++;
                j--; break;
            case ':': i+=jump+1; jump = 0; break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': jump=jump*10 + key[i]-'0'; i++; break;
            default: if(j){
                key[i] = ')';
            } else length = i; break;
        }
    }

    int err = gcry_sexp_new(&skey, (void*)key, length, 1);
    if(err)
    {
        printf("couldn't build key");
    }
    gcry_sexp_t plain;
    gcry_sexp_t ciph;

    i = 0; j = 0; jump = 0;
    while(i<len)
    {
        switch(content[i])
        {
            case '(': i++; j++; break;
            case ')': 
                i++;
                j--; break;
            case ':': i+=jump+1; jump = 0; break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': jump=jump*10 + content[i]-'0'; i++; break;
            default: if(j){
                content[i] = ')';
            } else len = i; break;
        }
    }


    err = gcry_sexp_new(&ciph, (void*)content, len, 1);
    if(err)
    {
        printf("Failed to build up content");
        abort();
    }
    
    err = gcry_pk_decrypt(&plain, ciph, skey);
    if (err) {
        printf("gcrypt: decryption failed");
    }
    
}