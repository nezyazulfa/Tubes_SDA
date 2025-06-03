#ifndef QUEUE_H
#define QUEUE_H

#include "sll.h" // untuk typedef Paper

typedef struct QueueNode {
    Paper data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

void initQueue(Queue* q);
void enqueue(Queue* q, Paper data);
Paper dequeue(Queue* q);
void printQueue(Queue* q); // Mungkin tidak terpakai jika rekomendasi selalu "next"
void freeQueue(Queue* q);

#endif