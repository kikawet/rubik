#ifndef RUBIK_BARRIER_H
#define RUBIK_BARRIER_H

// I had to make my own barrier because pthread does not match my requirements....
// be aware this barrier is painful to even watch, but I wanted something that will release everything on destroy

typedef struct barrier ThreadBarrier;

int thread_barrier_init(ThreadBarrier* barrier, int num_threads);
int thread_barrier_wait(ThreadBarrier* barrier);
int thread_barrier_destroy(ThreadBarrier* barrier);

#ifdef BARRIER_IMPLEMENTATION

struct barrier
{
    int num_threads;
    int count;
    int waiting;
    int round;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

inline int thread_barrier_init(ThreadBarrier* barrier, const int num_threads)
{
    barrier->num_threads = num_threads;
    barrier->count = 0;
    barrier->waiting = 0;
    barrier->round = 0;

    pthread_cond_init(&barrier->cond, NULL);
    return pthread_mutex_init(&barrier->mutex, NULL);
}

inline int thread_barrier_wait(ThreadBarrier* barrier)
{
    // Implementation is from here https://stackoverflow.com/questions/78679894/trying-to-implement-a-barrier-for-pthreads-in-c-using-pthread-condition-variab
    pthread_mutex_lock(&barrier->mutex);

    const int wait_round = barrier->round;
    barrier->waiting++;
    if (barrier->waiting == barrier->num_threads)
    {
        barrier->waiting = 0;
        barrier->round++;
        pthread_cond_broadcast(&barrier->cond);
    }
    else
    {
        while ((barrier->waiting < barrier->num_threads) && (wait_round == barrier->round))
        {
            pthread_cond_wait(&barrier->cond, &barrier->mutex);
            pthread_testcancel();
        }
    }

    return pthread_mutex_unlock(&barrier->mutex);
}


inline int thread_barrier_destroy(ThreadBarrier* barrier)
{
    // When I call destroy I don't care any longer about any mutex or related just free everything and clean all resources
    pthread_cond_broadcast(&barrier->cond);
    pthread_mutex_unlock(&barrier->mutex);

    pthread_cond_destroy(&barrier->cond);
    pthread_mutex_destroy(&barrier->mutex);
    return 0;
}

#endif // BARRIER_IMPLEMENTATION

#endif // RUBIK_BARRIER_H
