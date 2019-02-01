#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
// #include "analyseur_s.h"

#include "tabsymb.h"
#include "error.h"

#define debug false


extern int yylex();
typetoken token;
symb symbol;
// lit le prochain token s'il n'a pas déjà été lu par le prédicat d'un nullable
typetoken _lire_token(){
		return (typetoken) yylex();
	
}



void init_packs() {
  packs = (pack*) malloc(3*sizeof(pack));
  pack p1,p2,p3;
  p1.name = TEXT_IO;
  p2.name = INTEGER_TEXT_IO;
  p3.name = FLOAT_TEXT_IO;
  packs[0]=p1;
  packs[1]=p2;
  packs[2]=p3;
}

/* 
 PROG : LISTE_DECL begin LISTE_INST end
 LISTE_INST : INST LISTE_INSTAUX
 LISTE_INSTAUX : LISTE_INST  | epsilon
 LISTE_DECL : DECL LISTE_DECLAUX 
 LISTE_DECLAUX : LISTE_DECLAUX : LISTE_DECL | epsilon
 DECL : idf TYPE DECL_AUX
 DECL_AUX : CONST ';' | ';'
 TYPE : int | bool |float | char | string ; 
 CONST : inumber | dnumber | true | false
 INST : INST : idf = ADDSUB | true | false ';' 
      | if ‘(‘ idf == ADDSUB ‘)’ then LISTE_INST IF_INSTAUX 
      | print IDF ';'
      | for IDF = dnumber to dnumber do LISTE_INST endfor
 IF_INSTAUX :  endif  | else LISTE_INST endif

 ADDSUB : MULTDIV ADDSUBAUX

 ADDSUBAUX : – MULTDIV ADDSUBAUX
 ADDSUBAUX : + MULTDIV ADDSUBAUX
 ADDSUBAUX : epsilon

 MULTDIV : AUX MULTDIVAUX

 MULTDIVAUX : * AUX MULTDIVAUX ==> * MULTDIV
 MULTDIVAUX : / AUX MULTDIVAUX ==> / MULTDIV
 MULTDIVAUX : epsilon

 AUX : idf
 AUX : num
 AUX : ( ADDSUB )

*/


bool _list_inst(){
	return true;
}



// TYPE : int | bool |float | char | string ; 
bool _type() {
	bool result;
	if (debug) printf("type()\n");

	if (token == INT) {result = true;}
	else if (token == FLOAT) {result = true;}
	else if (token == BOOL) {result = true;}
    else if (token == CHAR) {result = true;}
    else if (token == STRING) {result = true;}
	else result = false;

	if (debug) printf("out of type()\n");
	return result;
}

//CONST : inumber | dnumber | true | false | character | string; 
bool _const() {
	bool result;
	if (debug) printf("const()\n");

	if (token == INUMBER) {
		
		result = true;	
	}else if (token == DNUMBER) {
		
		result = true;
	}else if (token == CHARACTER) {
		
		result = true;
	}else if (token == SSTRING) {
		result = true;
	}	
     else if ( (token == B_TRUE) || (token == B_FALSE) ){
		
		result = true;	
	} else result = false;

	if (debug) printf("out of const()\n");
	return result;
}










// DECL : idf TYPE DECL_AUX
bool _decl() {
	bool result;

	if ( token == IDF ) {
		symbol.name = (char *) malloc((strlen(yytext) + 1) * sizeof(char));
    	strcpy(symbol.name, yytext);
    	symbol.line = yylineno;
		token = _lire_token();
		if (token == COLUMN ){
			result=true;
			token = _lire_token();
				if (_type()){
					symbol.type = (char *) malloc((strlen(yytext) + 1) * sizeof(char));
      				strcpy(symbol.type, yytext);
					token = _lire_token();
					if (token == COLUMN_EQ){
						result=true;
						token = _lire_token();
						if (_decl_aux() == true){
							result=true;
						}else result = false;
					}else result = false;
				}else result = false;
		}else result = false;
	}else result = false;
	return result;
}
		
