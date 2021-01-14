#include <thread.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <Client.hpp>
#include <Server.hpp>
#include <Shared.hpp>
#define N_SERVERS 4
#define N_CLIENTS 20

static pthread_t thrServers[N_SERVERS];
static pthread_t thrClients[N_CLIENTS];

void *clientFunc(void *vargs)
{
    int id = *(int *)vargs;
    free(vargs);
    Client c = Client();
    int bufId = c.requestBuffer();
    c.putData((char *)std::to_string(id).c_str(), bufId);
    char timeStr[32];
    time_t now = time(NULL);
    strftime(timeStr, 32, "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("Client %1d requested at %s\n", id, timeStr);
    char *res = c.getResponse(bufId);
    c.releaseBuffer(bufId);
    return NULL;
}

void *serverFunc(void *vargs)
{
    int id = *(int *)vargs;
    free(vargs);
    Server s = Server();
    while (true)
    {
        using namespace std;
        int bufId = s.getPendingRequest();
        char *data = s.readData(bufId);
        time_t now = time(NULL);
        char timeStr[32];
        strftime(timeStr, 32, "%Y-%m-%d %H:%M:%S", localtime(&now));
        printf("Server %1d responded to Client %s at %s\n", id, data, timeStr);
        s.putData(data, bufId);
        usleep(1000);
    }
    return NULL;
}

int main()
{
    Shared::init(20);
    for (int i = 0; i < N_SERVERS; i++)
    {
        int *id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&thrServers[i], NULL, serverFunc, (void *)id);
    }
    for (int i = 0; i < N_CLIENTS; i++)
    {
        int *id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&thrClients[i], NULL, clientFunc, (void *)id);
    }
    for (int i = 0; i < N_CLIENTS; i++)
    {
        pthread_join(thrClients[i], NULL);
    }
    for (int i = 0; i < N_SERVERS; i++)
    {
        pthread_join(thrServers[i], NULL);
    }
    Shared::dispose();
    return 0;
}

