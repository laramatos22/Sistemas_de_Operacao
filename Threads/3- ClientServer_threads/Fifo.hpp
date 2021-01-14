#pragma once
#include <thread.h>

class Fifo {
    public:
        Fifo(int size);
        ~Fifo();
        int out();
        void in(int value);
        bool isEmpty();
        bool isFull();
        int getCount();

    private:
        pthread_mutex_t mutex;
        pthread_cond_t isNotFullCond;
        pthread_cond_t isNotEmptyCond;
        int *data;
        int size;
        int head;
        int tail;
        int count;
};