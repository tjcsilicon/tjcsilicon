#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <sys/time.h>
#include <time.h> // for `clock_gettime()`

#include "a2d.h"
#include "eprintf.h"
#include "tspec_diff.h"
#include "mat_mul_pt2.h"

char *progname = "*** error: 'progname' not set ***";

#define MAX(x, y) (x > y ? x : y)

// forward reference
static double dot(double *a, double *b, int n);


double *da2d_new(int n, int m)
// instances an n x m array of doubles
{
    double *result = malloc(n * m * sizeof(double));
    assert(result != NULL);
    return result;
}


void da2d_delete(double *d)
// frees an array
{
    free(d);
}


void da2d_printf(double *_d, int n, int m, char fmt[])
// prints a 2D array using a given format `fmt` for each element
{
#define d(i,j) _d[I2D(i, n, j, m)]
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf(fmt, d(i,j));
        }
        printf("\n");
    }
#undef d
}


void da2d_orthonormalize(double *_a, int n, int m)
{
/* makes an n x m matrix `_a` orthonormal via Gram-Schmidt (in-place) */
#define a(i,j) _a[I2D(i, n, j, m)]
    int i, iPrev, j;
    double dotProduct, mag;

    for (i = 0; i < n; i++)
    {
        // For all previous rows...
        for (iPrev = 0; iPrev < i; iPrev++)
        {
            // ...compute the dot product of row i with row iPrev...
            dotProduct = dot(&a(iPrev,0), &a(i,0), m);
            // ...then subtract row iPrev scaled by the dot product.
            for (j = 0; j < m; j++)
            {
                a(i,j) -= dotProduct * a(iPrev,j);
            }
        }
        // Normalize the modified row i.
        mag = sqrt(dot(&a(i,0), &a(i,0), m));
        for (j = 0; j < m; j++)
        {
            a(i,j) /= mag;
        }
    }
#undef a
}


void da2d_fillRandom(double *_a, int n, int m)
/* fills a 2D array with random numbers */
{
#define a(i,j) _a[I2D(i, n, j, m)]
    int i, j;

    // set a to a random matrix
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            a(i,j) = ((double) random()) / RAND_MAX;
        }
    }
#undef a
}


double *da2d_transpose(double *_a, int n, int m)
{
#define a(i,j) _a[I2D(i, n, j, m)]
#define b(i,j) _b[I2D(i, m, j, n)]
    double *_b = da2d_new(m, n);
    int i, j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            b(j,i) = a(i,j);
        }
    }
    return _b;
#undef b
#undef a
}


static double dot(double *a, double *b, int n)
// compute the dot product of two 1-d double arrays of length `n`
{
    double result = 0;
    int i;

    for (i = 0; i < n; i++)
        result += a[i] * b[i];
    return result;
}


/* usage: issue a usage error message */
static void usage(void)
{
    eprintf("usage: %s [{args}]\n", progname);
    eprintf("%s",
            "computes the matrix product c = a x b for two random matrices\n"
            "a[] and b[] of user-specified size, optionally with threads,\n"
            "and prints the times involved\n"
            " {args} are:\n");
    eprintf("%s\n",
            "  -h      this help message");
    eprintf("%s\n",
            "  -n {i}  number of rows of c[] and a[] (default: 4)");
    eprintf("%s\n",
            "  -m {i}  number of columns of c[] and b[] (default: 4)");
    eprintf("%s\n",
            "  -o      test algorithm by forcing a[] to be orthonormal and b[]\n"
            "          to be its transpose (implies '-v' and n == m == p; default: don't)");
    eprintf("%s\n",
            "  -p {i}  number of columns of a[] and rows of b[] (default: 4)");
    eprintf("%s\n",
            "  -s {i}  seed value for srandom (default: no seeding)");
    eprintf("%s\n",
            "  -t {i}  number of threads (0 means unthreaded) (default: 4)");
    eprintf("%s",
            "  -v      verbose: print out a and b as well as their product\n");
    return;
}


