class Client
{
public:
    int requestBuffer();
    void putData(char *data, int id);
    char *getResponse(int id);
    void releaseBuffer(int id);
};