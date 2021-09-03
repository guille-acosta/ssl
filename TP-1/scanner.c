#include "scanner.h"

extern int errno;
int errorNum = 0;

/*---------------------------------------------------------------------------------------
    get_tokens: recibe como parametro un puntero a puntero del tipo Tokens al cual   
    asocia una lista simplemente enlazada con los tokens/lexemas econtrados.
----------------------------------------------------------------------------------------*/
void get_tokens(Tokens **tokensFound){
    Letters *string = NULL;
    Letters *comma = NULL;
    Letters *endOfInput = NULL;

    State wordState = OUT; // indica si se esta ante una palabra/string, en caso de recibir
                           // un caracter distinto a un espacio o coma pasa a ser IN
    TokenType token = FDT;
    int letter;

    while( letter = getchar() ){
        if( isspace(letter) ){
            if( wordState == IN ){
                insert_token( tokensFound, CAD, string );
                string = NULL;
            }
            wordState = OUT;
        }
        else{ 
            if( wordState == OUT ){
            	if( letter == EOF ){
            		form_lexeme( &endOfInput,  END );
            		insert_token( tokensFound, FDT, endOfInput );
            		break;
            	}
                if( letter == ','){
                    comma = NULL;
                    form_lexeme( &comma, SEPARATOR );
                    insert_token( tokensFound, SEP, comma );
                }
                else {
                    wordState = IN;
                    form_lexeme( &string, letter);
                }    
            }
            else if( wordState == IN ){
            	if( letter == EOF ){
            		insert_token( tokensFound, CAD, string);
            		form_lexeme( &endOfInput,  END );
            		insert_token( tokensFound, FDT, endOfInput );
            		break;
            	}
                if( letter == ',' ){
                    wordState = OUT;
                    insert_token( tokensFound, CAD, string);
                    string = NULL;

                    comma = NULL;
                    form_lexeme( &comma, SEPARATOR );
                    insert_token( tokensFound, SEP, comma);
                }
                else {
                    form_lexeme( &string, letter );
                }
            }
        }
    }

}


/*---------------------------------------------------------------------------------------
    form_lexeme: recibe un puntero a puntero al cual se asocia una lista simplemente 
    enlazada donde se van guardando el/los caracter/res del lexema que se esta proce_
    zando actualmente.
----------------------------------------------------------------------------------------*/

void form_lexeme(Letters **word, int letter){
    Letters *lastLetter = (Letters *) malloc( sizeof(Letters) );

    if( lastLetter != NULL ){

        lastLetter -> oneLetter = letter;
        lastLetter -> nextLetter = NULL;

        if( *word == NULL ){
            *word = lastLetter;
        }
        else{
            Letters *aux = *word;

            while( aux -> nextLetter != NULL ){
                aux = aux -> nextLetter;
            }
            aux -> nextLetter = lastLetter;
        }
    }
    else{
        errorNum = errno;
        printf( "strerror(%d) = %s\n", errorNum, strerror( errorNum ) );
        exit( EXIT_FAILURE );
    }
}


/*---------------------------------------------------------------------------------------
    insert_token: recibe la referencia a un puntero del tipo Tokens que apunta a una 
    lista simplemente enlazada donde la función inserta un nodo el cual que contiene  
    el token(tokenValue) y su lexema correspondiente(lexemeValue).
----------------------------------------------------------------------------------------*/
void insert_token(Tokens **tokensList, TokenType tokenValue, Letters *lexemeValue){
    Tokens *lastToken = (Tokens *) malloc( sizeof( Tokens ) );

    if( lastToken != NULL ){

        lastToken -> token = tokenValue;
        lastToken -> lexeme = lexemeValue;
        lastToken -> nextToken = NULL;

        if( *tokensList == NULL ){
            *tokensList = lastToken;
        }
        else{
            Tokens *aux = *tokensList;

            while( aux -> nextToken != NULL ){
                aux = aux -> nextToken;
            }
            aux -> nextToken = lastToken;
        }
    }
    else{
        errorNum = errno;
        printf( "strerror(%d) = %s\n", errorNum, strerror( errorNum ) );
        exit( EXIT_FAILURE );
    }
}


/*---------------------------------------------------------------------------------------
    print_tokens: recibe un puntero del tipo Tokens que apunta a una lista simplemente
    enlazada que contiene los pares token/lexema que la función se encarga de imprimir.
----------------------------------------------------------------------------------------*/
void print_tokens( Tokens *tokensList ){
    Tokens *aux = tokensList;
    
    printf("===================================\n");
    
    while( aux  != NULL ){
        switch( aux -> token ){
            case CAD:
                printf("\tCadena: ");
                print_lexeme( aux -> lexeme );
                printf("\n");
                break;
            
            case SEP:
                printf("\tSeparador: ");
                print_lexeme( aux -> lexeme );
                printf("\n");
                break;

            default:
                printf("\tFin de Texto:\n");
        }   

        aux = aux -> nextToken;
    }

    printf("===================================\n");
}


/*---------------------------------------------------------------------------------------
    print_lexeme: función auxiliar a print_tokens que recibe un puntero del tipo
    Letters con los caracteres que conforman el lexema que la funcion se encarga de
    imprimir.
----------------------------------------------------------------------------------------*/
void print_lexeme( Letters *lexeme ){
    Letters *aux = lexeme;

    while( aux != NULL ){
        printf("%c", (char) aux -> oneLetter );
        aux = aux -> nextLetter;
    }
}


/*---------------------------------------------------------------------------------------
    clean_memory: la función se encarga de liberar la memoria(solicitada via malloc) 
    asociada a la lista de tokens/lexemas referenciada por el parámetro tokensList.
----------------------------------------------------------------------------------------*/
void clean_memory( Tokens **tokensList ){
    Tokens *aux = NULL;

    while( *tokensList != NULL ){
        aux = *tokensList;
        *tokensList = (*tokensList) -> nextToken;

        remove_lexeme(&(aux -> lexeme));
        free(aux);  
    }
}


/*---------------------------------------------------------------------------------------
    remove_lexeme: funcion auxiliar a clean_memory, la función se encarga de liberar la  
    memoria(solicitada via malloc)asociada a la lista con los caracteres del lexema 
    referenciado por el parámetro lexeme.
----------------------------------------------------------------------------------------*/
void remove_lexeme( Letters **lexeme ){
    Letters *aux = NULL;
   
    while( *lexeme != NULL ){
        aux = *lexeme;
        *lexeme = ( *lexeme ) -> nextLetter;

        free(aux);
    }
}