#include <libtext.h>

void test_rsa();
void test_aes();
void test_sha256();

int main()
{
    test_rsa();
    test_aes();
    test_sha256();
    return 0;
}

void test_rsa()
{
    char* private_key = NULL;
    char* public_key = NULL;
    int priv_len;
    int publ_len;
    int ciph_len;
    char* msg = (char*)malloc(sizeof(char)*10000);
    strcpy(msg, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque purus lectus, lacinia non lorem congue, pretium ullamcorper elit. Vestibulum congue nisl ligula.  Ut at aliquet risus. Sed commodo urna odio, sit amet posuere lectus interdum nec. Donec in bibendum nisi. Praesent pulvinar erat et metus mattis, at rutrum diam tempor.");

    printf("Starting string:\n%s\n", msg);

    generate_keypair(512, &private_key, &priv_len, &public_key, &publ_len);

    printf("\nPrivate key (base64 - S-expression):\n%s\n", encode_base64(private_key, priv_len));

    printf("\nPublic key (base64 - S-expression):\n%s\n", encode_base64(public_key, publ_len));

    encrypt_text(&msg, &ciph_len, public_key, publ_len);

    printf("\nEncrypted text (base64 - S-expression):\n%s\n", encode_base64(msg, ciph_len));
    
    msg = decode_base64(encode_base64(msg, ciph_len));
    private_key = decode_base64(encode_base64(private_key, priv_len));
    decrypt_text(&msg, &ciph_len, private_key, priv_len);

    printf("\nDecrypted text:\n%s\n", msg);
}

void test_aes()
{
    char* key = NULL;
    char* msg = (char*)malloc(sizeof(char)*10000);
    int len = 0;
    strcpy(msg, "a");

    printf("Starting string:\n%s\n", msg);

    generate_aes256_key(&key);

    printf("\nAES-256 Key (base64 - S-expression):\n%s\n", encode_base64(key, 32));

    len = strlen(msg) + 1;
    encrypt_aes256_text(&msg, &len, key);

    printf("\nEncrypted text (base64 - S-expression):\n%s\n", encode_base64(msg, len));

    decrypt_aes256_text(&msg, &len, key);

    printf("\nDecrypted text:\n%s\n", msg);
}

void test_sha256()
{
    char* key = NULL;
    char* msg = (char*)malloc(sizeof(char)*10000);

    strcpy(msg, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam efficitur, ex et maximus ullamcorper, quam lacus auctor ex, eu pretium libero diam vitae metus. Pellentesque feugiat eros id mi lacinia, et posuere sapien dignissim. Nam maximus aliquam lobortis. Nullam eleifend nec lectus sit amet molestie. Phasellus sagittis lectus tellus, sit amet dictum nunc ultricies eu. Morbi neque dui, pharetra quis fringilla ac, egestas eu tortor. Sed ac pulvinar ipsum, dignissim tempor velit. Donec nec velit semper, feugiat diam tristique, sagittis lacus. Duis nunc est, posuere vehicula felis ut, volutpat cursus orci. Nulla ultricies tellus accumsan diam imperdiet sodales. Sed interdum fringilla dapibus. Duis aliquet lacus vitae mi efficitur maximus.");

    printf("Starting string:\n%s\n", msg);

    sha256_text(&msg);

    printf("\nHashed string:\n%s\n", msg);
    
    printf("\nHashed string (base64):\n%s\n", encode_base64(msg, 32));
}