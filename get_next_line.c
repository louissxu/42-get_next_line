#include "get_next_line.h"

char    *get_next_line(int fd)
{
    static char *buff_remaining[FD_MAX];

    // grow_buffer(pointer, fd)
    // find end char
    // gnl start to end
    // extract out the new line and return from buff
    // grow buff - keeps filling chunks to buff until it finds a \n or \0 char
    // return buff
    // if new line not found

}