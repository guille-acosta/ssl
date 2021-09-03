#include "scanner.h"

int main(void){
    Tokens *tokensFound = NULL;
    
    get_tokens( &tokensFound );
    print_tokens( tokensFound );
    clean_memory( &tokensFound );

    return EXIT_SUCCESS;
}