#include <stdio.h>
#include <assert.h> // for assert()
#include <ctype.h>

struct CodeStats {
    int lineCount;
    int lineWithCodeCount;
    int foundCodeOnLine;
    int cplusplusCommentCount;
    int cCommentCount;
};


void codeStats_init(struct CodeStats *codeStats)
{
    codeStats->lineCount = 0;
    codeStats->lineWithCodeCount = 0;
    codeStats->foundCodeOnLine = 0;
    codeStats->cplusplusCommentCount = 0;
    codeStats->cCommentCount = 0;
}


void codeStats_print(struct CodeStats codeStats, char *fileName)
{
    printf("             file: %s\n", fileName);
    printf("       line count: %d\n", codeStats.lineCount);
    printf("  code line count: %d\n", codeStats.lineWithCodeCount);
    printf("c++ comment count: %d\n", codeStats.cplusplusCommentCount);
    printf("  c comment count: %d\n", codeStats.cCommentCount);
    
}


void codeStats_accumulate(struct CodeStats *codeStats, char *fileName)
{
    FILE *f = fopen(fileName, "r");
    int ch;
    enum {
        START,
        FOUNDSLASH,
        INCPPCOMMENT,
        INCCOMMENT,
        FOUNDSTAR,
    } state = START;

    assert(f);
    while ((ch = getc(f)) != EOF) {
        switch (state) {

        case START:
            if (ch == '\n') {
                codeStats->lineCount++;
                codeStats->lineWithCodeCount += codeStats->foundCodeOnLine;
                codeStats->foundCodeOnLine = 0;
            }
            else if(isspace(ch)) {

            }
            else if(ch == '/')
            {
                state = FOUNDSLASH;
            }
            else {
                codeStats->foundCodeOnLine = 1;
                state = START;
            }
            break;

        case FOUNDSLASH:
            if (ch == '\n') {
                codeStats->foundCodeOnLine = 1;
                codeStats->lineCount++;
                codeStats->lineWithCodeCount += codeStats->foundCodeOnLine;
                codeStats->foundCodeOnLine = 0;
                state = START;
            }
            else if(ch == '/')
            {
                state = INCPPCOMMENT;
                codeStats->cplusplusCommentCount++;
            }
            else if(ch == '*')
            {
                state = INCCOMMENT;
                codeStats->cCommentCount++;
            }
            else {
                codeStats->foundCodeOnLine = 1;
                state = START;
            }
            break;

        case FOUNDSTAR:
            if (ch == '\n') {
                codeStats->lineCount++;
                codeStats->lineWithCodeCount += codeStats->foundCodeOnLine;
                codeStats->foundCodeOnLine = 0;
                state = INCCOMMENT;
            }
            else if(ch == '/')
            {
                state = START;
            }
            else
            {
                state = INCCOMMENT;
            }
            break;

        case INCPPCOMMENT:
            if (ch == '\n') {
                codeStats->lineCount++;
                codeStats->lineWithCodeCount += codeStats->foundCodeOnLine;
                codeStats->foundCodeOnLine = 0;
                state = START;
            }
            break;
        
        case INCCOMMENT:
            if (ch == '\n') {
                codeStats->lineCount++;
                codeStats->lineWithCodeCount += codeStats->foundCodeOnLine;
                codeStats->foundCodeOnLine = 0;
            }
            else if(ch == '*')
            {
                state = FOUNDSTAR;
            }
            break;
        default:
            assert(0);
            break;
        }
    }
    fclose(f);
    assert(state == START);
}


int main(int argc, char *argv[])
{
    struct CodeStats codeStats;
    int i;

    for (i = 1; i < argc; i++) {
        codeStats_init(&codeStats);
        codeStats_accumulate(&codeStats, argv[i]);
        codeStats_print(codeStats, argv[i]); // no "&" -- see why?
        if (i != argc-1)   // if this wasn't the last file ...
            printf("\n");  // ... print out a separating newline
    }
    return 0;
}