#include <stdlib.h> // for malloc()
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <stdio.h>

#include "a2d.h"
#include "tspec_diff.h"
#include "mat_mul_pt2.h"

/*
 *  Good practice: This structure contains all of the "globals" each
 *  thread can access.
 *
 *  A pointer to a shared instance of this one struct is shared by all
 *  threads used for a single matrix multiply, so any values that
 *  might be modified by more than one thread should not be changed by
 *  any thread without being protected by some kind of mutex, a la
 *  "nextRow".
 */


typedef struct {
    /*
     * `nextRow` is modified by every thread, so it needs a mutex.
     */
    int nextRow; /* the next row to be done *and* # of rows already done */
    pthread_mutex_t nextRowMutex; /* restricts access to `nextRow` */

    /*
     * As far as a thread is concerned, these are constants, so no
     * mutex is necessary.
     */
    int nThreads;
    int n, m, p; /* matrix dimensions */
    const double *_a, *_b; /* input (1D) matrices */

    /*
     * Each row of this matrix is modified by only one thread, so
     * again no mutex is necessary. (Note how this decision, which is
     * critical to good threaded performance, requires knowledge of
     * the algorithm.)
     */
    double *_c; /* output (1D) matrix */
} ThreadGlobals;


static void multiplyRow(double *_c, const int i, const int n, const int m,
                        const double *_a, const int p, const double *_b)
{
#define c(i,j) _c[I2D(i, n, j, m)]
#define a(i,j) _a[I2D(i, n, j, p)]
#define b(i,j) _b[I2D(i, p, j, m)]
    int j, k;
    double sum;

    sum = 0.0;
    for (j = 0; j < m; j++) {
        sum = 0.0;
        for (k = 0; k < p; k++)
            sum += a(i,k) * b(k,j);
        c(i,j) = sum;
    }
#undef a
#undef b
#undef c
}


// inThread -- function for each thread
static void *inThread(void *threadGlobals_)
{
    ThreadGlobals *threadGlobals = threadGlobals_;
    MatMulThreadStats *matMulStats = malloc(sizeof(MatMulThreadStats));
    matMulStats->nRowsDone = 0;
    struct timespec PTbef, PTaft;
    clockid_t cid;
    pthread_getcpuclockid(pthread_self(), &cid);
    clock_gettime(cid,&PTbef);
    int i = 0;
    static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    while(1)
    {
        if(threadGlobals->nThreads > 1)
        {
            pthread_mutex_lock(&mut);
        }
        i = threadGlobals->nextRow; //RACE CONDITION
        threadGlobals->nextRow++;
        if(threadGlobals->nThreads > 1)
        {
            pthread_mutex_unlock(&mut);
        }
        if(i >= threadGlobals->n)
        {
            break;
        }
        multiplyRow(threadGlobals->_c,i,threadGlobals->n,threadGlobals->m,threadGlobals->_a,threadGlobals->p, threadGlobals->_b);
        matMulStats->nRowsDone++;
    }

    clock_gettime(cid,&PTaft);
    matMulStats->cpuTime = tspecDiff(PTbef,PTaft);
    return matMulStats;
}


void mat_mul(double *_c, const int n, const int m,
             const double *_a, const int p, const double *_b,
             const int nThreads, MatMulThreadStats allMatMulThreadStats[])
{
    ThreadGlobals *threadGlobals = malloc(sizeof(ThreadGlobals));
    threadGlobals->_c = _c;
    threadGlobals->n = n;
    threadGlobals->m = m;
    threadGlobals->_a = _a;
    threadGlobals->p = p;
    threadGlobals->_b = _b;
    threadGlobals->nextRow = 0;
    threadGlobals->nThreads = nThreads;

    static pthread_mutex_t nextRow;
    if(nThreads > 0)
    {
        pthread_t *threads = malloc(nThreads * sizeof(pthread_t));
        pthread_mutex_init(&nextRow,NULL);
        for(int i = 0; i < nThreads; i++)
        {
            pthread_create(&threads[i], NULL, inThread, threadGlobals);
        }

        for(int i = 0; i < nThreads; i++)
        {
            void *retval;
            pthread_join(threads[i], &retval);
            allMatMulThreadStats[i] = *((MatMulThreadStats *)retval);
            free(retval);
        }

        pthread_mutex_destroy(&nextRow);
        free(threads);
    }
    else
    {
        void *retval;
        retval = inThread(threadGlobals);
        allMatMulThreadStats[0] = *((MatMulThreadStats *)retval);
        free(retval);
    }
    free(threadGlobals);
}
