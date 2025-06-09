// include/queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include "sll.h"

typedef struct QueueNode {
    Paper data;
    struct QueueNode* next;
} QueueNode;

/*
 * =====================================================================================
 * PENJELASAN UNTUK DOSEN: Struktur Data Queue
 * =====================================================================================
 * "Kami mengimplementasikan Queue (Antrean) untuk fitur rekomendasi paper.
 * Queue bekerja dengan prinsip FIFO (First-In, First-Out), yang sangat cocok
 * untuk kasus ini. Saat sistem menemukan paper-paper yang mensitasi paper target,
 * paper tersebut dimasukkan (enqueue) ke dalam antrean. Kemudian, saat ditampilkan
 * kepada pengguna, paper akan keluar (dequeue) sesuai urutan penemuannya,
 * menciptakan alur rekomendasi yang logis dan terurut."
 * =====================================================================================
 */
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

void initQueue(Queue* q);
void enqueue(Queue* q, Paper data);
Paper dequeue(Queue* q);
void freeQueue(Queue* q);

#endif