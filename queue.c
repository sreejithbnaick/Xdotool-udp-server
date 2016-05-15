/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "queue.h"

Queue * createQueue(int maxElements) {
    /* Create a Queue */
    Queue *Q;
    Q = (Queue *) malloc(sizeof (Queue));
    /* Initialise its properties */
    Q->elements = malloc(sizeof (char*) *maxElements);
    Q->size = 0;
    Q->capacity = maxElements;
    Q->front = 0;
    Q->rear = -1;
    /* Return the pointer */
    return Q;
}

char * Dequeue(Queue *Q) {
    char * ret = 0;
    /* If Queue size is zero then it is empty. So we cannot pop */
    if (Q->size == 0) {
        printf("Queue is Empty\n");
        return 0;
    }        /* Removing an element is equivalent to incrementing index of front by one */
    else {
        ret = front(Q);
        Q->size--;
        Q->front++;
        /* As we fill elements in circular fashion */
        if (Q->front == Q->capacity) {
            Q->front = 0;
        }
    }
    return ret;
}

char * front(Queue *Q) {
    if (Q->size == 0) {
        printf("Queue is Empty\n");
        exit(0);
    }
    /* Return the element which is at the front*/
    return Q->elements[Q->front];
}

void Enqueue(Queue *Q, char * element) {
    /* If the Queue is full, we cannot push an element into it as there is no space for it.*/
    if (Q->size == Q->capacity) {
        printf("Queue is Full\n");
    } else {
        Q->size++;
        Q->rear = Q->rear + 1;
        /* As we fill the queue in circular fashion */
        if (Q->rear == Q->capacity) {
            Q->rear = 0;
        }
        /* Insert the element in its rear side */
        Q->elements[Q->rear] = element;
        printf("Queue size %d\n",Q->size);
    }
    return;
}
