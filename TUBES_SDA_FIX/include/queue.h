// include/queue.h

#ifndef QUEUE_H
#define QUEUE_H

#include "sll.h"

// Node untuk Queue
typedef struct QueueNode {
    Paper data;
    struct QueueNode* next;
} QueueNode;

/*
 * =====================================================================================
 * Penjelasan Struktur Data Queue
 * =====================================================================================
 * Queue (Antrean) adalah struktur data yang menganut prinsip FIFO (First-In, First-Out).
 * Data yang pertama masuk adalah data yang pertama keluar.
 * - 'front': Pointer ke elemen pertama (kepala antrean).
 * - 'rear': Pointer ke elemen terakhir (ekor antrean).
 *
 * Mengapa Queue? Queue sangat cocok untuk fitur "Rekomendasi Paper". Saat kita
 * menemukan paper yang relevan, kita masukkan (enqueue) ke dalam antrean.
 * Kemudian, kita tampilkan satu per satu (dequeue) sesuai urutan penemuannya.
 * Ini memberikan pengalaman yang adil dan terurut bagi pengguna.
 * =====================================================================================
 */
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Inisialisasi queue.
void initQueue(Queue* q);

// Menambah elemen ke akhir queue.
void enqueue(Queue* q, Paper data);

// Mengambil elemen dari awal queue.
Paper dequeue(Queue* q);

// Membebaskan memori queue.
void freeQueue(Queue* q);

#endif