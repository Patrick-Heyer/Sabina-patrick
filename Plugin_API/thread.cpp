void* Thread::memberFunction(void) {
    // Do actual work here
    return 0;
}

void  Thread::startThread(void) {
    pthread_t tid;
    int       result;
    result = pthread_create(&tid, 0, Thread::callMemberFunction, this);
    if (result == 0)
        pthread_detach(tid);
}
