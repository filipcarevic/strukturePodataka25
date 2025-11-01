
/*Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa rezultat. Stog je potrebno realizirati preko vezane liste.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// DEFINICIJA STOGA PREKO VEZANE LISTE
typedef struct stackThing {
    double number;              // broj koji spremamo
    struct stackThing* next;    // pokazivač na sljedeći element stoga
} stackThing;

// KREIRANJE NOVOG ELEMENTA
stackThing* makeNewThing(double n) {
    stackThing* t = (stackThing*)malloc(sizeof(stackThing));
    if(t == NULL) return NULL;
    t->number = n;
    t->next = NULL;
    return t;
}

// STAVI ELEMENT NA VRH STOGA
int putOnTop(stackThing** top, double n) {
    stackThing* t = makeNewThing(n);
    if(t == NULL) return EXIT_FAILURE;
    t->next = *top;
    *top = t;
    return EXIT_SUCCESS;
}

// UZMI ELEMENT SA VRHA STOGA
int takeFromTop(stackThing** top, double* out) {
    if(*top == NULL) {
        printf("Stog je prazan!\n");
        return EXIT_FAILURE;
    }
    stackThing* temp = *top;
    *out = temp->number;
    *top = temp->next;
    free(temp);
    return EXIT_SUCCESS;
}

// PROVJERA JE LI ZNAK OPERATOR
int isSign(char c) {
    return c=='+' || c=='-' || c=='*' || c=='/';
}

// OSLABLJENA EVALUACIJA POSTFIKS IZRAZA
int doMath(char* expr, double* result) {
    stackThing* myStack = NULL;
    char* tok = strtok(expr, " ");

    while(tok != NULL) {
        if(isdigit(tok[0]) || (tok[0]=='-' && isdigit(tok[1]))) {
            if(putOnTop(&myStack, atof(tok)) == EXIT_FAILURE) return EXIT_FAILURE;
        } else if(isSign(tok[0])) {
            double b, a;
            if(takeFromTop(&myStack, &b) == EXIT_FAILURE) return EXIT_FAILURE;
            if(takeFromTop(&myStack, &a) == EXIT_FAILURE) return EXIT_FAILURE;

            switch(tok[0]) {
                case '+': if(putOnTop(&myStack, a+b) == EXIT_FAILURE) return EXIT_FAILURE; break;
                case '-': if(putOnTop(&myStack, a-b) == EXIT_FAILURE) return EXIT_FAILURE; break;
                case '*': if(putOnTop(&myStack, a*b) == EXIT_FAILURE) return EXIT_FAILURE; break;
                case '/': if(putOnTop(&myStack, a/b) == EXIT_FAILURE) return EXIT_FAILURE; break;
            }
        } else {
            printf("Nepoznat token: %s\n", tok);
        }
        tok = strtok(NULL, " ");
    }

    if(takeFromTop(&myStack, result) == EXIT_FAILURE) return EXIT_FAILURE;

    // OSLOBODI STOG AKO JE OSTALO NEŠTO
    while(myStack != NULL) {
        stackThing* temp = myStack;
        myStack = myStack->next;
        free(temp);
    }

    return EXIT_SUCCESS;
}

int main() {
    char line[256];
    FILE* f = fopen("postfiks.txt", "r");
    if(f == NULL) {
        printf("Ne mogu otvoriti datoteku.\n");
        return EXIT_FAILURE;
    }

    while(fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        double res;
        if(doMath(line, &res) == EXIT_SUCCESS) {
            printf("Rezultat \"%s\" = %.2f\n", line, res);
        } else {
            printf("Greska pri evaluaciji izraza \"%s\"\n", line);
        }
    }

    fclose(f);
    return EXIT_SUCCESS;
}
