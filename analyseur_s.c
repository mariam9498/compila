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
	}else if ( (token == B_TRUE) || (token == B_FALSE) ){
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
		if (token == DEUXPOINT ){
			result=true;
			token = _lire_token();
				if (_type()){
					symbol.type = (char *) malloc((strlen(yytext) + 1) * sizeof(char));
      				strcpy(symbol.type, yytext);
					  printf("%s",symbol.type);
					token = _lire_token();
						if (_decl_aux()){
							result=true;
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
		printf("ddd") ; 
		symbol.initialized = false;
    if (!add_symb(symbol.name, symbol.type, symbol.initialized, symbol.value, symbol.line)) {
      creer_sm_erreur(AlreadyDeclared, symbol.name, symbol.line);}
		result = true; 
	}else if (token == EQ){
						result=true;
						token = _lire_token();
						if(_const()) {
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
				}else result = false;
	
	return result;
}

bool _package_aux(){
	bool result=false;
	
	if(_package()||token==PROCEDURE){
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
			printf("PPP\n");
			token = _lire_token();
			if(token==PVIRG){
				result=true;
				printf("PPP\n");
				token=_lire_token();
				if(token==USE){
					printf("PPP\n");
					result=true;
					token = _lire_token();
					if(token==TEXT_IO){
						printf("PPP\n");
						token = _lire_token();
						result=true;
						if(token==PVIRG){
							printf("PPP\n");
							result=true;
							token=_lire_token();
							if(_package_aux()){
								printf("PPP\n");
							result=true;
							}else result = false;
						}else result = false;
					}else{ result = false;
        				creer_sm_erreur(SousPackNotExist, p, tab_pack.packs[tab_pack.last].line);}
						}
				}else result = false;
			}else if(token==INTEGER_TEXT_IO){

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
						if(token==INTEGER_TEXT_IO){
						 	token = _lire_token();
							result=true;
							if(token==PVIRG){
								result=true;
								if(_package_aux()){
								result=true;
								}else result = false;
							}else result = false;
						}else{ result = false;
        				creer_sm_erreur(SousPackNotExist, p, tab_pack.packs[tab_pack.last].line);}
					}else result = false;

				}
			}else if (token==FLOAT_TEXT_IO){

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
						if(token==FLOAT_TEXT_IO){
						 	token = _lire_token();
							result=true;
							if(token==PVIRG){
								result=true;
								if(_package_aux()){
								result=true;
								}else result = false;
							}else result = false;
						}else{ result = false;
        				creer_sm_erreur(SousPackNotExist, p, tab_pack.packs[tab_pack.last].line);}
					}else result = false;
		
		}
	}
	if(exist == false){
        	creer_sm_erreur(PackNotExist, p, tab_pack.packs[tab_pack.last].line);
	}
}
	

return result;
}

// LISTE_DECL : DECL LISTE_DECLAUX
bool _liste_decl(){
	bool result;
	printf("liste_decl()\n");
	if (_decl()){
		token = _lire_token();
		result = _liste_decl_aux();
	}else result = false;
	printf("out of liste_decl()\n");
	return result;
}

// LISTE_DECLAUX : LISTE_DECL | '.' ==> LISTE_DECLAUX : LISTE_DECL | epsilon
// NULLABLE(LISTE_DECLAUX) = true
// follow(LISTE_DECLAUX) = {begin}
// first(LISTE_DECLAUX) = {idf}
bool _liste_decl_aux(){
	bool result;
	if (debug) printf("liste_decl_aux()\n");
	if (_liste_decl()||token==BEG_IN){
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
			}else result = false;
		}else result = false;
	}else result = false;
	return result;
}
//
bool _inst(){
	bool result;
	if(_case()){
		printf("CCC\n");
		result=true;
	}else if (loop_statement()){
		result=true;
	}else if(_put()){
		printf("CCC\n");

		result=true;
	}else if(_put_line()){
		result=true;
	}else if(_if_instruction()){
		result=true;
	}else result=false;

return result;
}

bool  _list_inst(){

	bool result=false;
	if(_inst()){
		printf("CCC\n");
		result = true;
		if(_list_inst_aux()||token==END){
			result = true;
		}else result=false;
	}else result=false;
	return result;
 }

bool _list_inst_aux(){

	bool result=false;
	if(_inst()){
		result = true;
	}else result=false;
	return result;
 }
// IF_INSTRUCTION : if exp CONDITION_DE_TEST exp LIST_INST IF_INSTRUCTION_AUX ;
bool _if_instruction(){
	bool result = false;
	if (token == IF){
		token = _lire_token();
			if (_condition(token)){
				token = _lire_token();
					if (token == THEN){
						token = _lire_token();
						if (_list_inst()){
							token = _lire_token();
							if (_if_instruction_aux()){
								result = true;
							}
						}
					}
			}			
	}

	return result;
}


