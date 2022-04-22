#include <stdlib.h>    // for exit()
#include <stdio.h>     // for the usual printf(), etc.
#include <getopt.h>    // for getopt()
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "eprintf.h"
#include <sys/wait.h>


/*
 * Note the new #include
 */
#include "critical_section.h"


// To get `getopt_long()` to work, you need to provide a static
// (usually) array of `struct option` structures.  There are four
// members to be filled in:

// 1. `name` is a (char *) string containing the "long" option name
// (e.g. "--help" or "--format=pdf").

// 2. `has_arg` has one of these values that describe the
// corresponding option:
enum {
    NO_ARG  = 0, // the option takes no argument
    REQ_ARG = 1, // the option must have an argument
    OPT_ARG = 2  // the option takes an optional argument
};

// 3. The "flag" is an int pointer that determines how the function
// will return its value. If it is NULL, getopt_long() will return
// "val" (the fourth member) as its function return. If it is not
// NULL, getopt_long() will return 0 and set "*flag" to "val".

// 4. "val" is an int which is either a character to denote a "short"
// (e.g. "-h" or "-f pdf") option or 0, to denote an option which does
// not have a "short" form.

// The array is terminated by an entry with a NULL name (first
// element).

static struct option options[] = {
    // elements are:
    // name       has_arg   flag   val
    { "children", OPT_ARG,  NULL,  'c'},
    { "help",     NO_ARG,   NULL,  'h'},
    { "nosync",   NO_ARG,   NULL,  'n'},
    { "pgid",     NO_ARG,   NULL,  'g'},
    { "ppid",     NO_ARG,   NULL,  'p'},
    { NULL }  // end of options table
};

/*
 * These globals are set by command line options. Here, they are set
 * to their default values.
 */
int showPpids = 0;   // show parent process IDs
int showPgids = 0;   // show process group IDs
int synchronize = 1; // synchronize outputs (don't use until Part 3)


enum { IN_PARENT = -1 }; // must be negative
/*
 * In the parent, this value is IN_PARENT. In the children, it's set
 * to the order in which they were spawned, starting at 0.
 */
int siblingIndex = IN_PARENT;


// This is a global count of signals received.
int signalCount = 0;


void writeLog(char message[], const char *fromWithin)
// print identifying information about the current process to stdout
{
    int colonIndent;
    char *processName = (char *) calloc(1, sizeof(char*));

    if(siblingIndex == IN_PARENT)
    {
        strcpy(processName, "parent");
        colonIndent = 20;
    }
    else
    {
        snprintf(processName, 10 * sizeof(char), "child %d", siblingIndex);
        colonIndent = 30;
    }
    if(synchronize)
    {
        criticalSection_enter();
    }
    printf("%*s: %s\n", colonIndent, "process name", processName);
    printf("%*s: %d\n", colonIndent, "process ID", getpid());
    if(showPpids)
    {
        printf("%*s: %d\n", colonIndent, "parent process ID", getppid());
    }
    if(showPgids)
    {
        printf("%*s: %d\n", colonIndent, "process GID", getpgrp());
    }
    printf("%*s: %d\n", colonIndent, "signal count", signalCount);
    printf("%*s: %s\n", colonIndent, "message", message);
    printf("%*s: %s\n", colonIndent, "from within", fromWithin);
    printf("\n");
    if(synchronize)
    {
        criticalSection_leave();
    }
}


void inChild(int iSibling)
// do everything that's supposed to be done in the child
{
    siblingIndex = iSibling;
    writeLog("pause()'d for signal", __func__);
    while(1)
    {
        pause();
    }
}


void handler(int sigNum)
// handle signal `sigNum`
{
    signalCount++;
    char *message = (char *) calloc(1, sizeof(char*));
    snprintf(message, 100, "%s", strsignal(sigNum));
    writeLog(message, __func__);
}


void initSignals(void)
// initialize all signals
{
    for(int i = 1; i <= _NSIG; i++)
    {
        if(i == SIGTRAP || i == SIGQUIT)
        {
            ;
        }
        else if((signal(i, handler) == SIG_ERR))
        {
            writeLog(strsignal(i), __func__);
        }
    }
}


void inParent(void)
// do everything that's supposed to be done in the parent
{
    writeLog("parent is waiting for children to exit", __func__);
    int wstatus;
    while(wait(&wstatus) != -1)
    {
        if(WIFEXITED(wstatus))
        {
            writeLog("child process exited normally", __func__);
        }
        else
        {
            writeLog("child process exited abnormally", __func__);
        }
        if(WIFSIGNALED(wstatus))
        {
            writeLog(strsignal(wstatus), __func__);
        }
        wait(&wstatus);
    }
    if(wstatus == -1)
    {
        writeLog("no children to wait for", __func__);
    }
    else
    {
        writeLog("wait() failed for an unknown reason", __func__);
    }
}


static void usage(char *progname)
// issue a usage error message
{
    eprintf("usage: %s [{args}]*\n", progname);
    eprintf("%s\n", " {args} are:");
    eprintf("%s",
        "  -c[{arg}] or --children[={arg}]  fork {arg} children (default: 1)\n"
        "  -g or --pgid                     list process group IDs\n"
        "  -n or --nosync                   turn off synchronization\n"
        "  -p or --ppid                     list parent PIDs (default: no)\n"
        "  -h or --help                     help (this message) and exit\n"
        );
    return;
}

int main(int argc, char **argv)
{
    int ch;
    int nSiblings = 0;
    static char *progname = "**UNSET**";

    /*
     * Parse the command line arguments.
     */
    progname = argv[0];
    for (;;) {
        ch = getopt_long(argc, argv, "c::ghnp", options, NULL);
        if (ch == -1)
            break;

        switch (ch) {

        case 'c':
            if (optarg)
                nSiblings = atoi(optarg);
            else
                nSiblings = 1;
            break;

        case 'g':
            showPgids = 1;
            break;

        case 'h':
            usage(progname);
            exit(0);

        case 'n':
            synchronize = 0;
            break;

        case 'p':
            showPpids = 1;
            break;

        default:
            printf("?? getopt returned character code 0x%02x ??\n", ch);
            exit(1);
        }
    }
    /*
     * ASSIGNMENT
     *
     * - Insert your previous main() code here unchanged, except that
     *   if the global `synchronize` flag is set, add a call to
     *   criticalSection_init() before the initializeSignals() call.
     */
    if(synchronize)
    {
        criticalSection_init();
    }
    initSignals();
    if(nSiblings == 0)
    {
        writeLog("pause()'d for signal", __func__);
        while(1)
        {
            pause();
        }
    }
    else
    {
        for(int iSibling = 0; iSibling < nSiblings; iSibling++)
        {
            if(fork() == 0)
            {
                inChild(iSibling);
            }
            
            {
                char *buffer = (char *) calloc(100,sizeof(char*));
                snprintf(buffer, 100, "parent forked iSibling %d process %d", iSibling, getpid());
                writeLog(buffer, __func__);
            }
        }
        inParent();
    }
    return 0;
}