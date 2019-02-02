#include "error.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//static int nb;
typedef struct {
  smError errors[100];
  int last;
}erreur;
erreur queue;

void creer_file_erreur()
{
    queue.last = -1;
}

void afficher_sm_erreurs()
{
    if (queue.last+1==0) printf("\t \t \t Vous n'avez AUCUNE erreur semantique !\n\n");
    else{
    
        printf("\t \t \t Vous avez %d erreurs semantiques !\n\n", queue.last + 1);
        for (int i = 0; i <= queue.last; i++) {
        printf("\t \t Ligne %d: %s ", queue.errors[i].line, queue.errors[i].name);
        switch (queue.errors[i].type) {
        case AlreadyDeclared:
            puts("est deja declaree\n");
            break;
        case NoneInitialized:
            puts("est mal initialisee\n");
            break;
        case PackNotExist:
            puts("package incorrecte\n");
            break;
        case NotDeclared:
            puts(": variable non déclarée\n");
            break;
        case IncompatibleForCase:
            puts("non compatible avec case\n");
            break;
	case IncompatibleForCondition:
            puts("Comparaison de deux types incompatible\n");
            break;
        case IntervalError:
            puts("intervalle non valide\n");
            break;
	case IncompatibleForInterval:
            puts("non compatible avec l'intervalle\n");
            break;
        
        }
       
        }
        }
    }      

int nombre_sm_erreurs()
{
    return queue.last + 1;
}

void creer_sm_erreur(SemanticErrorType er, char* name, int line){
	queue.last++;
  queue.errors[queue.last].type = er;
  queue.errors[queue.last].line = line;
  queue.errors[queue.last].name = (char *) malloc((strlen(name) + 1) * sizeof(char));
  strcpy(queue.errors[queue.last].name, name);
}


