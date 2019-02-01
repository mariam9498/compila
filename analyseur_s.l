%option noyywrap
%option caseless

%option yylineno

%{

#include "analyseur_s.h"
%}
IDF [a-zA-Z](_?[a-zA-Z0-9])* 	      
DNUMBER [\-\+]?[0-9]+\.[0-9]+     
INUMBER [\-\+]?[0-9]+                      
CHARACTER '[a-zA-Z0-9]'         
SSTRING \"[a-zA-Z0-9]*\"

%%
"Ada.Text_IO"   return TEXT_IO;
"Ada.Integer_Text_IO"   return INTEGER_TEXT_IO;
"Ada.Float_Text_IO"   return FLOAT_TEXT_IO;
"begin"			return BEG_IN;
"int" 			return INT;
"bool" 			return BOOL;
"float" 		return FLOAT;
"true" 			return B_TRUE;
"false" 		return B_FALSE;
"Character"     return CHAR;
"String"       return STRING;
"end"                    return END;
":="                     return COLUMN_EQ;
":"                      return COLUMN;
";"                 return PVIRG;

"procedure"				 return PROCEDURE;

"with"                   return WITH;
"use"                    return USE;

"is"                     return IS;
{IDF}        	 return IDF;     
{DNUMBER}	   return DNUMBER;     
{INUMBER}		return INUMBER;                      
{CHARACTER}     return CHARACTER;          
{SSTRING}    return SSTRING;


[\n\t ]+
.                        printf("ERROR(%s)\n", yytext);

%%
