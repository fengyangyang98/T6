#ifndef LCOK_HPP_
#define LCOK_HPP_

#include <pthread.h>
#include <semaphore.h>  

class exclusiveLock 
{
private:
    pthread_mutex_t _lock;
public:
    exclusiveLock() 
    {
        pthread_mutex_init(&_lock, 0);
    }

    ~exclusiveLock() 
    {
        pthread_mutex_destroy(&_lock);
    }

    void get()
    {
        pthread_mutex_lock(&_lock);
    }

    void release()
    {
        pthread_mutex_unlock(&_lock);
    }

    bool tryGet()
    {
        return (pthread_mutex_trylock(&_lock) == 0);
    }
};

class cpLock
{
private:
    sem_t empty;
    sem_t full;
    sem_t mutex;

public:
    cpLock(int len=1)
    {
        sem_init(&empty,0,len);
        sem_init(&full,0,0);
        sem_init(&mutex,0,1);
    }

    void cGet()
    {
        sem_wait(&full);
        sem_wait(&mutex);
    }

    void cRelease()
    {
        sem_post(&mutex);
        sem_post(&empty);
    }

    void pGet()
    {
        sem_wait(&empty);
        sem_wait(&mutex);
    }

    void pRelease()
    {
        sem_post(&mutex);
        sem_post(&full);
    }

};

#endif