// DECL_AUX : CONST ';' | ';'
bool _decl_aux() {
	bool result;
	
	if ( token == PVIRG ) { 
		symbol.initialized = false;
    if (!add_symb(symbol.name, symbol.type, symbol.initialized, symbol.value, symbol.line)) {
      creer_sm_erreur(AlreadyDeclared, symbol.name, symbol.line);}
		result = true; 
	}else if (_const()) {
		symbol.initialized = true;
      if (!check_type_assignment(symbol.type, token)) {
        creer_sm_erreur(NoneInitialized, symbol.name, symbol.line);}
	else {
        if (!strcasecmp(symbol.type, "bool")) {
          if (!strcasecmp(yytext, "true")) {
            symbol.value = 1.0;
          } else {
            symbol.value = 0.0;
          }
        } else {
          symbol.value = atof(yytext);
        }
      }
		token = _lire_token();
		if (token == PVIRG) {
			result = true;
        if (!add_symb(symbol.name, symbol.type, symbol.initialized, symbol.value, symbol.line)) {
        creer_sm_erreur(AlreadyDeclared, symbol.name, symbol.line);
        }
		} else result = false;
	}else result = false;

	
	return result;
}

bool _package_aux(){
	bool result=false;
	token=_lire_token();
	if(_package()){
		result=true;
	}else {result=false;}
	if(_decl_proc()){

		result=true;
	}else {result=false;}
	return result;
}
bool _package(){

	bool result=false;
	bool exist = false;
	char* p = (char *)malloc(1024);

	if(token==WITH){
		result=true;
		token = _lire_token();
		add_pack(token);
		strcpy(p, yytext);
    

		if (token==TEXT_IO){
      		tab_pack.packs[tab_pack.last].valide = 1;
			tab_pack.packs[tab_pack.last].line = yylineno;
			exist = true;
			result=true;
			
			token = _lire_token();
			if(token==PVIRG){
				result=true;
				token=_lire_token();
				if(token==USE){
					result=true;
					token = _lire_token();
					if(token==TEXT_IO){
						token = _lire_token();
						result=true;
						if(token==PVIRG){
							result=true;
							if(_package_aux()){
							result=true;
							}else result = false;
						}else result = false;
					}else{ result = false;
        				creer_sm_erreur(PackNotExist, p, tab_pack.packs[tab_pack.last].line);}
						}
				}else result = false;
			}else result = false;
		}else result = false;	

		
	if(exist == false){
        creer_sm_erreur(PackNotExist, p, tab_pack.packs[tab_pack.last].line);}

	}else result = false;

	return result;
}

// LISTE_DECL : DECL LISTE_DECLAUX
bool _liste_decl(){
	bool result;
	if (debug) printf("liste_decl()\n");

	if (_decl()){
		token = _lire_token();
		result = _liste_decl_aux();
	}else result = false;

	if (debug) printf("out of liste_decl()\n");
	return result;
}

// LISTE_DECLAUX : LISTE_DECL | '.' ==> LISTE_DECLAUX : LISTE_DECL | epsilon
// NULLABLE(LISTE_DECLAUX) = true
// follow(LISTE_DECLAUX) = {begin}
// first(LISTE_DECLAUX) = {idf}
bool _liste_decl_aux(){
	bool result;
	if (debug) printf("liste_decl_aux()\n");

	if (token == BEG_IN){
		result = true;
	}else if (_liste_decl()){
		result = true;
	}else{
		/* printf("TOKE = %d\n",token);
		creer_sx_erreur(BeginExpected, tokenattribute.line); */
		result = false;
	}

	if (debug) printf("out of liste_decl_aux()\n");
	return result;
}

bool _decl_proc(){
	bool result;
	
	if (token == PROCEDURE){
		result=true;
		token=_lire_token();
		if(token == IDF){
			result=true;
			token=_lire_token();
			if(token == IS){
			result=true;
			token=_lire_token();
				if(_liste_decl() || token == BEG_IN ){
				result=true;
				token=_lire_token();
					if (_list_inst()){
						result=true;
						token=_lire_token();
						if (token==END){
							result=true;
							token=_lire_token();
							if (token==IDF||token ==PVIRG){
								result=true;
							}else result = false;
						}else result = false;
					}else result = false;
				}else result = false;
			}else result = false;
		}else result = false;
	}else result = false;
	return result;
}



int main(){
	creer_file_erreur();
  	init_tab_sym();
	token=_lire_token();
	if(_package())
		printf("ok \n");
	else
		{printf("not ok \n");
		afficher_sm_erreurs();
		printf("Error package");}
	
	return 0;
}
