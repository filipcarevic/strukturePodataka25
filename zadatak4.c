#include <stdio.h>
#include <stdlib.h>

typedef struct term
{
    int coef;
    int exp;
    struct term *next;
}term;

term *createNewTerm(int coef, int exp);
void insertSorted(term **head, term *newTerm);
void printPolynomial(term *head);
term *readFromFile(char *fileName);
term *addPolynomials(term *p1, term *p2);
term *multiplyPolynomials(term *p1, term *p2);
void freeList(term *head);

int main()
{
    term *p1 = readFromFile("poly1.txt");
    term *p2 = readFromFile("poly2.txt");

    printf("Prvi polinom:\n");
    printPolynomial(p1);
    printf("\nDrugi polinom:\n");
    printPolynomial(p2);

    term *sum = addPolynomials(p1, p2);
    term *product = multiplyPolynomials(p1, p2);

    printf("\nZbroj polinoma:\n");
    printPolynomial(sum);
    printf("\nUmnozak polinoma:\n");
    printPolynomial(product);

    freeList(p1);
    freeList(p2);
    freeList(sum);
    freeList(product);

    return 0;
}

term *createNewTerm(int coef, int exp)
{
    term *newTerm = (term*)malloc(sizeof(term));
    newTerm->coef = coef;
    newTerm->exp = exp;
    newTerm->next = NULL;
    return newTerm;
}

void insertSorted(term **head, term *newTerm)
{
    // ubacuje po eksponentu od najvećeg prema najmanjem
    if(*head == NULL || (*head)->exp < newTerm->exp)
    {
        newTerm->next = *head;
        *head = newTerm;
        return;
    }

    term *temp = *head;
    while(temp->next != NULL && temp->next->exp > newTerm->exp)
    {
        temp = temp->next;
    }

    if(temp->next != NULL && temp->next->exp == newTerm->exp)
    {
        temp->next->coef += newTerm->coef;
        free(newTerm);
    }
    else
    {
        newTerm->next = temp->next;
        temp->next = newTerm;
    }
}

void printPolynomial(term *head)
{
    term *temp = head;
    while(temp != NULL)
    {
        printf("%dx^%d ", temp->coef, temp->exp);
        if(temp->next != NULL && temp->next->coef > 0) printf("+ ");
        temp = temp->next;
    }
    printf("\n");
}

term *readFromFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if(file == NULL)
    {
        printf("Greska u otvaranju filea!\n");
        exit(1);
    }

    int coef, exp;
    term *head = NULL;

    while(fscanf(file, "%d %d", &coef, &exp) == 2)
    {
        term *newTerm = createNewTerm(coef, exp);
        insertSorted(&head, newTerm);
    }

    fclose(file);
    return head;
}

term *addPolynomials(term *p1, term *p2)
{
    term *result = NULL;

    while(p1 != NULL)
    {
        insertSorted(&result, createNewTerm(p1->coef, p1->exp));
        p1 = p1->next;
    }

    while(p2 != NULL)
    {
        insertSorted(&result, createNewTerm(p2->coef, p2->exp));
        p2 = p2->next;
    }

    return result;
}

term *multiplyPolynomials(term *p1, term *p2)
{
    term *result = NULL;

    for(term *i = p1; i != NULL; i = i->next)
    {
        for(term *j = p2; j != NULL; j = j->next)
        {
            int coef = i->coef * j->coef;
            int exp = i->exp + j->exp;
            insertSorted(&result, createNewTerm(coef, exp));
        }
    }

    return result;
}

void freeList(term *head)
{
    term *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
