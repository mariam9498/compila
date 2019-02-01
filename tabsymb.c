
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "tabsymb.h"


void init_tab_sym() {
  tab_symb.last = -1;
}

void init_tab_pack_user() {
  tab_pack.last = -1;
}


bool add_symb(char *name, char *type, bool initialized, double value, int line) 
{
  bool exists = false;
  for (int i = 0; i <= tab_symb.last && !exists; i++) {
    if (!strcmp(tab_symb.symboles[i].name, name)) {
      exists = true;
    }
  }
  if (!exists) {
    tab_symb.last++;
    tab_symb.symboles[tab_symb.last].value = value;
    tab_symb.symboles[tab_symb.last].initialized = initialized;
    tab_symb.symboles[tab_symb.last].line = line;
    tab_symb.symboles[tab_symb.last].name = (char *) malloc ((1 + strlen(name)) * sizeof(char));
    strcpy(tab_symb.symboles[tab_symb.last].name, name);
    tab_symb.symboles[tab_symb.last].type = (char *) malloc ((1 + strlen(type)) * sizeof(char));
    strcpy(tab_symb.symboles[tab_symb.last].type, type);
  }
  return !exists;
}

bool add_pack(typetoken pack) 
{
  tab_pack.last ++;
  tab_pack.packs[tab_pack.last].name = pack;
}


extern bool check_type_assignment(char *type, typetoken value_type) {
  bool result ;
  if (!strcasecmp(type, "bool") && (value_type == B_FALSE || value_type == B_TRUE)) {
    result = true;
  } else if (!strcasecmp(type, "int") && value_type == INUMBER) {
    result = true;
  } else if (!strcasecmp(type, "double") && value_type == DNUMBER) {
    result = true;
  }
  else result=false;
  return result;
}