int main(int argc, char *argv[])
{
    int n = 4;
    int p = 4;
    int m = 4;
    int verbose = 0;
    int orthonormal = 0;
    int ch, ok;
    double *a, *b, *c;
    int colonIndent;
    int nThreads = 4;
    //int i;
    //MatMulThreadStats *matMulThreadStats;
    double cpuTimeTotal;

    ok = 1;
    progname = argv[0];
    while ((ch = getopt(argc, argv, "hm:n:op:qs:t:v")) != -1) {
        switch (ch) {

        case 'h':
            usage();
            exit(EXIT_SUCCESS);

        case 'n':
            n = atoi(optarg);
            break;

        case 'o':
            verbose = 1;
            orthonormal = 1;
            break;

        case 'm':
            m = atoi(optarg);
            break;

        case 'p':
            p = atoi(optarg);
            break;

        case 's':
            srandom(atoi(optarg));
            break;

        case 't':
            nThreads = atoi(optarg);
            break;

        case 'v':
            verbose = 1;
            break;

        default:
            ok = 0;
            break;
        }
    }
    if (!ok || n <= 0 || p <= 0  || m <= 0 || nThreads < 0) {
        usage();
        exit(EXIT_FAILURE);
    }

    /*
     * This value is set empirically so that all the ':'s line up
     * (like movie credits) for readability.
     */
    colonIndent = 30;

    if (orthonormal) {
        // necessary: override any command line specification of m or p
        m = p = n;
    }
    a = da2d_new(n, p);
    da2d_fillRandom(a, n, p);
    if (orthonormal) {
        da2d_orthonormalize(a, n, p);
    }
    if (verbose) {
        printf("%*s:\n", colonIndent, "a");
        da2d_printf(a, n, p, "%8.3f ");
        printf("\n");
    }

    if (orthonormal) {
        b = da2d_transpose(a, n, p);
    } else {
        b = da2d_new(p, m);
        da2d_fillRandom(b, p, m);
    }
    if (verbose) {
        printf("%*s:\n", colonIndent, "b");
        da2d_printf(b, p, m, "%8.3f ");
        printf("\n");
    }

    c = da2d_new(n, m);

    struct timespec RTbef, RTaft, PTbef, PTaft;
    clock_gettime(CLOCK_REALTIME, &RTbef);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &PTbef);

    MatMulThreadStats *AllMatMulThreadStats = malloc(MAX(nThreads,1) * sizeof(MatMulThreadStats));

    mat_mul(c,n,m,a,p,b,nThreads,AllMatMulThreadStats);
   
    clock_gettime(CLOCK_REALTIME, &RTaft);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &PTaft);

    if (verbose) {
        printf("%*s:\n", colonIndent, "c (= a x b)");
        da2d_printf(c, n, m, "%8.3f ");
        printf("\n");
    }

    if(nThreads == 0)
    {
        printf("\tCPU TIME: %0.10lf\n", AllMatMulThreadStats[0].cpuTime);
        printf("\tREAL TIME: %0.10lf\n", tspecDiff(RTbef,RTaft));
    }

    else
    {
        cpuTimeTotal = 0.0;
        for(int i = 0; i < nThreads; i++)
        {
            printf("\n\tCPU TIME THREAD %d: %0.10lf\n",i, AllMatMulThreadStats[i].cpuTime);
            cpuTimeTotal += AllMatMulThreadStats[i].cpuTime;
            printf("\n\t          NUM ROWS: %d\n",AllMatMulThreadStats[i].nRowsDone);
        }
        printf("\n\tCPU TIME: %0.10lf\n",cpuTimeTotal);
    }

    printf("\n\tCPU TIME: %0.10lf\n", tspecDiff(PTbef,PTaft));
    printf("\tREAL TIME: %0.10lf\n", tspecDiff(RTbef,RTaft));

    free(AllMatMulThreadStats);

    da2d_delete(a);
    da2d_delete(b);
    da2d_delete(c);

    return 0;
}