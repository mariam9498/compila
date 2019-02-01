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
CSTRING \"[^"]*\"

%%
"Ada.Text_IO"           return TEXT_IO;
"Ada.Integer_Text_IO"   return INTEGER_TEXT_IO;
"Ada.Float_Text_IO"     return FLOAT_TEXT_IO;
"begin"			        return BEG_IN;
"int" 			        return INT;
"bool" 			        return BOOL;
"float" 		        return FLOAT;
"true" 			        return TRUE;
"false" 		        return FALSE;
"Character"             return CHAR;
"String"                return STRING;
"loop"                  return LOOP;
"end"                   return END;
":="                    return EQ;
":"                     return DEUXPOINT;
";"                     return PVIRG;
"="                     return EQEQ;
"put"                   return PRINT;

"procedure"				 return PROCEDURE;

"with"                   return WITH;
"use"                    return USE;


"="                 return EQEQ;
"<"                 return INF;
">"                 return SUP;
"<="                 return INFEQ;
">="                 return SUPEQ;

"if"			return IF;
"then"			return THEN;
"else"			return ELSE;
"endif"                    return ENDIF;

"("                    return POPEN;
")"                    return PCLOSE;


"is"        return IS;
"+"			return PLUS;
"-"			return MINUS;
"*"			return MULT;
"/"			return DIV;

"add"		return ERRORLEX;
"idiv"		return ERRORLEX;
"ddiv"		return ERRORLEX;

"dupl" 		return ERRORLEX;	
"jmp" 		return ERRORLEX;	
"jne" 		return ERRORLEX;	
"jg" 		return ERRORLEX;	
"load" 		return ERRORLEX;	
"mult" 		return ERRORLEX;	
"push" 		return ERRORLEX;	
"sub" 		return ERRORLEX;	
"store"		return ERRORLEX;	
"swap" 		return ERRORLEX;	

{IDF}        	 return IDF;     
{DNUMBER}	   return DNUMBER;     
{INUMBER}		return INUMBER;                      
{CHARACTER}     return CHARACTER;          
{CSTRING}    return CSTRING;


[\n\t ]+
.                        printf("ERROR(%s)\n", yytext);

%%
