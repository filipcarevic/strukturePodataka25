/*
Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji: 
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi(po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.

Prethodnom zadatku dodati funkcije:
F. dinamički dodaje novi element iza određenog elementa,
G. dinamički dodaje novi element ispred određenog elementa, C. sortira listu po prezimenima osoba,
H. upisuje listu u datoteku,
J. čita listu iz datoteke.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//denijiranje stukture osoba koa listu
typedef struct person
{
    char name[30];
    char surname[30];
    int birthYear;
    struct person* next; // to je samo pokazivac na sljedeci cvor u listi, nije globalna variabla
} person;

//kalup koda
person* createNewPerson(const char*name, const char*surname, int birthYear); //fje. za stvaranje novog cvora bez da se uneseni string mjenja
int addToStart(person**head, person*current);
int printList(person*head);
int addToEnd(person**head, person*current);
person* findElement(person* head, char* surname);
int deleteElement(person**head, char*surname);
int addElementAfter(person**head, char*surname, person *current);
int addElementBefore(person**head, char*surname, person *current);
int addToFile(person*head, char*fileName);
int readFile(person** head, char* fileName);
/*
person**head se koristi kad zelimo promjenit vrijesnodt headai njegovu adresu. A person* head kad zelimo promjenit
samo kopiju tog heada, nije stalna promjena.
*/


//mozak funkcija/coda
person* createNewPerson(const char*name, const char*surname, int birthYear)
{
    person* current = (person*)malloc(sizeof(person));

    strcpy(current->name, name); // -> sluzi kao pokazivac na strukturu
    strcpy(current->surname, surname);
    current->birthYear = birthYear;
    current->next = NULL; // pokazivac pokazuje na novi element 
    return current;
}

int addToEnd(person**head, person*current)
{
    if(*head==NULL)
    {
        *head = current; // ako je lista prazna, novi elemtn je na prvom(jedinom) mjestu
        return EXIT_SUCCESS;
    }

    person *temp = *head; // sluzi da prodjemo kroz listu bez da diramo stvarni head 
    while(temp->next != NULL) // Dok pokazivač next iz trenutnog čvora (temp) nije prazan (NULL), idi dalje
    {
        temp = temp->next;
    }
    temp->next = current; // zato sta petlja jos pokazuje na zadnji elemnt, u ovom slucaju je to 3. element
    return EXIT_SUCCESS;
}

