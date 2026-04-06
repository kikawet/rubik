#ifndef RUBIK_BARRIER_H
#define RUBIK_BARRIER_H

// I had to make my own barrier because pthread does not match my requirements....
// be aware this barrier is painful to even watch, but I wanted something that will release everything on destroy

typedef struct barrier ThreadBarrier;

int thread_barrier_init(ThreadBarrier* barrier, int num_threads);
int thread_barrier_wait(ThreadBarrier* barrier);
int thread_barrier_cleanup(ThreadBarrier* barrier); // call only inside pthread_cleanup_push and you better call thread_barrier_destroy right after because state is broken without any mercy
int thread_barrier_destroy(ThreadBarrier* barrier);

#ifdef BARRIER_IMPLEMENTATION

#define thread_barrier_clean_errno() (errno == 0 ? "None" : strerror(errno))
#define thread_barrier_log_error(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __filename, __line, thread_barrier_clean_errno(), ##__VA_ARGS__)
#define thread_barrier_assertf(A, M, ...) if(!(A)) {thread_barrier_log_error(M, ##__VA_ARGS__); assert(A); }

static int barrier_run_inner(const int result, const char* __filename, const int __line)
{
    thread_barrier_assertf(result == 0, "Assert failed with code %d", result);
    return result;
}

#define BARRIER_RUN(F) barrier_run_inner((F), __FILE__, __LINE__)

struct barrier
{
    int num_threads;
    int count;
    int waiting;
    int round;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
    pthread_cond_t cond;
};

int thread_barrier_init(ThreadBarrier* barrier, const int num_threads)
{
    barrier->num_threads = num_threads;
    barrier->count = 0;
    barrier->waiting = 0;
    barrier->round = 0;

    BARRIER_RUN(pthread_mutexattr_init(&barrier->mutexattr));
    BARRIER_RUN(pthread_mutexattr_settype(&barrier->mutexattr, PTHREAD_MUTEX_ERRORCHECK));

    BARRIER_RUN(pthread_cond_init(&barrier->cond, NULL));
    return BARRIER_RUN(pthread_mutex_init(&barrier->mutex, &barrier->mutexattr));
}

int thread_barrier_wait(ThreadBarrier* barrier)
{
    // Implementation is from here https://stackoverflow.com/questions/78679894/trying-to-implement-a-barrier-for-pthreads-in-c-using-pthread-condition-variab
    BARRIER_RUN(pthread_mutex_lock(&barrier->mutex));

    const int wait_round = barrier->round;
    barrier->waiting++;
    if (barrier->waiting == barrier->num_threads)
    {
        barrier->waiting = 0;
        barrier->round++;
        BARRIER_RUN(pthread_cond_broadcast(&barrier->cond));
    }
    else
    {
        while ((barrier->waiting < barrier->num_threads) && (wait_round == barrier->round))
        {
            BARRIER_RUN(pthread_cond_wait(&barrier->cond, &barrier->mutex));
            pthread_testcancel();
        }
    }

    return pthread_mutex_unlock(&barrier->mutex);
}

inline int thread_barrier_cleanup(ThreadBarrier* barrier)
{
    return pthread_mutex_unlock(&barrier->mutex);
}

int thread_barrier_destroy(ThreadBarrier* barrier)
{
    BARRIER_RUN(pthread_cond_destroy(&barrier->cond));
    BARRIER_RUN(pthread_mutex_destroy(&barrier->mutex));
    return BARRIER_RUN(pthread_mutexattr_destroy(&barrier->mutexattr));
}

#endif // BARRIER_IMPLEMENTATION

#endif // RUBIK_BARRIER_H