// IF_INSTRUCTION_AUX : else LIST_INST endif | endif;
bool _if_instruction_aux(){
	bool result = false;
	if (token == ELSE){
		token = _lire_token();
		if (_list_inst()){
			token = _lire_token();
			if(token == ENDIF){
				token = _lire_token();
				if(token == PVIRG){
					result = true;
				}
			}
		}
	}
	else if(token == ENDIF){
		token = _lire_token();
		if(token == PVIRG){
			result = true;
		}
	}
	return result;
}

//fonction put
bool _put(){
	bool result = false;
	if (token == PRINT){
		token = _lire_token();
		if (token == POPEN){
			token = _lire_token();
			if (token == IDF || token == DNUMBER || token == INUMBER || token == CHARACTER || token == SSTRING){
				token = _lire_token();
				if (token == PCLOSE){
					token = _lire_token();
					if (token == PVIRG){
						result = true;
					}
				}
			}
		}
	}
	return result;
}

//fonction put_line
bool _put_line(){
	bool result = false;
	if (token == PRINTLINE){
		token = _lire_token();
		if (token == POPEN){
			token = _lire_token();
			if (token == IDF || token == DNUMBER || token == INUMBER || token == CHARACTER || token == SSTRING){
				token = _lire_token();
				if (token == PCLOSE){
					token = _lire_token();
					if (token == PVIRG){
						result = true;
					}
				}
			}
		}
	}
	return result;
}
bool _case(){
	bool result;
	char *name= (char*)malloc(1024);
	//char *type= (char*)malloc(1024);
	int line;

	//int value;
	if (token == CASE){
		printf("CCC\n");
		result=true;
		token=_lire_token();
		if (token == IDF){
			printf("CCC\n");
			strcpy(name,yytext);
			//value = yylval;
			line=yylineno;
			printf("%s",type_var(name)) ;
			if (!in_tab_symb(name)){
				creer_sm_erreur(NotDeclared, name, line);
				
			}else
				{printf("%s",type_var(name)) ;
				if(!strcmp(type_var(name),"int")){
				creer_sm_erreur(IncompatibleForCase, name, line);}
			}
			result=true;
			token=_lire_token();
			if(_case_body()){
				result=true;
				token=_lire_token();
				if(token == WHEN){
					printf("CCC\n");
					result=true;
					token=_lire_token();
					if (token == OTHERS){
						result=true;
						token=_lire_token();
						if (token == ARROW){
							result=true;
							token=_lire_token();
							if(_list_inst()){
								result=true;
								token=_lire_token();
								if (token == PVIRG){
									result=true;
									token=_lire_token();
									if (token == ENCASE){
										result=true;
									}else {result =false;}
								}else {result =false;}
							}else {result =false;}
						}else {result =false;}
					}else {result =false;}
				}else {result =false;}
			}else {result =false;}
		}else {result =false;}
	}else {result =false;}
	return result;
} 

//CASE_BODY : when inumber => LIST_INST ;  CASE_BODYAUX
bool _case_body(){
	bool result;
	if (token == WHEN){
		result=true;
		token=_lire_token();
		if (token == INUMBER){
			result=true;
			token=_lire_token();
			if (token == ARROW){
				result=true;
				token=_lire_token();
				if(_list_inst()){
					result=true;
					token=_lire_token();
					if(token == PVIRG){
						result=true;
						token=_lire_token();
						if(_case_body_aux()){
							result=true;
						}else {result =false;}
					}else {result =false;}
				}else {result =false;}
			}else {result =false;}
		}else {result =false;}
	}else {result =false;}
	return result;
}



// CASE_BODYAUX : CASE_BODY | epsilon

bool _case_body_aux(){
	bool result;
	if(_case_body()){
		result=true;
	}

	if(token == WHEN){
		result=true;
		token=_lire_token();
		if(token == OTHERS){
			result=true;
			token=_lire_token();
			if(token == ARROW){
				result=true;
				token=_lire_token();
				if(_list_inst()){
					result=true;
					token=_lire_token();
					if(token == PVIRG){
						result=true;
						token=_lire_token();
					}else {result =false;}
				}else {result =false;}
			}else {result =false;}			
		}else {result =false;}
	}else {result =false;}
	return result;
}

//loop_statement ::= iteration_scheme  loop sequence_of_statements end loop ;
bool loop_statement(){
	bool result;
			if (iteration_scheme()){
				result=true;
				token=_lire_token();
				if (token == LOOP){
					result=true;
					token=_lire_token();
					if (_list_inst()){
						result=true;
						token=_lire_token();
						if (token == END){
							result=true;
							token=_lire_token();
							if (token == LOOP ){
								result=true;
								token=_lire_token();
									if (token == PVIRG ){
										result=true;
									}else {result =false;}
								}else {result =false;}
							}else {result =false;}
						}else {result =false;}
					}else {result =false;}
			}	else {result =false;}			
	return result;
}

