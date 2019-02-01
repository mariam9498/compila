


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
LOOP,
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
EQ,
DEUXPOINT,
INF,
SUP,
INFEQ,
SUPEQ,
POPEN,
PCLOSE,
WHILE,
IN,
TWODOTS,
ARROW,
OTHERS,
ENCASE,
CASE,
FOR,
WHEN,
PRINT,
PLUS,
MINUS,
MULT,
DIV,
PCLOSE,
POPEN,
ERRORLEX
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


bool loop_statement();
bool _case_body();
bool _case();
bool _case_body_aux();
bool iteration_scheme();
bool _loop_parameter_specification();
bool _discrete_interval();

bool _condition(typetoken t);
bool _if_instruction() ;
bool _if_instruction_aux() ;

bool _put();
bool _put_line();

bool _addsub();
bool _addsubaux();
bool _multdiv();
bool _multdivaux();
bool _aux();





