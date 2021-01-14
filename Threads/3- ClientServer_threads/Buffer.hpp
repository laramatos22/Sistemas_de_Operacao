#pragma once

#define BUFFER_STATUS_REQUESTED 0
#define BUFFER_STATUS_RESPONDED 1

typedef struct Buffer {
    char *data;
    int status;
} Buffer;
