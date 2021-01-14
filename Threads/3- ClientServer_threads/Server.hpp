class Server{
    public:
        int getPendingRequest();
        char *readData(int id);
        void putData(char *requestData, int id);
};
