

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "analyseur_s.h"


typedef struct {
    char* name;
    char* type;
    bool initialized;
    double value;
    int line;
}symb;


#define MAX_SYMB 100
typedef struct {
  symb symboles[MAX_SYMB];
  int last;
} tab_symb_type;

tab_symb_type tab_symb;


typedef struct pack_user{
   typetoken name;
   int line;
   bool valide;
}pack_user;

//structure de packages
typedef struct tab_packs{
   pack_user packs[10];
   int last;
}tab_packs;

tab_packs tab_pack;

typedef struct sous_pack_user{
   typetoken name;
}sous_pack_user;

//structure de packages
typedef struct tab_sous_packs{
   pack_user packs[10];
   int last;
}tab_sous_packs;

tab_sous_packs tab_sous_pack;

extern void init_tab_sym();

extern bool add_symb(char *name, char *type, bool initialized, double value, int line);

extern bool check_type_assignment(char *type, typetoken value_type);

bool add_pack(typetoken pack) ;
bool add_sous_pack(typetoken sous_pack) ;