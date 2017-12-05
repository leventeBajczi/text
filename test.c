#include <libtext.h>

int main()
{
    char* private_key = NULL;
    char* public_key = NULL;
    int priv_len;
    int publ_len;
    int ciph_len;
    char* msg = (char*)malloc(sizeof(char)*10);
    strcpy(msg, "Hello World");

    generate_keypair(256, &private_key, &priv_len, &public_key, &publ_len);

    write(1, private_key, priv_len);
    printf("\n\n%s\n", encode_base64(private_key, priv_len));

    printf("\n\n%s\n", encode_base64(public_key, publ_len));

    encrypt_text(&msg, &ciph_len, private_key, priv_len);

    printf("\n\n%s\n", encode_base64(msg, ciph_len));
    

    return 0;
}