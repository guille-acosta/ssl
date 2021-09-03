#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#define SEPARATOR ','
#define END ' '


typedef struct node{
    int oneLetter;
    struct node *nextLetter;
} Letters;


typedef enum {OUT, IN} State;
typedef enum {FDT, SEP, CAD} TokenType;

typedef struct TokenLex{
    TokenType token;
    Letters *lexeme;
    struct TokenLex *nextToken;

} Tokens;

void get_tokens( Tokens ** );
void form_lexeme(Letters **, int );
void insert_token(Tokens **, TokenType , Letters *);
void clean_memory( Tokens ** );
void print_tokens (Tokens * );
void print_lexeme( Letters * );
void remove_lexeme( Letters ** );


#endif
