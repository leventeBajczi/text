#include <libtext.h>

void build_http(char* header, char* content)
{
    char* helper = (char*)malloc((strlen(content)+1)*sizeof(char));
    memcpy(helper, content, (strlen(content)+1)*sizeof(char));
    sprintf(content, "%s%d\r\n\r\n%s", header, strlen(helper), helper);
    free(helper);
}

char* get_http(char* str)
{
    char* helper = (char*)malloc(sizeof(char)*(1+strlen(str)));
    strcpy(helper, strstr(str, "\r\n\r\n") + 4);
    strcpy(str, helper);
    free(helper);
    return str;
}