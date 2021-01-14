#include <Client.hpp>
#include <Shared.hpp>
#include <unistd.h>
#include <iostream>

int Client::requestBuffer()
{
    return Shared::freeBuffers->out();
}

void Client::putData(char *data, int id)
{
    Shared::write(data, id);
    Shared::pendingRequests->in(id);
    Shared::updateStatus(BUFFER_STATUS_REQUESTED, id);
}

char *Client::getResponse(int id)
{
    Shared::waitForStatus(BUFFER_STATUS_RESPONDED, id);
    auto value = Shared::read(id);
    return value;
}

void Client::releaseBuffer(int id)
{
    Shared::releaseBuffer(id);
}

