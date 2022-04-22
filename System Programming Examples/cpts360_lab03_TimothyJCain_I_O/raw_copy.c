#include <stdio.h> 
#include <stdlib.h>
#include "allocarray.h"
#include "syscall_check.h"
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char* argv[])
{
    // if(argc != 4)
    // {
    //     //printf("Missing parameters: ./raw_copy BUF_SIZE INPUT_FILE OUTPUT_FILE");
    //     fprintf(stderr, "Missing parameters: ./raw_copy BUF_SIZE INPUT_FILE OUTPUT_FILE\n");
    //     abort();
    // }
    // //assert(argc == 4);
    int bufferSize = atoi(argv[1]);
    char *buffer;
    ALLOC_ARRAY(buffer, char, bufferSize);
    int input = open(argv[2], O_RDONLY);
    SYSCALL_CHECK(input);
    int output = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0700);
    SYSCALL_CHECK(output);
    int bytes_read = read(input, buffer, bufferSize);
    SYSCALL_CHECK(bytes_read);
    while(bytes_read != 0)
    {
        write(output, buffer, bytes_read);
        bytes_read = read(input, buffer, bufferSize);
    }
    free(buffer);
    close(input);
    close(output);
    return 0;
}