int printList(person*head)
{
    person *temp = head;
    while(temp != NULL) // ne dok postoji iduci element, nego dok trenutni element nije nepostojec
    {
        printf("%s %s %d\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int deleteElement(person**head, char*surname)
{
    person *prev = NULL;
    person *temp = *head;
    while (temp != NULL && strcmp(temp->surname, surname) != 0)
    {
        prev = temp;
        temp = temp->next;
    }

    // ako element nije pronađen
    if (temp == NULL)
    {
        printf("taj element ne postoji");
        return EXIT_FAILURE;
    }

    if (prev == NULL) *head = temp->next;
    else prev->next = temp->next;  // doslovno skippa trenutni elent i na taj nacin lista ostane uredna 

    free(temp); // brise preskoceni element
    return EXIT_SUCCESS;
}

person* findElement(person* head, char* surname)
{
    person *temp = head;
    while (temp != NULL && strcmp(temp->surname, surname) != 0)
    {
        temp = temp->next;
    }

    // ako element nije pronađen
    if (temp == NULL) 
    {
        printf("taj element ne postoji");
        return NULL;
    }
    else printf("%s %s %d\n", temp->name, temp->surname, temp->birthYear);
    return temp;
}

int addToStart(person**head, person*current)
{
    current->next = *head; // novi cvor pokazuje na stari početak
    *head = current; // head sad pokazuje na novi pocetk sta je novi cvor na prvom mjestu
    return EXIT_SUCCESS;
}

int addToFile(person*head, char*fileName)
{
    person *temp = head;
    FILE* file = fopen(fileName, "w");

    if(file==NULL)
    {
        printf("datoteka se ne moze otvorit");
        return EXIT_FAILURE;
    }

    while(temp != NULL)
    {
        fprintf(file, "%s %s %d\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }
    fclose(file);
    return EXIT_SUCCESS;
}

int readFile(person** head, char* fileName)
{
    char name[30], surname[30];
    int age;

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Ne mogu otvoriti datoteku.\n");
        return EXIT_FAILURE;
    }

    while (fscanf(file, "%s %s %d", name, surname, &age) == 3)
    {
        person* current = createNewPerson(name, surname, age);
        addToEnd(head, current);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

int addElementAfter(person**head, char*surname, person *current)
{
    person *temp = *head;
    while(temp != NULL && strcmp(temp->surname, surname) != 0)
    {
        temp = temp->next; 
    }

    if(temp==NULL)
    {
        printf("taj elemetn ne postoji");
        return EXIT_FAILURE;
    }
    current->next = temp->next;
    temp->next = current;

    return EXIT_SUCCESS;
}

int addElementBefore(person**head, char*surname, person *current)
{
    person *temp = *head;
    person* prev = NULL;

    while(temp != NULL && strcmp(temp->surname, surname) != 0)
    {
        prev = temp;
        temp = temp->next; 
    }

    if(temp==NULL)
    {
        printf("taj elemetn ne postoji");
        return EXIT_FAILURE;
    }

    prev->next = current;
    current->next = temp;

    return EXIT_SUCCESS;
}


//tjelo koda: odabir opcija i poziv funkcija
int main()
{
    person* head = NULL;
    char name[30], surname[30];
    int age;
    
    char fileName[15] = "studenti.txt";

    char input = 'T';
    while(toupper(input) != 'F')
    {
        printf("T ili F: ");
        scanf(" %c", &input);

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if(toupper(input) == 'F') break;

        char function;

        printf("Odaberi izmedju:\n");
        printf("A. dodaje novi element na početak liste\n");
        printf("B. ispisuje listu\n");
        printf("C. dodaje novi element na kraj liste\n");
        printf("D. pronalazi element u listi (po prezimenu)\n");
        printf("E. briše određeni element iz liste\n");
        printf("F. dinamički dodaje novi element iza određenog elementa\n");
        printf("G. dinamički dodaje novi element ispred određenog elementa, C. sortira listu po prezimenima osoba\n");
        printf("H. upisuje listu u datoteku\n");
        printf("J. čita listu iz datoteke\n");
        scanf(" %c", &function);
    
        person* current = NULL;
        switch (function) 
        {
            case 'A':
            {
                printf("unesi ime prezima godinu rodenja tim redom sa razmakom izmedju svakog: ");
                scanf(" %s %s %d", name, surname, &age);
                current = createNewPerson(name, surname, age);
                addToStart(&head, current);  
                break;
            }
            case 'B':
            {
                printList(head);
                break;
            }
            case 'C':
            {
                printf("unesi ime prezima godinu rodenja tim redom sa razmakom izmedju svakog: ");
                scanf(" %s %s %d", name, surname, &age);
                current = createNewPerson(name, surname, age);
                addToEnd(&head, current);  
                break;
            }
            case 'D':
            {
                printf("unesi prezima: ");
                scanf(" %s", surname);
                person* found = findElement(head, "Peric");
                if (found != NULL) printf("Pronadjen: %s %s (%d)\n", found->name, found->surname, found->birthYear);
                break;
            }
            case 'E':
            {
                printf("unesi prezima: ");
                scanf(" %s", surname);
                deleteElement(&head, surname);
                break;
            }
            case 'F':
            {
                printf("unesi ime prezima godinu rodenja tim redom sa razmakom izmedju svakog: ");
                scanf(" %s %s %d", name, surname, &age);
                current = createNewPerson(name, surname, age);
                printf("unesi prezima: ");
                scanf(" %s", surname);
                addElementAfter(&head, surname, current);
                break;
            }
            case 'G':
            {
                printf("unesi ime prezima godinu rodenja tim redom sa razmakom izmedju svakog: ");
                scanf(" %s %s %d", name, surname, &age);
                current = createNewPerson(name, surname, age);
                printf("unesi prezima: ");
                scanf(" %s", surname);
                addElementBefore(&head, surname, current);
                break;
            }
            case 'H':
            {
                addToFile(head, fileName);
                break;
            }
            case 'J':
            {
                readFile(&head, fileName);
                break;
            }
            default:
            {
                printf("uensen je ne postojeca opcija: %c\n", function);
            }
        }    
    }  
    return 0;
} 
