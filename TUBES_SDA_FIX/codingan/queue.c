#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Untuk strcmp, jika diperlukan di masa depan, atau strcpy
#include "../include/queue.h" // Sesuaikan path

void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

void enqueue(Queue* q, Paper data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) {
        perror("Gagal alokasi memori untuk QueueNode");
        return; // Atau handle error lebih baik
    }
    newNode->data = data; // Salin struct Paper
    newNode->next = NULL;
    if (q->rear) { // Jika antrean tidak kosong
        q->rear->next = newNode;
    } else { // Jika antrean kosong
        q->front = newNode;
    }
    q->rear = newNode;
}

Paper dequeue(Queue* q) {
    Paper emptyPaper = {"-1", "Queue Kosong", "N/A", "N/A", 0}; // Data sentinel
    if (!q->front) {
        // printf("Antrean kosong.\n"); // Pesan ini bisa ditampilkan di main jika perlu
        return emptyPaper;
    }
    QueueNode* temp = q->front;
    Paper data = temp->data; // Salin struct Paper
    q->front = q->front->next;
    if (!q->front) { // Jika antrean menjadi kosong setelah dequeue
        q->rear = NULL;
    }
    free(temp);
    return data;
}

void printQueue(Queue* q) {
    if (!q->front) {
        printf("\n  [INFO] Antrean kosong.\n");
        return;
    }
    QueueNode* current = q->front;
    printf("\n  +----------------------------------------------------+\n");
    printf("  |                  ISI ANTRIAN                     |\n");
    printf("  +----------------------------------------------------+\n");
    int count = 1;
    while (current) {
        printf("\n  [%d] ID    : %s\n", count++, current->data.id);
        printf("      Judul : %s\n", current->data.title);
        printf("      Tahun : %d\n", current->data.year);
        current = current->next;
    }
    printf("\n  -- Akhir Isi Antrian --\n");
}

void freeQueue(Queue* q) {
    while (q->front) {
        QueueNode* temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL; // Pastikan rear juga NULL
}