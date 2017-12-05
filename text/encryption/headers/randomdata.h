#ifndef RANDOMDATA_H
#define RANDOMDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

uint8_t* genrand_rdrand(long);
uint8_t* genrand_random(long);
uint8_t* genrand_urandom(long);

#endif