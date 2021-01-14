#include <Shared.cpp>
#include <iostream>

Fifo *Shared::freeBuffers;
Fifo *Shared::pendingRequests;

pthread_mutex_t *Shared::mutexes;
pthread_mutex_t *Shared::respondedCond;
pthread_mutex_t *Shared::requestedCond;

void Shared::init(int size)
{
	pendingRequests = new Fifo(size);
	freeBuffers = new Fifo(size);
	buffers = new Buffer[size];
	mutexes = new pthread_mutex_t[size];
	respondedCond = new pthread_cond_t[size];
	requestedCond = new pthread_cond_t[size];

	for(int i=0; i<size, i++)
	{
		freeBuffers->in(i);
		mutexes[i] = PTHREAD_MUTEX_INITIALIZER;
		respondedCond[i] = PTHREAD_COND_INITIALIZER;
		requestedCond[i] = PTHREAD_COND_INITIALIZER;
	}
}

void Shared::dispose()
{
	delete pendingRequests;
	delete freeBuffers;
	delete[] buffers;
	delete[] respondedCond;
	delete[] requestedCond;
}

void Shared::write(char *data, int id) 
{
	pthread_mutex_lock(&mutexes[id]);
	buffers[id].data = data;
	pthread_mutex_unlock(&mutexes[id]);
}

void Shared::updateStatus(int statuc, int id)
{
	pthread_mutex_lock(&mutexes[id]);
	buffers[id].status = status;

	if(buffers[id].status == BUFFER_STATUS_RESPONDED) 
	{
		pthread_cond_broadcast(&respondedCond[id]);
	}
	else
	{
		pthread_cond_broadcast(&requestedCond[id]);
	}

	pthread_mutex_unlock(&mutexes[id]);
}

void Shared::waitForStatus(int status, int id) 
{
	pthread_mutex_lock(&mutexes[id]);

	while(buffers[id].status != status)
	{
		if(status == BUFFER_STATUS_RESPONDED)
		{
			pthread_cond_wait(&respondedCond[id], &mutexes[id]);
		}
		else
		{
			pthread_cond_wait(&requestedCond[id], &mutexes[id]);
		}
	}

	pthread_mutex_unlock(&mutexes[id]);
}

char *Shared::read(int id)
{
	pthread_mutex_lock(&mutexes[id]);

	char *data = buffers[id].data;

	pthread_mutex_unlock(&mutexes[id]);

	return data;
}

void Shared::releaseBuffer(int id)
{
	pthread_mutex_lock(&mutexes[id]);

	buffers[id].data = (char *)"";

	freeBuffers->in(id);

	pthread_mutex_unlock(&mutexes[id]);
}

