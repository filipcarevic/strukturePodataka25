//kod na engleskom jeziku
//gdje god ima smisla koristit funkciju treba je napavit i koristit
//pise se u C jeziku 
//prvo incializirat funkciju, prije int maina, ali joj ne dat funkcionlanost do tek nakon int maina
//ne koristit FILE* kao argument funkcije 
//za return u fje. se moze stavit return EXIT_FAILOURE i ima return SUCCESS

/*
Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji: 
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi(po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
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
int findElement(person*head, char*surname);
int deleteElement(person**head, char*surname);
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

    prev->next = temp->next; // doslovno skippa trenutni elent i na taj nacin lista ostane uredna 

    free(temp); // brise preskoceni element
    return EXIT_SUCCESS;
}

int findElement(person*head, char*surname)
{
    person *temp = head;
    while (temp != NULL && strcmp(temp->surname, surname) != 0)
    {
        temp = temp->next;
    }

    // ako element nije pronađen
    if (temp == NULL) printf("taj element ne postoji");
    else printf("%s %s %d\n", temp->name, temp->surname, temp->birthYear);
    return EXIT_SUCCESS;
}

int addToStart(person**head, person*current)
{
    current->next = *head; // novi cvor pokazuje na stari početak
    *head = current; // head sad pokazuje na novi pocetk sta je novi cvor na prvom mjestu
    return EXIT_SUCCESS;
}


//tjelo koda: odabir opcija i poziv funkcija
int main()
{
    person* head = NULL;
    char name[30], surname[30];
    int age;

    //spremanje elemenata iz datoteke u listu
    FILE* file = fopen("studenti.txt", "r");
    while (fscanf(file, "%s %s %d", name, surname, &age) == 3) 
    {   
        person* current = createNewPerson(name, surname, age);
        addToEnd(&head, current);    
    }

    fclose(file);

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
                findElement(head, surname);
                break;
            }
            case 'E':
            {
                printf("unesi prezima: ");
                scanf(" %s", surname);
                deleteElement(&head, surname);
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
