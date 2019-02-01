


#include <stdio.h>
#include <stdbool.h>
#include<stdlib.h>



//typedef enum {false=0, true=1} boolean;

typedef enum {
INUMBER,
IDF,
INT,
BOOL,
B_TRUE,
B_FALSE,
PVIRG,
BEG_IN,
END,
PROCEDURE,
DNUMBER,
FLOAT,
CHAR,
STRING,
SSTRING,
CHARACTER,
USE,
WITH,
COLUMN_EQ,
COLUMN,
IS,
TEXT_IO,
INTEGER_TEXT_IO,
FLOAT_TEXT_IO,
IF,
THEN,
ELSE,
ENDIF,
EQEQ,
INF,
SUP,
INFEQ,
SUPEQ,
POPEN,
PCLOSE
} typetoken;


//structure de packages
typedef struct pack{
   typetoken name;
}pack;

pack * packs;

void init_packs();

extern int yylex();

extern char *yytext;
extern int yylineno;


void set_idf_attributes(char *name, int line);
void set_number_attributes(double value);
void set_character_attributes(char* value);
void set_token_attributes(int line);
bool _type();
bool _const();
typetoken _lire_token();
bool _package();
bool _decl() ;
bool _decl_aux() ;
bool _liste_decl();
bool _liste_decl_aux();
bool _decl_proc();
bool  _list_inst();
bool _package_aux();

bool _condition(typetoken t);
bool _if_instruction() ;
bool _if_instruction_aux() ;

bool _put();
bool _put_line();




