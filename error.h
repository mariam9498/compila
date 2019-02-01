

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum
    {
        AlreadyDeclared,
        NoneInitialized,
         PackNotExist,
            NotDeclared,
        IncompatibleForCase,
        IntervalError
    } SemanticErrorType;

 
typedef struct 
    {
        char* name;
        int line;
        SemanticErrorType type;

    }smError;
//struct smError *tab;  optionnelle (error.c)

//Constructeur
void creer_file_erreur(); //cree la structure FIFO globale
void creer_sm_erreur(SemanticErrorType er, char* name, int line); //cree et ajoute une erreur semantique a la structure FIFO

//Getter
int nombre_sm_erreurs(); //renvoie le nombre d'erreurs
void afficher_sm_erreurs(); //affiche les erreurs

