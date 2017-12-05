#include "headers/randomdata.h"

extern uint64_t rdrand();

uint8_t* genrand_rdrand(long length)
{
    uint8_t *result = (uint8_t*)malloc(sizeof(uint8_t)*length);
    for(int i = 0; i<length; i++)
    {
        result[i] = rdrand();
    }
    return result;
}

uint8_t* genrand_urandom(long length)
{

    int randomdata = open("/dev/urandom", O_RDONLY);
    if (randomdata >= 0)
    {
        uint8_t *result = (uint8_t*)malloc(sizeof(uint8_t)*length);
        size_t s = read(randomdata, result, length);
        if(s >= 0)
        {
            close(randomdata);
            return result;
        }

    }
    return NULL;
}

uint8_t* genrand_random(long length)
{
    size_t sum = 0;
    int randomdata = open("/dev/random", O_RDONLY);
    if (randomdata >= 0)
    {
        uint8_t *result = (uint8_t*)malloc(sizeof(uint8_t)*length);
        while(sum < length)
        {
            printf("\r%lf%%\n", 100 * (double)sum / length);
            ssize_t s = read(randomdata, result + sum, length - s);
            if(s>=0)sum+=s;
        }
        close(randomdata);
        return result;

    }
    return NULL;
}