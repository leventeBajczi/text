#include <libtext.h>

char* encode_base64(uint8_t* str, int len){
    char* base64;
    uint8_t* chars = (uint8_t*)malloc(sizeof(uint8_t)*len);
    memcpy(chars,str,len*sizeof(uint8_t));
    int help_len = len;
    int i = 0;
    while((len*8) % 6)
    {
        chars = (uint8_t*)realloc(chars, sizeof(uint8_t)*(len+1));
        chars[len] = 0xff;
        len++;
    }
    base64 = (char*)malloc(sizeof(char)*len*8/6 + 2);
    for(i= 0; i<len; i+=3){ //smallest common multiple of 8 and 6 is 24, which is 8*3
        base64[i*8/6] = to_base64_char(chars[i]>>2);
        chars[i  ] &= 0b00000011;
        base64[i*8/6 + 1] = to_base64_char(chars[i]<<4 | chars[i+1] >> 4);
        chars[i  ] &= 0b00000000;
        chars[i+1] &= 0b00001111;
        base64[i*8/6 + 2] = to_base64_char(chars[i+1]<<2 | chars[i+2] >> 6);
        chars[i+1] &= 0b00000000;
        chars[i+2] &= 0b00111111;
        base64[i*8/6 + 3] = to_base64_char(chars[i+2]);
        chars[i+2] &= 0b00000000;
    }
    for(help_len; help_len<len; help_len++){
        base64[i*8/6 - (len - help_len)] = '=';
    }
    base64[i*8/6] = '\0';

    memset(chars, 0, sizeof(uint8_t)*len);  //to delete the key from memory
    free(chars);
    return base64;
}
char to_base64_char(uint8_t c)      //A....Z, a....z, 0....9, +, /
{
    if(c <= 25) return (char)(c+'A');
    else if(c <= 51) return (char)(c+'a' - 26);
    else if(c <= 61) return (char)(c+'0' - 52);
    else if(c == 62) return '+';
    else return '/';
}


uint8_t* decode_base64(char* base64){
    uint8_t* chars = (uint8_t*)malloc(sizeof(uint8_t)*strlen(base64)*6/8);  //We know that it has 4k chars, so it'll be an integer
    for(int i = 0; i<strlen(base64); i+=4)
    {
        chars[i*6/8] = get_char(base64[i])<<2 | get_char(base64[i+1]) >> 4;
        chars[i*6/8 + 1] = get_char(base64[i+1])<<4 | get_char(base64[i+2]) >> 2;
        chars[i*6/8 + 2] = get_char(base64[i+2])<<6 | get_char(base64[i+3]);
    }
    return chars;
}

uint8_t get_char(char c)
{
    if('A' <= c && c <= 'Z') return (uint8_t)(c - 'A');
    else if('a' <= c && c <= 'z') return (uint8_t)(c - 'a' + 26);
    else if('0' <= c && c <= '9') return (uint8_t)(c - '0' + 52);
    else if(c == '+') return 62;
    else return 63;
}