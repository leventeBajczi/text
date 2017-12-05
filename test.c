#include <libtext.h>

int main()
{
    printf("%s\n", encode_base64("abcd", 4));

    return 0;
}