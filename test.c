#include <libtext.h>

int main()
{
    char* private_key = NULL;
    char* public_key = NULL;
    int priv_len;
    int publ_len;
    int ciph_len;
    char* msg = (char*)malloc(sizeof(char)*10000);
    strcpy(msg, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque purus lectus, lacinia non lorem congue, pretium ullamcorper elit. Vestibulum congue nisl ligula.  Ut at aliquet risus. Sed commodo urna odio, sit amet posuere lectus interdum nec. Donec in bibendum nisi. Praesent pulvinar erat et metus mattis, at rutrum diam tempor.");

    generate_keypair(512, &private_key, &priv_len, &public_key, &publ_len);

    printf("\n\n%s\n", encode_base64(private_key, priv_len));

    printf("\n\n%s\n", encode_base64(public_key, publ_len));

    encrypt_text(&msg, &ciph_len, public_key, publ_len);

    printf("\n\n%s\n", encode_base64(msg, ciph_len));
    
    msg = decode_base64(encode_base64(msg, ciph_len));
    private_key = decode_base64(encode_base64(private_key, priv_len));
    decrypt_text(&msg, &ciph_len, private_key, priv_len);

    printf("\n\n%s\n", msg);
    
    return 0;
}