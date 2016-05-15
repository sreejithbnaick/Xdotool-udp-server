/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   queue.h
 * Author: sreejith
 *
 * Created on 2 April, 2016, 5:11 PM
 */

#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Queue
{
        int capacity;
        int size;
        int front;
        int rear;
        char **elements;
}Queue;

Queue * createQueue(int maxElements);
char * Dequeue(Queue *Q);
char * front(Queue *Q);
void Enqueue(Queue *Q,char *element);


#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */

