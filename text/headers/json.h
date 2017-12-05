#ifndef JSON_H
#define JSON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void build_json(char*, char*, char*);
void add_child(char*, char*, char*);
int get_value(char*, char*, char*);
void finalize(char*);
void replace_str(char*, char, char*);

#endif