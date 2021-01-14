#include <Fifo.hpp>

Fifo::Fifo(int size)
{
    this->mutex = PTHREAD_MUTEX_INITIALIZER;
    this->isNotEmptyCond = PTHREAD_COND_INITIALIZER;
    this->isNotFullCond = PTHREAD_COND_INITIALIZER;
    this->size = size;
    this->data = new int[size];
}

Fifo::~Fifo()
{
    delete[] this->data;
}

bool Fifo::isEmpty()
{
    return this->count == 0;
}

bool Fifo::isFull()
{
    return this->count == this->size;
}

void Fifo::in(int value)
{
    pthread_mutex_lock(&this->mutex);
    while (this->isFull())
    {
        pthread_cond_wait(&this->isNotFullCond, &this->mutex);
    }
    this->data[this->head] = value;
    this->head = (this->head + 1) % this->size;
    this->count++;
    pthread_cond_broadcast(&this->isNotEmptyCond);
    pthread_mutex_unlock(&this->mutex);
}

int Fifo::out()
{
    pthread_mutex_lock(&this->mutex);
    while (this->isEmpty())
    {
        pthread_cond_wait(&this->isNotEmptyCond, &this->mutex);
    }
    int value = this->data[this->tail];
    this->tail = (this->tail + 1) % this->size;
    this->count--;
    pthread_cond_broadcast(&this->isNotFullCond);
    pthread_mutex_unlock(&this->mutex);
    return value;
}

int Fifo::getCount()
{
    return this->count;
}


