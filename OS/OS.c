#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#define LOWER_NUM 1
#define UPPER_NUM 10000
#define BUFFER_SIZE 100
#define MAX_COUNT 10000

int bufferArr[BUFFER_SIZE];
int bufferCounter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int finished = false;

void *producer(void *arg) {
    FILE *file = fopen("all.txt", "w");
    for (int i = 0; i < MAX_COUNT; i++) {
        int number = rand() % (UPPER_NUM - LOWER_NUM + 1) + LOWER_NUM;
        while(bufferCounter >= BUFFER_SIZE){
            //...
        };
        pthread_mutex_lock(&lock);
        bufferArr[bufferCounter++] = number;
        if (file != NULL) {
            fprintf(file, "%d\n", number);  
        }
        pthread_mutex_unlock(&lock);
    }
    fclose(file);
    finished = true;
}

void *customer(void *arg) {
    int parity = *((int *)arg);
    char filename[20];
    sprintf(filename, "%s.txt", parity ? "odd" : "even");
    FILE *file = fopen(filename, "w");

    while (true) {
        pthread_mutex_lock(&lock);
        if (bufferCounter == 0 && finished) {
            pthread_mutex_unlock(&lock);
            break;
        }

        if (bufferCounter > 0) {
            int number = bufferArr[bufferCounter-1];
            if ((number % 2) == parity) {
                bufferCounter--;
                if (file != NULL) {
                    fprintf(file, "%d\n", number);
                }
            }
        }
        pthread_mutex_unlock(&lock);
    }
    fclose(file);
}



int main() {
    pthread_t prod_tid, cust1_tid, cust2_tid;
    int custEven_parity = 0;
    int custOdd_parity = 1;

    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cust1_tid, NULL, customer, &custEven_parity);
    pthread_create(&cust2_tid, NULL, customer, &custOdd_parity);

    pthread_join(prod_tid, NULL);
    pthread_join(cust1_tid, NULL);
    pthread_join(cust2_tid, NULL);

    // printf("Done.\n");
    return 0;
}