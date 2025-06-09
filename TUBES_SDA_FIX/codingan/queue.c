// src/queue.c

/*
 * File: queue.c
 * Deskripsi: Implementasi struktur data Queue (Antrean) dengan prinsip FIFO.
 * Digunakan untuk fitur rekomendasi paper.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/queue.h"

void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

void enqueue(Queue* q, Paper data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) return;
    newNode->data = data;
    newNode->next = NULL;
    if (q->rear) {
        q->rear->next = newNode;
    } else {
        q->front = newNode;
    }
    q->rear = newNode;
}

Paper dequeue(Queue* q) {
    Paper emptyPaper = {"-1", "Queue Kosong", "N/A", "N/A", 0};
    if (!q->front) {
        return emptyPaper;
    }
    QueueNode* temp = q->front;
    Paper data = temp->data;
    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    free(temp);
    return data;
}

void freeQueue(Queue* q) {
    while (q->front) {
        QueueNode* temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
}