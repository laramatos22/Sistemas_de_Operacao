#pragma once
#include <Fifo.hpp>
#include <Buffer.hpp>

class Shared {
    public:
        static void init(int size);
        static void dispose();
        static char *read(int id);
        static void write(char *data, int id);
        static void updateStatus(int status, int id);
        static void waitForStatus(int status, int id);
        static void releaseBuffer(int id);
        static Fifo *pendingRequests;
        static Fifo *freeBuffers;

    private:
        static Buffer *buffers;
        static pthread_mutex_t *mutexes;
        static pthread_cond_t *respondedCond;
        static pthread_cond_t *requestedCond;
};