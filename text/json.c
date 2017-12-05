#include "headers/json.h"


void build_json(char* json, char* key, char* value){        //json MUST be long enough!
    char* sub = (char*)malloc(sizeof(char)*(strlen(key) + strlen(value) + 13));
    if(!strlen(json))strcpy(json, "{}");  //if a new json string needs to be created, initialize it
    json[strlen(json)-1] = '\0';              //Delete the last }
    sprintf(sub, "\"%s\": \"%s\",}", key, value);
    strcat(json, sub);
    free(sub);
}
void add_child(char* parent, char* key, char* child){
    char* sub = (char*)malloc(sizeof(char)*(strlen(key) + strlen(child) + 11));
    if(!strlen(parent))strcpy(parent, "{}");  //if a new json string needs to be created, initialize it
    parent[strlen(parent)-1] = '\0';              //Delete the last }
    sprintf(sub, "\"%s\": %s,}", key, child);
    strcat(parent, sub);
    free(sub);
}

int get_value(char* json, char* key, char* value){
    int counter = 0, j = 0, en = 0;
    char terminating_char;
    for(int i = 0; i<= strlen(json); i++)
    {
        if(en){
            value[j++] = json[i];
            if(json[i] == terminating_char){
                if(j==1) j--;
                else
                {
                    value[j-1] = '\0';
                    return 1;
                }
            }
            
        }
        else{

            if(json[i] == '{')counter++;
            else if(json[i] == '}')counter--;
    
            if(counter == 1 && strstr(json+i, key) == json+i){
                en = 1;
                i+=strlen(key) + 2;
                terminating_char = (json[i+1] == '{') ? '}' : '\"';
            }
        }
    }

    return 0;
}

void finalize(char* msg){
    char en = 0;        //boolean, enable
    for(int i = strlen(msg); i>=0; i--){
        if(msg[i] == '}') en = 1;
        else if(en && (msg[i] == ',')){
            msg[i] = ' ';   //a space is less problematic then an ending comma 
            en = 0;
        }
    }
    msg[strlen(msg)] = 0;
}