//iteration_scheme ::= while condition | for loop_parameter_specification
bool iteration_scheme(){
	bool result;
	if (token == WHILE){
		result=true;
		token=_lire_token();
		if (_condition(token)){
			result=true;
		}else {result =false;}	
	}else if (token == FOR){
		result=true;
		token=_lire_token();
		if (_loop_parameter_specification()){
			result=true;

		}else {result =false;}
	}else {result =false;}
	return result;
}

//loop_parameter_specification ::= identifier in discrete_interval
bool _loop_parameter_specification(){
	bool result;
	if (token == IDF){
		result=true;
		token=_lire_token();
		if(token == IN){
			result=true;
			token=_lire_token();
			if(_discrete_interval()){
				result=true;
			}else {result =false;}
		}else {result =false;}
	}else {result =false;}
	return result;
}

bool _discrete_interval(){
	bool result;
	char * name =(char*) malloc(1024);
	int line;
	int inf,sup;
	if (token == INUMBER){
		inf = atoi(yytext);
		result=true;
		token=_lire_token();
		if (token == TWODOTS){
			result=true;
			token=_lire_token();
			if (token == INUMBER){
				sup = atoi(yytext);
				result=true;
				if(inf > sup){
					line=yylineno;
					strcpy(name,"erreur");
					creer_sm_erreur(IntervalError,name,line);
				}
			}else {result =false;}
		}else {result =false;}
	}else {result =false;}
	return result;
}

/*bool _condition(){
	bool result;
	if (token == IDF){
		result=true;
		token=_lire_token();
		if(token == SUP || token == INF || token == EQEQ ){
			result=true;
			token=_lire_token();
			if (token == INUMBER){
				result=true;
		    }else result =false;
	    }else result =false;
	 }else result =false;
	return result;
}*/
//CONDITION_DE_TEST
bool _condition(typetoken t){
	bool result = false;

	if (token == IDF || _const()){
		token = _lire_token();
		if(t == NOTEQ || t == EQEQ || t == INFEQ || t == INF || t == SUPEQ || t == SUP){
			token = _lire_token();
			if (token == IDF || _const()){
				result = true;
			}
		}
	}

	return result;	
}

bool _addsub(){
   bool result;

	if (_multdiv()){
		token = _lire_token();
		result = _addsubaux();
	}else result = false;
   
   return result;
}


bool _addsubaux(){
   bool result;
   
	if ( (token == PVIRG) || (token == PCLOSE) ){
		result = true;
	}else if (token == PLUS) {
			token = _lire_token();
			result = _addsub();
	} else if (token == MINUS) {
			token = _lire_token();
			result = _addsub();
	} else 
	result = false;
	
}

bool _multdiv(){
	bool result;
	if(_aux()){
		result=true;
		token=_lire_token();
		if(_multdivaux()){
			result=true;
		}else {result =false;}
	}else {result =false;}
	return result;
}

bool _multdivaux(){
	bool result=false;
	if ( (token == PLUS) || (token == MINUS) || (token == PVIRG) || (token == PCLOSE) ){
		result=true;
		token=_lire_token();
	}else if(token==MULT){
		result=true;
		token=_lire_token();
		if(_multdiv()){
			result=true;
		}else {result =false;}
	}else {	if(token==DIV){
		result=true;
		token=_lire_token();
		if(_multdiv()){
			result=true;
		}else {result =false;}
	}
	return result;
}
}

bool _aux(){
	bool result=false;
	if ( token == INUMBER ){
		result=true;
		token=_lire_token();
		} else if ( token == DNUMBER ){
			result=true;
			token=_lire_token();
		} else if ( token == IDF ){
			result=true;
			token=_lire_token();
		}else if ( token == POPEN){
			result=true;
			token=_lire_token();
			if (_addsub()){
				result=true;
				token=_lire_token();
				if ( token == PCLOSE){
					result=true;

				}else {result =false;}
			}else {result =false;}
		}else {result =false;}
	
	return result;

}

bool _prog(){
	bool result = false;
	
	if(_package()){
				//printf("%s",yytext);
		//token=_lire_token();
		result=true;
		if(_decl_proc()){
				//printf("%s",yytext);
			result=true;
			token=_lire_token();
			if(_liste_decl()){
				//printf("%s",yytext);
				result=true;
				//token=_lire_token();
				if (token == BEG_IN){
				//printf("%s",yytext);
					result = true;
					token=_lire_token();
					if(_list_inst()){
						printf("%s",yytext);
						result=true;
						//token=_lire_token();
										printf("%s",yytext);

						if (token == END){
							result=true;
						}else result=false;
					}else result=false;
				}else result=false;
			} else {result=false;}
		}else {result=false;}
	}else result=false;
	return result;
}
int main(){
	creer_file_erreur();
  	init_tab_sym();
	token=_lire_token();
	if(_prog())
		printf("ok \n");
	else
		{printf("not ok \n");
		afficher_sm_erreurs();
		}
	
	return 0;
}
