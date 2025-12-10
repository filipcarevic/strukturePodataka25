#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Article {
    char name[100];
    int qty;
    float price;
    struct Article *next;
} Article;

typedef struct Bill {
    char date[11]; 
    Article *articles;
    struct Bill *next;
} Bill;

/* umetanje artikla sortirano po nazivu */
void insertArticleSorted(Article **head, Article *newA) {
    if (*head == NULL || strcmp(newA->name, (*head)->name) < 0) {
        newA->next = *head;
        *head = newA;
        return;
    }
    Article *temp = *head;
    while (temp->next && strcmp(newA->name, temp->next->name) > 0)
        temp = temp->next;
    newA->next = temp->next;
    temp->next = newA;
}

/* umetanje računa sortirano po datumu */
void insertBillSorted(Bill **head, Bill *newB) {
    if (*head == NULL || strcmp(newB->date, (*head)->date) < 0) {
        newB->next = *head;
        *head = newB;
        return;
    }
    Bill *temp = *head;
    while (temp->next && strcmp(newB->date, temp->next->date) > 0)
        temp = temp->next;
    newB->next = temp->next;
    temp->next = newB;
}

/* učitavanje pojedine datoteke računa */
Bill* loadSingleBill(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    Bill *b = malloc(sizeof(Bill));
    b->articles = NULL;
    b->next = NULL;

    fgets(b->date, 11, f);

    char name[100];
    int qty;
    float price;

    /* čitanje artikala */
    while (fscanf(f, "%[^,], %d, %f\n", name, &qty, &price) == 3) {
        Article *a = malloc(sizeof(Article));
        strcpy(a->name, name);
        a->qty = qty;
        a->price = price;
        a->next = NULL;
        insertArticleSorted(&b->articles, a);
    }

    fclose(f);
    return b;
}

/* učitavanje svih računa iz racuni.txt */
Bill* loadBills() {
    FILE *f = fopen("racuni.txt", "r");
    if (!f) return NULL;

    Bill *head = NULL;
    char filename[100];

    while (fscanf(f, "%s", filename) == 1) {
        Bill *b = loadSingleBill(filename);
        if (b) insertBillSorted(&head, b);
    }
    fclose(f);
    return head;
}

/* izračun troška i količine za artikl u intervalu */
void query(Bill *head, char *target, char *dateFrom, char *dateTo) {
    float totalMoney = 0.0;
    int totalQty = 0;

    Bill *b = head;
    while (b) {
        if (strcmp(b->date, dateFrom) >= 0 && strcmp(b->date, dateTo) <= 0) {
            Article *a = b->articles;
            while (a) {
                if (strcmp(a->name, target) == 0) {
                    totalMoney += a->qty * a->price;
                    totalQty += a->qty;
                }
                a = a->next;
            }
        }
        b = b->next;
    }

    printf("Ukupno potroseno za '%s': %.2f\n", target, totalMoney);
    printf("Ukupno kupljena kolicina: %d\n", totalQty);
}

int main() {
    Bill *bills = loadBills();
    if (!bills) {
        printf("Greska pri ucitavanju.\n");
        return 1;
    }

    char item[100], date1[11], date2[11];

    /* korisnicki unos */
    printf("Artikl: ");
    scanf("%s", item);
    printf("Datum od (YYYY-MM-DD): ");
    scanf("%s", date1);
    printf("Datum do (YYYY-MM-DD): ");
    scanf("%s", date2);

    query(bills, item, date1, date2);

    return 0;
}
