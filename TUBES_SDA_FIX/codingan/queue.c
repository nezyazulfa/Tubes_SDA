// src/queue.c

/*
 * File ini berisi implementasi struktur data Queue (Antrean) - FIFO.
 * Digunakan untuk fitur rekomendasi paper agar paper ditampilkan
 * sesuai urutan ditemukannya.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/queue.h"

// Menginisialisasi queue agar kosong.
void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

// Menambahkan data ke belakang (ekor) antrean.
void enqueue(Queue* q, Paper data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) return;
    newNode->data = data;
    newNode->next = NULL;

    if (q->rear) { // Jika antrean tidak kosong
        q->rear->next = newNode;
    } else { // Jika antrean kosong
        q->front = newNode;
    }
    q->rear = newNode;
}

// Mengambil data dari depan (kepala) antrean.
Paper dequeue(Queue* q) {
    // Data "sentinel" atau penanda jika antrean kosong.
    Paper emptyPaper = {"-1", "Queue Kosong", "N/A", "N/A", 0};
    if (!q->front) {
        return emptyPaper;
    }

    QueueNode* temp = q->front;
    Paper data = temp->data;
    q->front = q->front->next;

    if (!q->front) { // Jika antrean menjadi kosong setelah dequeue
        q->rear = NULL;
    }
    free(temp);
    return data;
}

// Membebaskan semua node di dalam queue.
void freeQueue(Queue* q) {
    while (q->front) {
        QueueNode* temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
}