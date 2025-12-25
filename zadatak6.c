#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct article{
    char name[20];
    int quantity;
    float price;
    struct article *next;
} article;

typedef struct bill{
    int year, month, day;
    article *articleHead;
    struct bill *next;
} bill;

typedef struct{
    bill *front;
    bill *rear;
} manager;

//make new  nodes for bill and for articles
bill *newBillNode(int year, int month, int day);
article *newArticleNode(char const *name, int quantity, float price);

int compareDates(bill *a, bill *b);

//make enqueue for both bill and article
int billEnqueue(manager *manager, bill *newBill);
int articleEnqueue(bill *currentBill, article *newArticle);

//function to read all file
int readFile(char *fileName, manager *manager);

//find specific article; its total price in specific time pariod as well quantity of it
int findData(char *name, char *date, manager *manager);

//function for freeing(dequeue) and printing out all elements 
void dequeue(manager *manager);

int main(){
    char *fileName = "racuni.txt";
    char name[69], date[69];

    manager m;
    m.front = NULL;
    m.rear = NULL;

    readFile(fileName, &m);

    printf("What is date you want to chek for article? Like this: YY-MM-DD! ");
    scanf("%s", date);
    printf("Name of article with capital first letter? ");
    scanf("%s", name);

    puts("\n");
    findData(name, date, &m);
    puts("\n");
    dequeue(&m);

    return EXIT_SUCCESS;
}

bill *newBillNode(int year, int month, int day){
    bill *newBill = (bill*)malloc(sizeof(bill));
    if(!newBill){
        printf("error in alocating memory for new bill! \n");
        return NULL;
    }

    newBill->year = year;
    newBill->month = month;
    newBill->day = day;
    newBill->next = NULL;
    newBill->articleHead = NULL;

    return newBill;
}

article *newArticleNode(char const *name, int quantity, float price){
    article *newArticle = (article*)malloc(sizeof(article));
    if(!newArticle){
        printf("error in alocating memory for new article! ");
        return NULL;
    }

    strcpy(newArticle->name, name);
    newArticle->quantity = quantity;
    newArticle->price = price;
    newArticle->next = NULL;

    return newArticle;
}

int compareDates(bill *a, bill *b){
        if(a->year != b->year) return a->year - b->year;
        else if(a->month != b->month) return a->month - b->month;
        return a->day - b->day;
}

int billEnqueue(manager *manager, bill *newBill){
    if(manager->front==NULL){
        manager->front = manager->rear = newBill;
        return EXIT_SUCCESS;
    }

    bill *temp = manager->front;
    bill *prev = NULL;
    
    while(temp!=NULL && compareDates(temp, newBill)>0){
        prev = temp;
        temp = temp->next;
    }

    if(prev==NULL){
        newBill->next = temp;
        manager->front = newBill;
    } else{
        prev->next = newBill;
        newBill->next = temp;

        if(temp==NULL) manager->rear = newBill;
    }

    return EXIT_SUCCESS;
}

int articleEnqueue(bill *currentBill, article *newArticle){
    if(currentBill->articleHead==NULL){
        currentBill->articleHead = newArticle;
        return EXIT_SUCCESS;
    }

    article *temp = currentBill->articleHead;
    article *prev = NULL;

    while(temp!=NULL && strcmp(temp->name, newArticle->name)<0){
        prev = temp;
        temp = temp->next;
    }

    if(prev==NULL){
        newArticle->next = temp;
        currentBill->articleHead = newArticle;
    } else{
        prev->next = newArticle;
        newArticle->next = temp;
    }

    return EXIT_SUCCESS;
}

int readFile(char *fileName, manager *manager){
    FILE *mainFile = fopen(fileName, "r");
    if(!mainFile){
        printf("error in opening of main file! \n");
        return EXIT_FAILURE;
    }

    char subFileName[69];
    while(fgets(subFileName, sizeof(subFileName), mainFile)!=NULL){
        subFileName[strcspn(subFileName, "\r\n")] = '\0';

        FILE *subFile = fopen(subFileName, "r");
        if(!subFile){
            printf("error in opening of sub file! \n");
            return EXIT_FAILURE;
        }

        bill *currentBill = NULL;

        int year, month, day, quantity;
        char name[20];
        float price;
        char subFileTxt[69];

        while(fgets(subFileTxt, sizeof(subFileTxt), subFile)!=NULL){
            if(sscanf(subFileTxt, "%d-%d-%d", &year, &month, &day)==3){
                bill *newBill = newBillNode(year, month, day);
                currentBill = newBill;
                billEnqueue(manager, newBill);
            } 

            else if(sscanf(subFileTxt, "%[^,], %d, %f", name, &quantity, &price)==3){
                article *newArticle = newArticleNode(name, quantity, price);
                articleEnqueue(currentBill, newArticle);
            }
        }
        fclose(subFile);
    }
    fclose(mainFile);

    return EXIT_SUCCESS;
}

int findData(char *name, char *date, manager *manager){
    int year, month, day;
    sscanf(date, "%d-%d-%d ", &year, &month, &day);

    bill *temp = manager->front;
    while(temp!=NULL){
        if(temp->year==year){
            if(temp->month==month){
                if(temp->day==day) break;
            }
        }

        temp = temp->next;
    }

    if(temp==NULL){
        printf("there is no bill under that date! \n");
        return EXIT_FAILURE;
    } else{
        article *tempArticle = temp->articleHead;

        while(tempArticle!=NULL){
            if(strcmp(tempArticle->name, name)==0){
                printf("%s is bought %d times and price of it all was %f\n", name, tempArticle->quantity, tempArticle->price*tempArticle->quantity);
                return EXIT_SUCCESS;
            }

            tempArticle = tempArticle->next;
        }

        printf("there is no such item in bill under that date! \n");
    } 

    return EXIT_FAILURE;
}

void dequeue(manager *manager){
    while(manager->front!=NULL){
        bill *temp = manager->front;
        printf("%d-%d-%d\n", temp->year, temp->month, temp->day);
        
        while(temp->articleHead!=NULL){
            article *artTemp = temp->articleHead;
            printf("%s, %d, %f\n", artTemp->name, artTemp->quantity, artTemp->price);
            temp->articleHead = temp->articleHead->next;
            free(artTemp);
        }
        puts("\n");

        manager->front = manager->front->next;
        free(temp);
    }
}
