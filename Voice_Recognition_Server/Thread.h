#ifndef _THREAD_H_
#define _THREAD_H_
#ifdef WIN32
#include <windows.h>
#define DoSleep(x) (Sleep(x))
#else
#include <unistd.h>
#define DoSleep(x) (usleep(x * 1000))
#endif

#include <pthread.h>
class Thread {
private:
    pthread_t thread;

    static void* thread_func(void *d) {
        return ((Thread *)d)->run();
    }

protected:
    bool running;

public:
    Thread() {
        running=false;
    }
    virtual ~Thread() {}
    virtual void* run() {
        return (void *)0;
    }
    bool start() {
        running = true;
        return !pthread_create(&thread, NULL, Thread::thread_func, (void*)this);
    }
    bool isRunning() {
        return this->running;
    }

    int join() {
        return !pthread_join(thread, NULL);
    }
    void exit() {
        this->running = false;
        return pthread_exit((void *)this);
    }
    void stop() {
        this->running = false;
    }
};

class Mutex
{
private:
    pthread_mutex_t *mutex;
public:
    Mutex()
    {
        pthread_mutex_init(this->mutex, NULL);
    }

    Mutex(pthread_mutex_t *mutex)
    {
        this->mutex = mutex;
    }
    pthread_mutex_t* GetMutex()
    {
        return mutex;
    }
    ~Mutex()
    {
        pthread_mutex_destroy(this->mutex);
    }
};

class MutexLocker
{
private:
    Mutex* mutex;
public:

    MutexLocker(Mutex* mutex)
    {
        this->mutex = mutex;
        pthread_mutex_lock(this->mutex->GetMutex());
    }

    ~MutexLocker()
    {
        pthread_mutex_unlock(this->mutex->GetMutex());
    }
};


#endif
